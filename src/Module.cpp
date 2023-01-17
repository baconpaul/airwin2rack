#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <atomic>

#include "AirwinRegistry.h"

// @TODO: Param Smoothing
// @TODO: Update README
// @TODO: A dark and light mode
// @TODO: Cleanup plugin.json

struct BufferedDrawFunctionWidget : virtual rack::FramebufferWidget
{
    typedef std::function<void(NVGcontext *)> drawfn_t;
    drawfn_t drawf;

    struct InternalBDW : rack::TransparentWidget
    {
        drawfn_t drawf;
        InternalBDW(rack::Rect box_, drawfn_t draw_) : drawf(draw_) { box = box_; }

        void draw(const DrawArgs &args) override { drawf(args.vg); }
    };

    InternalBDW *kid = nullptr;
    BufferedDrawFunctionWidget(rack::Vec pos, rack::Vec sz, drawfn_t draw_) : drawf(draw_)
    {
        box.pos = pos;
        box.size = sz;
        auto kidBox = rack::Rect(rack::Vec(0, 0), box.size);
        kid = new InternalBDW(kidBox, drawf);
        addChild(kid);
    }
};

struct AW2RModule : virtual rack::Module
{
    static constexpr int maxParams{11};

    std::unique_ptr<Airwin2RackBase> airwin{}, airwin_display{};
    std::atomic<int32_t> forceSelect{-1}, resetCount{0};
    std::string selectedFX{}, selectedWhat{};

    struct AWParamQuantity : public rack::ParamQuantity
    {
        std::string getDisplayValueString() override
        {
            auto awm = dynamic_cast<AW2RModule *>(module);
            if (awm && awm->airwin_display)
            {
                int idx = paramId - PARAM_0;
                if (idx < awm->nParams)
                {
                    char txt[256]{0}, lab[256]{0};
                    awm->airwin_display->setParameter(idx, getValue());
                    awm->airwin_display->getParameterDisplay(idx, txt);
                    awm->airwin_display->getParameterLabel(idx, lab);
                    auto ls = std::string(lab);
                    return std::string(txt) + (ls.empty() ? "" : " " + ls);
                }
            }
            return ParamQuantity::getDisplayValueString();
        }
    };


    enum ParamIds
    {
        PARAM_0,
        ATTENUVERTER_0 = PARAM_0 + maxParams,
        NUM_PARAMS = ATTENUVERTER_0 + maxParams
    };

    enum InputIds
    {
        INPUT_L,
        INPUT_R,
        CV_0,
        NUM_INPUTS = CV_0 + maxParams
    };

    enum OutputIds
    {
        OUTPUT_L,
        OUTPUT_R,
        NUM_OUTPUTS
    };

    enum LightIds
    {
        NUM_LIGHTS
    };

    int nParams{0};
    AW2RModule()
    {
        assert(!AirwinRegistry::registry.empty());
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        memset(indat, 0, 2 * block * sizeof(float));
        memset(outdat, 0, 2 * block * sizeof(float));
        in[0] = &(indat[0]);
        in[1] = &(indat[block]);
        out[0] = &(outdat[0]);
        out[1] = &(outdat[block]);

        configBypass(INPUT_L, OUTPUT_L);
        configBypass(INPUT_R, OUTPUT_R);

        for (int i = 0; i < maxParams; ++i)
        {
            configParam<AWParamQuantity>(PARAM_0 + i, 0, 1, 0, "Param " + std::to_string(i));
            configParam(ATTENUVERTER_0 + i, -1, 1, 0, "CV Scale " + std::to_string(i));
        }
        resetAirwinByName("Galactic", true);
    }

