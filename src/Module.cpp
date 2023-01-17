#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <atomic>

#include "AirwinRegistry.h"

// @TODO: Param Smoothing
// @TODO: A Cleaner UI of course
// @TODO: Update README
// @TODO: A dark and light mode
// @TODO: Output labels and Areas on the grid
// @TODO: Use Chris' logo
// @TODO: Cleanup plugin.json
struct AW2RModule : virtual rack::Module
{
    static constexpr int maxParams{14};

    std::unique_ptr<Airwin2RackBase> airwin{}, airwin_display{};
    std::atomic<int32_t> forceSelect{-1}, resetCount{0};
    std::string selectedFX{};

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
        in[0][inPos] = inputs[INPUT_L].getVoltageSum() * 0.2;
        in[1][inPos] = inputs[INPUT_R].getVoltageSum() * 0.2;
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
        nvgFillColor(vg, nvgRGB(255,0,0));
        nvgFill(vg);

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
        nvgStrokeWidth(vg, 2);
        nvgStrokeColor(vg, nvgRGB(255,255,255));
        nvgLineCap(vg, NVG_ROUND);
        nvgStroke(vg);

    }
};

struct AWBG : rack::Widget
{
    void draw(const DrawArgs &args) override
    {
        // @TODO: Double Buffer
        auto vg = args.vg;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(0, 30, 0));
        nvgStrokeColor(vg, nvgRGB(100, 100, 100));
        nvgStrokeWidth(vg, 0.5);
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
        nvgFill(vg);
        nvgStroke(vg);
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

    }
};

struct AWSelector : rack::Widget
{
    AW2RModule *module;
    std::string fontPath;
    AWSelector() { fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf"); }

    void draw(const DrawArgs &args) override
    {
        auto vg = args.vg;
        // auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(0, 0, 0));
        nvgStrokeColor(vg, nvgRGB(100,100,200));
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
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
};

struct AW2RModuleWidget : rack::ModuleWidget
{
    typedef AW2RModule M;

    std::array<AWLabel *, M::maxParams> parLabels;
    std::array<rack::ParamWidget *, M::maxParams> parKnobs, attenKnobs;
    std::array<rack::PortWidget *, M::maxParams> cvPorts;
    AW2RModuleWidget(M *m)
    {
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 11, RACK_HEIGHT);

        auto bg = new AWBG;
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
        auto q = RACK_HEIGHT - 40;
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