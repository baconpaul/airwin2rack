#include "Airwin2Rack.hpp"
#include <iostream>

template<typename T, int n>
struct AW2RModule : virtual rack::Module
{
    typedef T underlyer;
    static constexpr int nParams{n};

    enum ParamIds
    {
        PARAM_0,
        NUM_PARAMS = PARAM_0 + n
    };

    enum InputIds
    {
        INPUT_L,
        INPUT_R,
        CV_0,
        NUM_INPUTS = CV_0 + n
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

    AW2RModule()
    {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    }


    void process(const ProcessArgs &args) override
    {
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

template <typename M>
struct AW2RModuleWidget : rack::ModuleWidget {
    AW2RModuleWidget(M *m) {
       setModule(m);
       box.size = rack::Vec(SCREW_WIDTH * 9, RACK_HEIGHT);

       auto bg = new AWBG;
       bg->box.pos = rack::Vec(0.0);
       bg->box.size = box.size;
       addChild(bg);

       for (int i=0; i<M::nParams; ++i)
       {
          char txt[256];
          M::underlyer::getParameterName(i, txt);
          std::cout << "Parameter " << i << " -> " << txt << std::endl;
       }
  }
};

#include "autogen_airwin/Galactic.h"

typedef AW2RModule<airwin2rack::Galactic::Galactic,airwin2rack::Galactic::kNumParameters> Galactic_model;
int t = addAirwin(rack::createModel<Galactic_model, AW2RModuleWidget<Galactic_model>>("Galactic"));

#if 0
addAirwin(x);

#endif