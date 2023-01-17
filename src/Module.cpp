#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <atomic>

#include "AirwinRegistry.h"

// @TODO: Total ordering and Jog Buttons
// @TODO: Poly
// @TODO: Unused modules go semi-transparent

#define MAX_POLY 16

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
    std::array<std::unique_ptr<Airwin2RackBase>, MAX_POLY> poly_airwin;
    std::atomic<int32_t> forceSelect{-1}, resetCount{0};
    std::string selectedFX{}, selectedWhat{}, selectedCat{};

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
    std::atomic<bool> polyphonic{false};
    AW2RModule()
    {
        assert(!AirwinRegistry::registry.empty());
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

        configBypass(INPUT_L, OUTPUT_L);
        configBypass(INPUT_R, OUTPUT_R);

        for (int i = 0; i < maxParams; ++i)
        {
            auto pq =
                configParam<AWParamQuantity>(PARAM_0 + i, 0, 1, 0, "Param " + std::to_string(i));
            pq->smoothEnabled = true;

            auto av = configParam(ATTENUVERTER_0 + i, -1, 1, 0, "CV Scale " + std::to_string(i));
            av->randomizeEnabled = false;

            configInput(CV_0 + i, "CV " + std::to_string(i));
        }
        resetAirwinByName("Galactic", true);
    }

    void resetAirwindowTo(int registryIdx, bool resetValues = true)
    {
        selectedFX = AirwinRegistry::registry[registryIdx].name;
        selectedCat = AirwinRegistry::registry[registryIdx].category;
        selectedWhat = AirwinRegistry::registry[registryIdx].whatText;

        airwin_display = AirwinRegistry::registry[registryIdx].generator();

        if (polyphonic)
        {
            airwin.reset(nullptr);
            for (int i = 0; i < MAX_POLY; ++i)
            {
                poly_airwin[i] = AirwinRegistry::registry[registryIdx].generator();
            }
        }
        else
        {
            airwin = AirwinRegistry::registry[registryIdx].generator();
            for (auto &aw : poly_airwin)
                aw.reset(nullptr);
        }
        nParams = AirwinRegistry::registry[registryIdx].nParams;

        for (int i = 0; i < nParams; ++i)
        {
            char txt[256];
            airwin_display->getParameterName(i, txt);
            paramQuantities[PARAM_0 + i]->name = txt;
            paramQuantities[ATTENUVERTER_0 + i]->name = std::string(txt) + " CV Scale";
            paramQuantities[PARAM_0 + i]->defaultValue = airwin_display->getParameter(i);
            inputInfos[CV_0 + i]->name = std::string(txt) + " CV";
            if (resetValues)
                paramQuantities[PARAM_0 + i]->setValue(paramQuantities[i]->defaultValue);
        }

        for (int i = nParams; i < maxParams; ++i)
        {
            inputInfos[CV_0 + i]->name = "Unused CV";
        }

        resetCount++;
    }

    json_t *dataToJson() override
    {
        auto res = json_object();
        json_object_set(res, "airwindowSelectedFX", json_string(selectedFX.c_str()));
        json_object_set(res, "polyphonic", json_boolean(polyphonic));
        return res;
    }

    void dataFromJson(json_t *rootJ) override
    {
        auto awfx = json_object_get(rootJ, "airwindowSelectedFX");
        if (awfx)
        {
            std::string sfx = json_string_value(awfx);
            resetAirwinByName(sfx, false);
        }
        auto awpl = json_object_get(rootJ, "polyphonic");
        if (awpl)
        {
            auto bl = json_boolean_value(awpl);
            resetPolyphony(bl);
        }
    }

    bool nextPoly{polyphonic};
    void stagePolyReset(bool b)
    {
        nextPoly = b;
    }

    void resetPolyphony(bool isPoly)
    {
        if (isPoly == polyphonic)
            return;

        polyphonic = isPoly;

        resetAirwinByName(selectedFX, false);
        if (polyphonic)
        {
            for (int i=0; i<MAX_POLY; ++i)
            {
                polyIO[i].reset();
            }
        }
        else
        {
            monoIO.reset();
        }
    }

    void resetAirwinByName(const std::string &sfx, bool reset)
    {
        for (auto i = 0U; i < AirwinRegistry::registry.size(); ++i)
        {
            if (AirwinRegistry::registry[i].name == sfx)
            {
                resetAirwindowTo(i, reset);
            }
        }
    }

    static constexpr int block{4};

    struct IOHolder
    {
        IOHolder() { reset(); }
        void reset() {
            memset(indat, 0, 2 * block * sizeof(float));
            memset(outdat, 0, 2 * block * sizeof(float));
            in[0] = &(indat[0]);
            in[1] = &(indat[block]);
            out[0] = &(outdat[0]);
            out[1] = &(outdat[block]);
        }

        float *in[2], *out[2];
        float indat[2 * block], outdat[2 * block];
        int inPos{0}, outPos{0};
    } monoIO, polyIO[MAX_POLY];

    void process(const ProcessArgs &args) override
    {
        if (nextPoly != polyphonic)
        {
            resetPolyphony(nextPoly);
        }
        if (forceSelect != -1) // a UI action doesn't warrant the compare_exchange rigamarole
        {
            resetAirwindowTo(forceSelect);
            forceSelect = -1;
        }

        if (polyphonic)
        {
            processPoly(args);
        }
        else
        {
            processMono(args);
        }
    }
    void processMono(const ProcessArgs &args)
    {
        auto rc = inputs[INPUT_R].isConnected() ? INPUT_R : INPUT_L;
        monoIO.in[0][monoIO.inPos] = inputs[INPUT_L].getVoltageSum() * 0.2;
        monoIO.in[1][monoIO.inPos] = inputs[rc].getVoltageSum() * 0.2;
        monoIO.inPos++;
        if (monoIO.inPos == block)
        {
            for (int i = 0; i < nParams; ++i)
            {
                auto pv = paramQuantities[PARAM_0 + i]->getSmoothValue();
                if (inputs[CV_0 + i].isConnected())
                {
                    auto v =
                        inputs[CV_0 + i].getVoltage() * 0.2 * params[ATTENUVERTER_0 + i].getValue();
                    pv = std::clamp(pv + v, 0., 1.);
                }
                airwin->setParameter(i, pv);
            }
            airwin->processReplacing(monoIO.in, monoIO.out, block);
            monoIO.outPos = 0;
            monoIO.inPos = 0;
        }

        outputs[OUTPUT_L].setVoltage(monoIO.out[0][monoIO.outPos] * 5);
        outputs[OUTPUT_R].setVoltage(monoIO.out[1][monoIO.outPos] * 5);
        monoIO.outPos++;
    }

    void processPoly(const ProcessArgs &args)
    {
        int chanCt = std::max({1, inputs[INPUT_R].getChannels(), inputs[INPUT_L].getChannels()});
        outputs[OUTPUT_L].setChannels(chanCt);
        outputs[OUTPUT_R].setChannels(chanCt);

        auto rc = inputs[INPUT_R].isConnected() ? INPUT_R : INPUT_L;

        for (int c = 0; c<chanCt; ++c)
        {
            polyIO[c].in[0][polyIO[c].inPos] = inputs[INPUT_L].getVoltage(c) * 0.2;
            polyIO[c].in[1][polyIO[c].inPos] = inputs[rc].getVoltage(c) * 0.2;
            polyIO[c].inPos++;
            if (polyIO[c].inPos == block)
            {
                for (int i = 0; i < nParams; ++i)
                {
                    auto pv = paramQuantities[PARAM_0 + i]->getSmoothValue();
                    if (inputs[CV_0 + i].isConnected())
                    {
                        auto v = inputs[CV_0 + i].getVoltage(inputs[CV_0 + i].getChannels() == 1 ? 0 : c) * 0.2 *
                                 params[ATTENUVERTER_0 + i].getValue();
                        pv = std::clamp(pv + v, 0., 1.);
                    }
                    poly_airwin[c]->setParameter(i, pv);
                }
                poly_airwin[c]->processReplacing(polyIO[c].in, polyIO[c].out, block);
                polyIO[c].outPos = 0;
                polyIO[c].inPos = 0;
            }

            outputs[OUTPUT_L].setVoltage(polyIO[c].out[0][polyIO[c].outPos] * 5, c);
            outputs[OUTPUT_R].setVoltage(polyIO[c].out[1][polyIO[c].outPos] * 5, c);
            polyIO[c].outPos++;
        }
    }
};