    void resetAirwindowTo(int registryIdx, bool resetValues = true)
    {
        selectedFX = AirwinRegistry::registry[registryIdx].name;
        selectedWhat = AirwinRegistry::registry[registryIdx].whatText;
        airwin = AirwinRegistry::registry[registryIdx].generator();
        airwin_display = AirwinRegistry::registry[registryIdx].generator();
        nParams = AirwinRegistry::registry[registryIdx].nParams;

        for (int i=0; i<nParams; ++i)
        {
            char txt[256];
            airwin->getParameterName(i, txt);
            paramQuantities[PARAM_0 + i]->name = txt;
            paramQuantities[ATTENUVERTER_0 + i]->name = std::string(txt) + " CV Scale";
            paramQuantities[PARAM_0 + i]->defaultValue = airwin->getParameter(i);
            if (resetValues)
                paramQuantities[PARAM_0 + i]->setValue(paramQuantities[i]->defaultValue);
        }

        resetCount++;
    }

    json_t *dataToJson() override {
        auto res = json_object();
        json_object_set(res, "airwindowSelectedFX", json_string(selectedFX.c_str()));
        return res;
    }

    void dataFromJson(json_t *rootJ) override {
        auto awfx = json_object_get(rootJ, "airwindowSelectedFX");
        if (awfx)
        {
            std::string sfx = json_string_value(awfx);
            resetAirwinByName(sfx, false);
        }
    }

    void resetAirwinByName(const std::string &sfx, bool reset)
    {
        for (auto i=0U; i<AirwinRegistry::registry.size(); ++i)
        {
            if (AirwinRegistry::registry[i].name == sfx)
            {
                resetAirwindowTo(i, reset);
            }
        }
    }

    static constexpr int block{4};

    float *in[2], *out[2];
    float indat[2 * block], outdat[2 * block];
    int inPos{0}, outPos{0};

    void process(const ProcessArgs &args) override
    {
        if (forceSelect != -1) // a UI action doesn't warrant the compare_exchange rigamarole
        {
            resetAirwindowTo(forceSelect);
            forceSelect = -1;
        }
        auto rc = inputs[INPUT_R].isConnected() ? INPUT_R : INPUT_L;
        in[0][inPos] = inputs[INPUT_L].getVoltageSum() * 0.2;
        in[1][inPos] = inputs[rc].getVoltageSum() * 0.2;
        inPos++;
        if (inPos == block)
        {
            for (int i = 0; i < nParams; ++i)
            {
                auto pv = params[PARAM_0 + i].getValue();
                if (inputs[CV_0 + i].isConnected())
                {
                    auto v = inputs[CV_0 + i].getVoltage() * 0.2 *
                        params[ATTENUVERTER_0 + i].getValue();
                    pv = std::clamp(pv + v, 0., 1.);
                }
                airwin->setParameter(i, pv);
            }
            airwin->processReplacing(in, out, block);
            outPos = 0;
            inPos = 0;
        }

        outputs[OUTPUT_L].setVoltage(out[0][outPos] * 5);
        outputs[OUTPUT_R].setVoltage(out[1][outPos] * 5);
        outPos++;
    }
};

template <int px, bool bipolar = false>
struct PixelKnob : rack::Knob
{
    PixelKnob()
    {
        box.size = rack::Vec(px, px);
        float angleSpreadDegrees = 40.0;

        minAngle = -M_PI * (180 - angleSpreadDegrees) / 180;
        maxAngle = M_PI * (180 - angleSpreadDegrees) / 180;
    }

