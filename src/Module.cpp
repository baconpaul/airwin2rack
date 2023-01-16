#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>

struct AW2RModule : virtual rack::Module
{
    static constexpr int maxParams{14};

    std::unique_ptr<Airwin2RackBase> airwin;

    static std::vector<std::pair<std::string, std::function<Airwin2RackBase *()>>> registry;
    static int registerAirwindow(const std::string &name,
                                  std::function<Airwin2RackBase *()> f)
    {
        registry.emplace_back(name, std::move(f));
        return registry.size();
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
        airwin.reset(registry[0].second());
        nParams = 5; // gotta get this from the enum
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        memset(indat, 0, 2 * block * sizeof(float));
        memset(outdat, 0, 2 * block * sizeof(float));
        in[0] = &(indat[0]);
        in[1] = &(indat[block]);
        out[0] = &(outdat[0]);
        out[1] = &(outdat[block]);

        configBypass(INPUT_L, OUTPUT_L);
        configBypass(INPUT_R, OUTPUT_R);

        for (int i=0; i<nParams; ++i)
            configParam(PARAM_0 + i, 0, 1, airwin->getParameter(i));
    }

    static constexpr int block{4};

    float *in[2], *out[2];
    float indat[ 2 * block], outdat[2 * block];
    int inPos{0}, outPos{0};

    void process(const ProcessArgs &args) override
    {
        in[0][inPos] = inputs[INPUT_L].getVoltageSum() * 0.2;
        in[1][inPos] = inputs[INPUT_R].getVoltageSum() * 0.2;
        inPos ++;
        if (inPos == block)
        {
            for (int i=0; i<nParams; ++i)
                airwin->setParameter(i, params[PARAM_0 + i].getValue());
            airwin->processReplacing(in, out, block);
            outPos = 0;
            inPos = 0;
        }

        outputs[OUTPUT_L].setVoltage(out[0][outPos] * 5);
        outputs[OUTPUT_R].setVoltage(out[1][outPos] * 5);
        outPos ++;
    }
};

struct AWBG : rack::Widget
{
   void draw(const DrawArgs &args) override {
      auto vg = args.vg;
      nvgBeginPath(vg);
      nvgFillColor(vg, nvgRGB(0,30,0));
      nvgStrokeColor(vg, nvgRGB(100,100,100));
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
    AWLabel()
    {
        fontPath = rack::asset::plugin(pluginInstance, "res/FiraMono-Regular.ttf");
    }
    void draw(const DrawArgs &args) override {
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

struct AW2RModuleWidget : rack::ModuleWidget {
    typedef AW2RModule M;
    AW2RModuleWidget(M *m) {
       setModule(m);
       box.size = rack::Vec(SCREW_WIDTH * 9, RACK_HEIGHT);

       auto bg = new AWBG;
       bg->box.pos = rack::Vec(0.0);
       bg->box.size = box.size;
       addChild(bg);

       char enm[256];
       //M::underlyer::getEffectName(enm);
       strncpy(enm, "Foo", 256);
       auto tlab = new AWLabel;
       tlab->px = 14;
       tlab->box.pos.x = 2;
       tlab->box.pos.y = 2;
       tlab->box.size.y = 20;
       tlab->box.size.x = box.size.x - 4;
       tlab->label = enm;
       addChild(tlab);

       auto pPos = 20, dPP = 35;
/*
       for (int i=0; i<M::nParams; ++i)
       {
           char txt[256];
           M::underlyer::getParameterName(i, txt);

           auto tlab = new AWLabel;
           tlab->px = 11;
           tlab->box.pos.x = 2;
           tlab->box.pos.y = pPos;
           tlab->label = txt;
           addChild(tlab);

           addParam(rack::createParamCentered<rack::RoundSmallBlackKnob>(rack::Vec(box.size.x - 40,
                                                                                   pPos + dPP * 0.5),
                                                                         module,
                                                                         M::PARAM_0 + i));
           pPos += 35;

       }
       */

       auto q = RACK_HEIGHT - 80;
       auto c1 = box.size.x * 0.25;
       auto c2 = box.size.x * 0.75;
       addInput(rack::createInputCentered<rack::PJ301MPort>(rack::Vec(c1, q), module, M::INPUT_L));
       addInput(rack::createInputCentered<rack::PJ301MPort>(rack::Vec(c2, q), module, M::INPUT_R));
       q += 40;
       addOutput(rack::createOutputCentered<rack::PJ301MPort>(rack::Vec(c1, q), module, M::OUTPUT_L));
       addOutput(rack::createOutputCentered<rack::PJ301MPort>(rack::Vec(c2, q), module, M::OUTPUT_R));

  }
};

std::vector<std::pair<std::string, std::function<Airwin2RackBase *()>>> AW2RModule::registry;

#include "ModuleAdd.h"

rack::Model *airwin2RackModel = rack::createModel<AW2RModule, AW2RModuleWidget>("Airwin2Rack");