struct AWSkin
{
    enum Skin
    {
        LIGHT,
        DARK
    } skin{DARK};

    void changeTo(Skin s, bool write)
    {
        skin = s;

        if (write)
        {
            std::string defaultsDir = rack::asset::user("Airwin2Rack/");
            if (!rack::system::isDirectory(defaultsDir))
                rack::system::createDirectory(defaultsDir);
            std::string defaultsFile = rack::asset::user("Airwin2Rack/default-skin.json");

            json_t *rootJ = json_object();
            json_object_set_new(rootJ, "defaultSkin", json_integer(skin));

            FILE *f = std::fopen(defaultsFile.c_str(), "w");
            if (f)
            {
                json_dumpf(rootJ, f, JSON_INDENT(2));
                std::fclose(f);
            }
            json_decref(rootJ);
        }
    }

    AWSkin()
    {
        std::string defaultsFile = rack::asset::user("Airwin2Rack/default-skin.json");
        json_error_t error;
        json_t *fd{nullptr};
        auto *fptr = std::fopen(defaultsFile.c_str(), "r");
        if (fptr)
        {
            fd = json_loadf(fptr, 0, &error);
            DEFER({ std::fclose(fptr); });
        }
        if (!fd)
        {
            changeTo(DARK, false);
        }
        else
        {
            json_t *defj = json_object_get(fd, "defaultSkin");
            int skinId{DARK};
            if (defj)
                skinId = json_integer_value(defj);
            if (skinId != DARK && skinId != LIGHT)
                skinId = DARK;
            changeTo((Skin)skinId, false);
        }
    }
};