    void draw(const DrawArgs &args)
    {
        auto vg = args.vg;
        float radius = box.size.x * 0.48;
        nvgBeginPath(vg);
        nvgEllipse(vg, box.size.x * 0.5, box.size.y * 0.5, radius, radius);
        nvgFillPaint(vg, nvgRadialGradient(vg, box.size.x * 0.5, box.size.y * 0.5,
                                           box.size.x * 0.1, box.size.x * 0.4,
                                           nvgRGB(110,110,120), nvgRGB(110,110,130)));
        nvgStrokeColor(vg, nvgRGB(20,20,20));
        nvgStrokeWidth(vg, 0.5);
        nvgFill(vg);
        nvgStroke(vg);

        auto pq = getParamQuantity();
        if (!pq)
            return;

        nvgBeginPath(vg);
        float angle = rack::math::rescale(pq->getValue(), pq->getMinValue(), pq->getMaxValue(), minAngle, maxAngle);
        float startAngle = minAngle;
        if (bipolar)
            startAngle = 0;

        nvgBeginPath(vg);
        nvgArc(vg, box.size.x * 0.5, box.size.y * 0.5, radius, startAngle - M_PI_2, angle - M_PI_2,
               startAngle < angle ? NVG_CW : NVG_CCW);
        nvgStrokeWidth(vg, 1);
        nvgStrokeColor(vg, nvgRGB(255,255,255));
        nvgLineCap(vg, NVG_ROUND);
        nvgStroke(vg);

        auto ox = std::sin(angle) * radius + box.size.x / 2;
        auto oy = box.size.y - (std::cos(angle) * radius + box.size.y / 2);

        auto ix = std::sin(angle) * radius * 0.4 + box.size.x / 2;
        auto iy = box.size.y - (std::cos(angle) * radius * 0.4 + box.size.y / 2);

        nvgBeginPath(vg);
        nvgMoveTo(vg, ox, oy);
        nvgLineTo(vg, ix, iy);
        nvgStrokeColor(vg, nvgRGB(220,220,230));
        nvgStrokeWidth(vg, 1);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgEllipse(vg, ox, oy, 1.5, 1.5);
        nvgFillColor(vg, nvgRGB(255,255,255));
        nvgStrokeColor(vg, nvgRGB(20,20,20));
        nvgStrokeWidth(vg, 0.5);
        nvgStroke(vg);
        nvgFill(vg);

    }
};

struct AWLabel : rack::Widget
{
    float px{11};
    std::string label{"label"};
    std::string fontPath;
    AWLabel() { fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf"); }
    void draw(const DrawArgs &args) override
    {
        // @TODO: Double Buffer
        auto vg = args.vg;
        auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(220, 220, 220));
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_LEFT);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, px);

        nvgText(vg, 0, box.size.y * 0.5, label.c_str(), nullptr);
        float bnd[4];
        nvgTextBounds(vg, 0, box.size.y*0.5, label.c_str(), nullptr, bnd);
        nvgBeginPath(vg);
        nvgMoveTo(vg, bnd[2] + 4, box.size.y * 0.5);
        nvgLineTo(vg, box.size.x - 4, box.size.y * 0.5);
        nvgStrokeColor(vg, nvgRGB(110,110,120));
        nvgStroke(vg);
    }
};

struct AWSelector : rack::Widget
{
    AW2RModule *module;
    std::string fontPath;
    AWSelector() {
        fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf");
    }

    void draw(const DrawArgs &args) override
    {
        auto vg = args.vg;
        // auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(20, 20, 20));
        nvgStrokeColor(vg, nvgRGB(140,140,160));
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 3);
        nvgFill(vg);
        nvgStrokeWidth(vg, 1);
        nvgStroke(vg);

