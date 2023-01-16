#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <atomic>

// @TODO: Save the FX chosen obviously but unstream that without value reset
// @TODO: CV control and attenuverters
// @TODO: A better knob and spacing
// @TODO: Output labels
// @TODO: Push to Github and add Actions with a build
struct AW2RModule : virtual rack::Module
{
    static constexpr int maxParams{14};

    std::unique_ptr<Airwin2RackBase> airwin{};
    std::atomic<int32_t> forceSelect{-1}, resetCount{0};
    std::string selectedFX{};

    struct awReg
    {
        std::string name;
        std::string category;
        int nParams;
        std::function<std::unique_ptr<Airwin2RackBase>()> generator;
    };
    static std::vector<awReg> registry;
    static std::set<std::string> categories;
    static int registerAirwindow(const awReg &r)
    {
        registry.emplace_back(r);
        return registry.size();
    }
    static int completeRegistry()
    {
        for (const auto &r : registry)
            categories.insert(r.category);
        return 0;
    }

    enum ParamIds
    {
        PARAM_0,
        NUM_PARAMS = PARAM_0 + maxParams
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
        assert(!registry.empty());
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
            configParam(PARAM_0 + i, 0, 1, 0, "Param " + std::to_string(i));

        resetAirwindowTo(0);
    }

    void resetAirwindowTo(int registryIdx)
    {
        selectedFX = registry[registryIdx].name;
        airwin = registry[registryIdx].generator();
        nParams = registry[registryIdx].nParams;

        for (int i=0; i<nParams; ++i)
        {
            char txt[256];
            airwin->getParameterName(i, txt);
            paramQuantities[i]->name = txt;
            paramQuantities[i]->defaultValue = airwin->getParameter(i);
            paramQuantities[i]->setValue(paramQuantities[i]->defaultValue);
        }

        resetCount++;
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
                airwin->setParameter(i, params[PARAM_0 + i].getValue());
            airwin->processReplacing(in, out, block);
            outPos = 0;
            inPos = 0;
        }

        outputs[OUTPUT_L].setVoltage(out[0][outPos] * 5);
        outputs[OUTPUT_R].setVoltage(out[1][outPos] * 5);
        outPos++;
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
        nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, px);

        nvgText(vg, 0, 0, label.c_str(), nullptr);
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
        nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 14);
        nvgText(vg, box.size.x * 0.5, 4, nm.c_str(), nullptr);
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
        for (const auto &cat : AW2RModule::categories)
        {
            m->addChild(rack::createSubmenuItem(
                cat, "", [this, cat](auto *m) { createCategoryMenu(m, cat); }));
        }
    }
    void createCategoryMenu(rack::Menu *m, const std::string &cat)
    {
        std::map<std::string, int> contents;
        int idx = 0;
        for (const auto &item : AW2RModule::registry)
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
    std::array<rack::ParamWidget *, M::maxParams> parKnobs;
    AW2RModuleWidget(M *m)
    {
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 9, RACK_HEIGHT);

        auto bg = new AWBG;
        bg->box.pos = rack::Vec(0.0);
        bg->box.size = box.size;
        addChild(bg);

        char enm[256];
        if (m)
            m->airwin->getEffectName(enm);
        else
            strncpy(enm, "Effect", 256);
        auto tlab = new AWSelector;
        auto s = box;
        s.size.y = 40;
        s = s.shrink(rack::Vec(5,5));
        tlab->box = s;
        tlab->module = m;
        addChild(tlab);

        auto pPos = 45, dPP = 22;

        for (int i = 0; i < M::maxParams; ++i)
        {
            auto tlab = new AWLabel;
            tlab->px = 11;
            tlab->box.pos.x = 5;
            tlab->box.pos.y = pPos;
            tlab->label = "Param " + std::to_string(i);
            parLabels[i] = tlab;
            addChild(tlab);

            parKnobs[i] = rack::createParamCentered<rack::RoundSmallBlackKnob>(
                rack::Vec(box.size.x - 40, pPos + dPP * 0.5), module, M::PARAM_0 + i);
            addParam(parKnobs[i]);
            pPos += 35;
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
        }
        for (int i=np; i<M::maxParams; ++i)
        {
            parLabels[i]->setVisible(false);
            parKnobs[i]->setVisible(false);
        }
    }
};

std::vector<AW2RModule::awReg> AW2RModule::registry;
std::set<std::string> AW2RModule::categories;

#include "ModuleAdd.h"

rack::Model *airwin2RackModel = rack::createModel<AW2RModule, AW2RModuleWidget>("Airwin2Rack");