AWSkin skinManager;

template <int px, bool bipolar = false> struct PixelKnob : rack::Knob
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
        if (skinManager.skin == AWSkin::DARK)
        {
            nvgFillPaint(vg, nvgRadialGradient(vg, box.size.x * 0.5, box.size.y * 0.5,
                                               box.size.x * 0.1, box.size.x * 0.4,
                                               nvgRGB(110, 110, 120), nvgRGB(110, 110, 130)));
            nvgStrokeColor(vg, nvgRGB(20, 20, 20));
        }
        else
        {
            nvgFillPaint(vg, nvgRadialGradient(vg, box.size.x * 0.5, box.size.y * 0.5,
                                               box.size.x * 0.1, box.size.x * 0.4,
                                               nvgRGB(185, 186, 220), nvgRGB(190, 190, 220)));
            nvgStrokeColor(vg, nvgRGB(50, 50, 60));
        }
        nvgStrokeWidth(vg, 0.5);
        nvgFill(vg);
        nvgStroke(vg);

        auto pq = getParamQuantity();
        if (!pq)
            return;

        nvgBeginPath(vg);
        float angle = rack::math::rescale(pq->getValue(), pq->getMinValue(), pq->getMaxValue(),
                                          minAngle, maxAngle);
        float startAngle = minAngle;
        if (bipolar)
            startAngle = 0;

        auto valueFill = nvgRGB(240, 240, 240);
        if (skinManager.skin == AWSkin::LIGHT)
            valueFill = nvgRGB(20, 20, 20);

        nvgBeginPath(vg);
        nvgArc(vg, box.size.x * 0.5, box.size.y * 0.5, radius, startAngle - M_PI_2, angle - M_PI_2,
               startAngle < angle ? NVG_CW : NVG_CCW);
        nvgStrokeWidth(vg, 1);
        nvgStrokeColor(vg, valueFill);
        nvgLineCap(vg, NVG_ROUND);
        nvgStroke(vg);

        auto ox = std::sin(angle) * radius + box.size.x / 2;
        auto oy = box.size.y - (std::cos(angle) * radius + box.size.y / 2);

        auto ix = std::sin(angle) * radius * 0.4 + box.size.x / 2;
        auto iy = box.size.y - (std::cos(angle) * radius * 0.4 + box.size.y / 2);

        nvgBeginPath(vg);
        nvgMoveTo(vg, ox, oy);
        nvgLineTo(vg, ix, iy);
        nvgStrokeColor(vg, valueFill);
        nvgStrokeWidth(vg, 1);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgEllipse(vg, ox, oy, 1.5, 1.5);
        nvgFillColor(vg, valueFill);
        nvgStrokeColor(vg, nvgRGB(20, 20, 20));
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
    BufferedDrawFunctionWidget *bdw{nullptr};
    AWLabel() { fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf"); }
    void setup()
    {
        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawLabel(vg); });
        addChild(bdw);
    }
    void drawLabel(NVGcontext *vg)
    {
        auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        if (skinManager.skin == AWSkin::DARK)
            nvgFillColor(vg, nvgRGB(220, 220, 220));
        else
            nvgFillColor(vg, nvgRGB(20, 20, 20));

        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_LEFT);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, px);

        nvgText(vg, 0, box.size.y * 0.5, label.c_str(), nullptr);
        float bnd[4];
        nvgTextBounds(vg, 0, box.size.y * 0.5, label.c_str(), nullptr, bnd);
        nvgBeginPath(vg);
        nvgMoveTo(vg, bnd[2] + 4, box.size.y * 0.5);
        nvgLineTo(vg, box.size.x - 4, box.size.y * 0.5);
        if (skinManager.skin == AWSkin::DARK)
            nvgStrokeColor(vg, nvgRGB(110, 110, 120));
        else
            nvgStrokeColor(vg, nvgRGB(150, 150, 160));

        nvgStrokeWidth(vg, 0.5);
        nvgStroke(vg);
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    std::string lastLabel{};
    void step() override
    {
        if (bdw)
        {
            if (lastLabel != label || lastSkin != skinManager.skin)
            {
                bdw->dirty = true;
            }
            lastLabel = label;
            lastSkin = skinManager.skin;
        }
        rack::Widget::step();
    }
};