        auto nm = (module ? module->selectedFX : "Airwindows");
        auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(220, 220, 220));
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 14);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.5, nm.c_str(), nullptr);
    }

    void onButton(const ButtonEvent &e) override {
        if (module && e.action == GLFW_PRESS)
        {
            showSelectorMenu();
            e.consume(this);
        }
        Widget::onButton(e);
    }

    void showSelectorMenu()
    {
        if (!module)
            return;

        auto m = rack::createMenu();
        m->addChild(rack::createMenuLabel("Airwindows Selector"));
        m->addChild(new rack::MenuSeparator);
        for (const auto &cat : AirwinRegistry::categories)
        {
            m->addChild(rack::createSubmenuItem(
                cat, "", [this, cat](auto *m) { createCategoryMenu(m, cat); }));
        }
    }
    void createCategoryMenu(rack::Menu *m, const std::string &cat)
    {
        std::map<std::string, int> contents;
        int idx = 0;
        for (const auto &item : AirwinRegistry::registry)
        {
            if (item.category == cat)
            {
                contents[item.name] = idx;
            }
            idx++;
        }
        for (const auto &[name, idx] : contents)
        {
            auto checked = name == module->selectedFX;
            m->addChild(rack::createMenuItem(name, CHECKMARK(checked),
                                             [this, i = idx](){module->forceSelect = i;}));
        }
    }

    void onHover(const HoverEvent &e) override {
        e.consume(this);
    }
    void onEnter(const EnterEvent &e) override {
        e.consume(this);
        if (!module)
            return;
        if (module->selectedWhat.empty())
            return;

        if (!rack::settings::tooltips)
            return;
        if (toolTip)
            return;

        toolTip = new rack::ui::Tooltip;
        toolTip->text = module->selectedWhat;
        APP->scene->addChild(toolTip);
    }
    void onLeave(const LeaveEvent &e) override {
        e.consume(this);
        if (!toolTip)
            return;
        APP->scene->removeChild(toolTip);
        delete toolTip;
        toolTip = nullptr;
    }

    rack::ui::Tooltip *toolTip{nullptr};

};

struct AW2RModuleWidget : rack::ModuleWidget
{
    typedef AW2RModule M;

    std::array<AWLabel *, M::maxParams> parLabels;
    std::array<rack::ParamWidget *, M::maxParams> parKnobs, attenKnobs;
    std::array<rack::PortWidget *, M::maxParams> cvPorts;
    std::string fontPath;

    std::shared_ptr<rack::Svg> clipperSvg;

    AW2RModuleWidget(M *m)
    {
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 11, RACK_HEIGHT);

        fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf");
        clipperSvg = rack::Svg::load(rack::asset::plugin(pluginInstance, "res/clipper.svg"));

        auto bg = new BufferedDrawFunctionWidget(rack::Vec(0,0), box.size,
                                                 [this](auto vg) {drawBG(vg); });
        bg->box.pos = rack::Vec(0.0);
        bg->box.size = box.size;
        addChild(bg);

        int headerSize{35};

        auto tlab = new AWSelector;
        auto s = box;
        s.size.y = headerSize;
        s = s.shrink(rack::Vec(5,5));
        tlab->box = s;
        tlab->module = m;
        addChild(tlab);

        auto pPos = headerSize + 2, dPP = 25;

        for (int i = 0; i < M::maxParams; ++i)
        {
            auto tlab = new AWLabel;
            tlab->px = 11;
            tlab->box.pos.x = 5;
            tlab->box.pos.y = pPos;
            tlab->box.size.x = box.size.x - 80;
            tlab->box.size.y = dPP;
            tlab->label = "Param " + std::to_string(i);
            parLabels[i] = tlab;
            addChild(tlab);

            auto bp = box.size.x - 65;
            parKnobs[i] = rack::createParamCentered<PixelKnob<18>>(
                rack::Vec(bp, pPos + dPP * 0.5), module, M::PARAM_0 + i);
            addParam(parKnobs[i]);

            attenKnobs[i] = rack::createParamCentered<PixelKnob<12, true>>(
                rack::Vec(bp + 22, pPos + dPP * 0.5), module, M::ATTENUVERTER_0 + i);
            addParam(attenKnobs[i]);

            cvPorts[i] = rack::createInputCentered<rack::PJ301MPort>(rack::Vec(bp + 45, pPos + dPP * 0.5), module, M::CV_0 + i);
            addInput(cvPorts[i]);

            pPos += dPP;
        }