struct AWSelector : rack::Widget
{
    AW2RModule *module;
    std::string fontPath;
    AWSelector() { fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf"); }

    BufferedDrawFunctionWidget *bdw{nullptr};
    void setup()
    {
        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawSelector(vg); });
        addChild(bdw);
    }

    void drawSelector(NVGcontext *vg)
    {
        // auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(20, 20, 20));
        nvgStrokeColor(vg, nvgRGB(140, 140, 160));
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 3);
        nvgFill(vg);
        nvgStrokeWidth(vg, 1);
        nvgStroke(vg);

        auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(240, 240, 240));
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 15);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.67, lastName.c_str(), nullptr);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(220, 220, 220));
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 8);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.21, lastCat.c_str(), nullptr);
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    std::string lastName{"Airwindows"}, lastCat{"Multi-Effect"};
    void step() override
    {
        if (module && bdw)
        {
            if (lastName != module->selectedFX || lastCat != module->selectedCat ||
                lastSkin != skinManager.skin)
            {
                bdw->dirty = true;
            }
            lastName = module->selectedFX;
            lastCat = module->selectedCat;
            lastSkin = skinManager.skin;
        }
        rack::Widget::step();
    }

    void onButton(const ButtonEvent &e) override
    {
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
                                             [this, i = idx]() { module->forceSelect = i; }));
        }
    }

    void onHover(const HoverEvent &e) override { e.consume(this); }
    void onEnter(const EnterEvent &e) override
    {
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
    void onLeave(const LeaveEvent &e) override
    {
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
    BufferedDrawFunctionWidget *bg{nullptr};

    AW2RModuleWidget(M *m)
    {
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 11, RACK_HEIGHT);

        fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf");
        clipperSvg = rack::Svg::load(rack::asset::plugin(pluginInstance, "res/clipper.svg"));

        bg = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                            [this](auto vg) { drawBG(vg); });
        bg->box.pos = rack::Vec(0.0);
        bg->box.size = box.size;
        addChild(bg);

        int headerSize{38};

        auto tlab = new AWSelector;
        auto s = box;
        s.size.y = headerSize;
        s = s.shrink(rack::Vec(5, 5));
        tlab->box = s;
        tlab->module = m;
        tlab->setup();
        addChild(tlab);

        auto pPos = headerSize + 1, dPP = 25;

        for (int i = 0; i < M::maxParams; ++i)
        {
            auto tlab = new AWLabel;
            tlab->px = 11;
            tlab->box.pos.x = 5;
            tlab->box.pos.y = pPos;
            tlab->box.size.x = box.size.x - 80;
            tlab->box.size.y = dPP;
            tlab->label = "Param " + std::to_string(i);
            tlab->setup();
            parLabels[i] = tlab;
            addChild(tlab);

            auto bp = box.size.x - 65;
            parKnobs[i] = rack::createParamCentered<PixelKnob<18>>(rack::Vec(bp, pPos + dPP * 0.5),
                                                                   module, M::PARAM_0 + i);
            addParam(parKnobs[i]);

            attenKnobs[i] = rack::createParamCentered<PixelKnob<12, true>>(
                rack::Vec(bp + 22, pPos + dPP * 0.5), module, M::ATTENUVERTER_0 + i);
            addParam(attenKnobs[i]);

            cvPorts[i] = rack::createInputCentered<rack::PJ301MPort>(
                rack::Vec(bp + 45, pPos + dPP * 0.5), module, M::CV_0 + i);
            addInput(cvPorts[i]);

            pPos += dPP;
        }

        auto q = RACK_HEIGHT - 42;
        auto c1 = box.size.x * 0.25;
        auto dc = box.size.x * 0.11;
        auto c2 = box.size.x * 0.75;
        addInput(
            rack::createInputCentered<rack::PJ301MPort>(rack::Vec(c1 - dc, q), module, M::INPUT_L));
        addInput(
            rack::createInputCentered<rack::PJ301MPort>(rack::Vec(c1 + dc, q), module, M::INPUT_R));
        addOutput(rack::createOutputCentered<rack::PJ301MPort>(rack::Vec(c2 - dc, q), module,
                                                               M::OUTPUT_L));
        addOutput(rack::createOutputCentered<rack::PJ301MPort>(rack::Vec(c2 + dc, q), module,
                                                               M::OUTPUT_R));
    }

    void appendContextMenu(rack::Menu *menu) override
    {
        menu->addChild(new rack::MenuSeparator);
        menu->addChild(rack::createMenuItem("Light Skin",
                                            CHECKMARK(skinManager.skin == AWSkin::LIGHT),
                                            []() { skinManager.changeTo(AWSkin::LIGHT, true); }));
        menu->addChild(rack::createMenuItem("Dark Skin",
                                            CHECKMARK(skinManager.skin == AWSkin::DARK),
                                            []() { skinManager.changeTo(AWSkin::DARK, true); }));

        auto awm = dynamic_cast<AW2RModule *>(module);
        if (awm)
        {
            menu->addChild(new rack::MenuSeparator);
            menu->addChild(rack::createMenuItem("Monophonic", CHECKMARK(!awm->polyphonic),
                                                [awm]() { awm->stagePolyReset(false); }));
            menu->addChild(rack::createMenuItem("Polyphonic", CHECKMARK(awm->polyphonic),
                                                [awm]() { awm->stagePolyReset(true); }));
        }
    }

    void drawBG(NVGcontext *vg)
    {
        auto cutPoint{58};

        // Main Gradient Background
        nvgBeginPath(vg);
        if (skinManager.skin == AWSkin::DARK)
        {
            nvgFillPaint(vg, nvgLinearGradient(vg, 0, 50, 0, box.size.y - cutPoint,
                                               nvgRGB(50, 50, 60), nvgRGB(70, 70, 75)));
        }
        else
        {
            nvgFillPaint(vg, nvgLinearGradient(vg, 0, 50, 0, box.size.y - cutPoint,
                                               nvgRGB(225, 225, 230), nvgRGB(235, 235, 245)));
        }
        nvgRect(vg, 0, 0, box.size.x, box.size.y - cutPoint);
        nvgFill(vg);
        nvgStroke(vg);

        // Draw the bottom region
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(160, 160, 170));
        nvgStrokeColor(vg, nvgRGB(0, 0, 0));
        nvgStrokeWidth(vg, 0.5);
        nvgRect(vg, 0, box.size.y - cutPoint, box.size.x, cutPoint);
        nvgFill(vg);
        nvgStroke(vg);

        // Input region
        auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(140, 140, 150));
        nvgFillColor(vg, nvgRGB(190, 190, 200));
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, 4, box.size.y - cutPoint + 3, box.size.x * 0.5 - 8, 37, 2);
        nvgFill(vg);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(40, 40, 50));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 10);
        nvgText(vg, box.size.x * 0.25, box.size.y - cutPoint + 38, "L  IN  R", nullptr);

        // Output region
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(40, 40, 50));
        nvgFillColor(vg, nvgRGB(60, 60, 70));
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, box.size.x * 0.5 + 4, box.size.y - cutPoint + 3, box.size.x * 0.5 - 8,
                       37, 2);
        nvgFill(vg);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(190, 190, 200));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 10);
        nvgText(vg, box.size.x * 0.75, box.size.y - cutPoint + 38, "L  OUT  R", nullptr);

        // Brand
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(0, 0, 0));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 14);
        nvgText(vg, box.size.x * 0.5, box.size.y - 2, "Airwindows", nullptr);

        if (clipperSvg)
        {
            nvgSave(vg);
            float t[6];
            nvgTranslate(vg, 0, 269);
            nvgScale(vg, 0.14, 0.14);
            if (skinManager.skin == AWSkin::DARK)
                nvgAlpha(vg, 0.73);
            else
                nvgAlpha(vg, 0.23);
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
    AWSkin::Skin lastSkin{AWSkin::DARK};
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

        if (lastSkin != skinManager.skin)
        {
            lastSkin = skinManager.skin;
            if (bg)
                bg->dirty = true;
        }

        rack::ModuleWidget::step();
    }

    void resetAirwinDisplay()
    {
        auto awm = dynamic_cast<AW2RModule *>(module);

        if (!awm)
            return; // should never happen but hey

        int np = awm->nParams;
        for (int i = 0; i < np; ++i)
        {
            parLabels[i]->setVisible(true);
            char txt[256];
            awm->airwin_display->getParameterName(i, txt);
            parLabels[i]->label = txt;
            parKnobs[i]->setVisible(true);
            attenKnobs[i]->setVisible(true);
            cvPorts[i]->setVisible(true);
        }
        for (int i = np; i < M::maxParams; ++i)
        {
            parLabels[i]->setVisible(false);
            parKnobs[i]->setVisible(false);
            attenKnobs[i]->setVisible(false);
            cvPorts[i]->setVisible(true);
        }
    }
};

rack::Model *airwin2RackModel = rack::createModel<AW2RModule, AW2RModuleWidget>("Airwin2Rack");