        // @TODO: paint labels
        auto q = RACK_HEIGHT - 42;
        auto c1 = box.size.x * 0.25;
        auto dc = box.size.x * 0.11;
        auto c2 = box.size.x * 0.75;
        addInput(rack::createInputCentered<rack::PJ301MPort>(rack::Vec(c1 - dc, q), module, M::INPUT_L));
        addInput(rack::createInputCentered<rack::PJ301MPort>(rack::Vec(c1 + dc, q), module, M::INPUT_R));
        addOutput(
            rack::createOutputCentered<rack::PJ301MPort>(rack::Vec(c2 - dc, q), module, M::OUTPUT_L));
        addOutput(
            rack::createOutputCentered<rack::PJ301MPort>(rack::Vec(c2 + dc, q), module, M::OUTPUT_R));
    }

    void drawBG(NVGcontext *vg)
    {
        auto cutPoint{58};

        // Main Gradient Background
        nvgBeginPath(vg);
        nvgFillPaint(vg, nvgLinearGradient(vg, 0, 50, 0, box.size.y - cutPoint,
                                           nvgRGB(50,50,60), nvgRGB(70,70,75)));
        nvgRect(vg, 0, 0, box.size.x, box.size.y - cutPoint);
        nvgFill(vg);
        nvgStroke(vg);


        // Draw the bottom region
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(160,160,170));
        nvgStrokeColor(vg, nvgRGB(0,0,0));
        nvgStrokeWidth(vg, 0.5);
        nvgRect(vg, 0, box.size.y - cutPoint, box.size.x, cutPoint);
        nvgFill(vg);
        nvgStroke(vg);

        // Input region
        auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(140,140,150));
        nvgFillColor(vg, nvgRGB(190,190,200));
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, 4, box.size.y - cutPoint + 3,
                       box.size.x * 0.5 - 8, 37, 2);
        nvgFill(vg);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(40,40,50));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 10);
        nvgText(vg, box.size.x * 0.25, box.size.y - cutPoint + 38, "L  IN  R", nullptr);

        // Output region
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(40,40,50));
        nvgFillColor(vg, nvgRGB(60,60,70));
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, box.size.x * 0.5 + 4, box.size.y - cutPoint + 3,
                       box.size.x * 0.5 - 8, 37, 2);
        nvgFill(vg);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(190,190,200));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 10);
        nvgText(vg, box.size.x * 0.75, box.size.y - cutPoint + 38, "L  OUT  R", nullptr);

        // Brand
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(0,0,0));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 14);
        nvgText(vg, box.size.x * 0.5, box.size.y - 2, "Airwindows", nullptr);

        if (clipperSvg)
        {
            nvgSave(vg);
            float t[6];
            nvgTranslate(vg, 0, 218);
            nvgScale(vg, 0.31, 0.31);
            nvgAlpha(vg, 0.73);
            clipperSvg->draw(vg);
            nvgRestore(vg);
        }

        // Outline the module
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(100, 100, 100));
        nvgStrokeWidth(vg, 1);
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
        nvgStroke(vg);
    }

    int resetCountCache{-1};
    void step() override
    {
        if (module)
        {
            auto awm = dynamic_cast<AW2RModule *>(module);
            if (awm && awm->resetCount != resetCountCache)
            {
                resetCountCache = awm->resetCount;
                resetAirwinDisplay();
            }
        }

        rack::ModuleWidget::step();
    }

    void resetAirwinDisplay()
    {
        auto awm = dynamic_cast<AW2RModule *>(module);

        if (!awm)
            return; // should never happen but hey

        int np = awm->nParams;
        for (int i=0; i<np; ++i)
        {
            parLabels[i]->setVisible(true);
            char txt[256];
            awm->airwin->getParameterName(i, txt);
            parLabels[i]->label = txt;
            parKnobs[i]->setVisible(true);
            attenKnobs[i]->setVisible(true);
            cvPorts[i]->setVisible(true);
        }
        for (int i=np; i<M::maxParams; ++i)
        {
            parLabels[i]->setVisible(false);
            parKnobs[i]->setVisible(false);
            attenKnobs[i]->setVisible(false);
            cvPorts[i]->setVisible(false);
        }
    }
};

rack::Model *airwin2RackModel = rack::createModel<AW2RModule, AW2RModuleWidget>("Airwin2Rack");