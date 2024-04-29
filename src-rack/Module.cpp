/*
 * Airwin2Rack - an adaptation of the airwindows effect suite for VCVRack
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <atomic>

#include "AirwinRegistry.h"

#include "sst/rackhelpers/json.h"
#include "sst/rackhelpers/ui.h"
#include "sst/rackhelpers/neighbor_connectable.h"
#include "sst/rackhelpers/module_connector.h"

// @TODO: Cloud perlin ala Steve

#define MAX_POLY 16

struct AW2RModule : virtual rack::Module, sst::rackhelpers::module_connector::NeighborConnectable_V1
{
    static constexpr int maxParams{10};

    std::unique_ptr<Airwin2RackBase> airwin{}, airwin_display{};
    std::array<std::unique_ptr<Airwin2RackBase>, MAX_POLY> poly_airwin;
    std::atomic<int32_t> forceSelect{-1}, resetCount{0}, selectedIdx{-1};
    std::atomic<bool> panicReset;
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
                    auto ls = rack::string::trim(std::string(lab));
                    return std::string(txt) + (ls.empty() ? "" : " " + ls);
                }
            }
            return ParamQuantity::getDisplayValueString();
        }

        void setDisplayValueString(std::string s) override
        {
            float pv = getValue();
            auto awm = dynamic_cast<AW2RModule *>(module);
            if (awm && awm->airwin_display)
            {
                int idx = paramId - PARAM_0;
                if (idx < awm->nParams)
                {
                    float rv = pv;
                    auto res = awm->airwin_display->parameterTextToValue(idx, s.c_str(), rv);
                    if (res)
                    {
                        setValue(std::clamp(rv, 0.f, 1.f));
                    }
                    else
                    {
                        setValue(pv);
                    }
                }
            }
        }
    };

    enum ParamIds
    {
        PARAM_0,
        MAX_PARAMS_USED_TO_BE_11_DONT_BREAK_FOLKS = PARAM_0 + maxParams,
        ATTENUVERTER_0,
        MAX_PARAMS_USED_TO_BE_11_DONT_BREAK_ATTENS = ATTENUVERTER_0 + maxParams,
        IN_LEVEL,
        OUT_LEVEL,
        NUM_PARAMS
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

    enum PolyphonyMode
    {
        MONOPHONIC,
        POLYPHONIC,               // the i/o is are LLL... RRR...
        POLYPHONIC_MIXMASTER,     // the i/o is LRLRLR ...  LRLRLR
        MIXMASTER_TO_MONOPHONIC,  // in is LRLR...LRLRL summed to mono
        MIXMASTER_TO_STEREO_POLY, // in is LRLRLR LRLRLRL out is LLLL RRRR
        STEREO_POLY_TO_MIXMASTER  // in is LLL RRR out is LRLR LRLR
    };

    std::atomic<PolyphonyMode> polyphonyMode{MONOPHONIC};
    std::atomic<bool> lockedType{false}, randomizeFX{false};
    AW2RModule()
    {
        Airwin2RackBase::defaultSampleRate = APP->engine->getSampleRate();

        assert(!AirwinRegistry::registry.empty());
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

        configInput(INPUT_L, "Left / Mono Input");
        configInput(INPUT_R, "Right Input");
        configOutput(OUTPUT_L, "Left Output");
        configOutput(OUTPUT_R, "Right Output");
        configBypass(INPUT_L, OUTPUT_L);
        configBypass(INPUT_R, OUTPUT_R);

        configParam(MAX_PARAMS_USED_TO_BE_11_DONT_BREAK_FOLKS, 0, 1, 0, "Unused");
        configParam(MAX_PARAMS_USED_TO_BE_11_DONT_BREAK_ATTENS, 0, 1, 0, "Unused");

        configParam(IN_LEVEL, 0, 1, 1, "Input Gain", "%", 0, 100);
        configParam(OUT_LEVEL, 0, 1, 1, "Output Gain", "%", 0, 100);

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

    std::optional<std::vector<labeledStereoPort_t>> getPrimaryInputs() override
    {
        return {{std::make_pair("Input", std::make_pair(INPUT_L, INPUT_R))}};
    }

    std::optional<std::vector<labeledStereoPort_t>> getPrimaryOutputs() override
    {
        return {{std::make_pair("Output", std::make_pair(OUTPUT_L, OUTPUT_R))}};
    }

    void onReset(const ResetEvent &e) override { resetAirwinByName("Galactic", true); }

    void onRandomize(const RandomizeEvent &e) override
    {
        if (randomizeFX && !lockedType)
        {
            auto ri = rand() % (int)(AirwinRegistry::registry.size());
            resetAirwindowTo(ri, true);
        }
        Module::onRandomize(e);
    }

    void resetAirwindowTo(int registryIdx, bool resetValues = true)
    {
        selectedIdx = registryIdx;
        selectedFX = AirwinRegistry::registry[registryIdx].name;
        selectedCat = AirwinRegistry::registry[registryIdx].category;
        selectedWhat = AirwinRegistry::registry[registryIdx].whatText;

        Airwin2RackBase::defaultSampleRate = APP->engine->getSampleRate();

        airwin_display = AirwinRegistry::registry[registryIdx].generator();
        airwin_display->setSampleRate(APP->engine->getSampleRate());

        if (polyphonyMode != MONOPHONIC && polyphonyMode != MIXMASTER_TO_MONOPHONIC)
        {
            airwin.reset(nullptr);
            for (int i = 0; i < MAX_POLY; ++i)
            {
                poly_airwin[i] = AirwinRegistry::registry[registryIdx].generator();
                poly_airwin[i]->setSampleRate(APP->engine->getSampleRate());
            }
        }
        else
        {
            airwin = AirwinRegistry::registry[registryIdx].generator();
            airwin->setSampleRate(APP->engine->getSampleRate());

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

        monoIO.reset();
        for (int c = 0; c < MAX_POLY; ++c)
            polyIO[c].reset();

        resetCount++;

        updateSampleRates();
    }

    void updateSampleRates()
    {
        auto sr = APP->engine->getSampleRate();
        Airwin2RackBase::defaultSampleRate = sr;
        airwin_display->setSampleRate(sr);
        if (airwin)
            airwin->setSampleRate(sr);
        for (auto &p : poly_airwin)
            if (p)
                p->setSampleRate(sr);
    }

    void onSampleRateChange(const SampleRateChangeEvent &e) override { updateSampleRates(); }

    json_t *dataToJson() override
    {
        auto res = json_object();
        json_object_set_new(res, "airwindowSelectedFX", json_string(selectedFX.c_str()));

        // Stream next poly here in case you change it without an audio thread
        json_object_set_new(res, "polyphonyMode", json_integer(nextPoly));
        json_object_set_new(res, "lockedType", json_boolean(lockedType));
        json_object_set_new(res, "randomizeFX", json_boolean(randomizeFX));
        json_object_set_new(res, "blockSize", json_integer(blockSize));
        return res;
    }

    void dataFromJson(json_t *rootJ) override
    {
        namespace jh = sst::rackhelpers::json;

        resetAirwinByName(
            jh::jsonSafeGet<std::string>(rootJ, "airwindowSelectedFX").value_or("Galactic"), false);
        lockedType = jh::jsonSafeGet<bool>(rootJ, "lockedType").value_or(false);

        bool oldPoly = jh::jsonSafeGet<bool>(rootJ, "polyphonic").value_or(false);
        int newPoly = jh::jsonSafeGet<int>(rootJ, "polyphonyMode").value_or(-1);

        if (newPoly == -1)
            resetPolyphony(oldPoly ? POLYPHONIC : MONOPHONIC);
        else
            resetPolyphony((PolyphonyMode)newPoly);

        randomizeFX = jh::jsonSafeGet<bool>(rootJ, "randomizeFX").value_or(false);
        forceBlockSize = jh::jsonSafeGet<int>(rootJ, "blockSize").value_or(4);
    }

    PolyphonyMode nextPoly{polyphonyMode};
    void stagePolyReset(PolyphonyMode b) { nextPoly = b; }

    void resetPolyphony(PolyphonyMode pMode)
    {
        if (pMode == polyphonyMode)
            return;

        polyphonyMode = pMode;
        nextPoly = pMode;

        resetAirwinByName(selectedFX, false);
        if (polyphonyMode != MONOPHONIC && polyphonyMode != MIXMASTER_TO_MONOPHONIC)
        {
            for (int i = 0; i < MAX_POLY; ++i)
            {
                polyIO[i].reset();
            }
        }
        else
        {
            monoIO.reset();
            outputs[OUTPUT_L].setChannels(1);
            outputs[OUTPUT_R].setChannels(1);
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

    static constexpr int maxBlockSize{64};
    int blockSize{4};
    std::atomic<int> forceBlockSize{-1};

    struct IOHolder
    {
        IOHolder() { reset(); }
        void reset()
        {
            memset(indat, 0, 2 * maxBlockSize * sizeof(float));
            memset(outdat, 0, 2 * maxBlockSize * sizeof(float));
            in[0] = &(indat[0]);
            in[1] = &(indat[maxBlockSize]);
            out[0] = &(outdat[0]);
            out[1] = &(outdat[maxBlockSize]);
            inPos = 0;
            outPos = 0;
        }

        float *in[2], *out[2];
        float indat[2 * maxBlockSize], outdat[2 * maxBlockSize];
        int inPos{0}, outPos{0};
    } monoIO, polyIO[MAX_POLY];

    void process(const ProcessArgs &args) override
    {
        if (nextPoly != polyphonyMode)
        {
            resetPolyphony(nextPoly);
        }
        if (forceSelect != -1) // a UI action doesn't warrant the compare_exchange rigamarole
        {
            resetAirwindowTo(forceSelect);
            forceSelect = -1;
        }
        if (panicReset && selectedIdx >= 0)
        {
            resetAirwindowTo(selectedIdx, false);
            panicReset = false;
        }
        if (forceBlockSize != -1)
        {
            blockSize = forceBlockSize;
            monoIO.reset();
            for (int i = 0; i < MAX_POLY; ++i)
                polyIO[i].reset();
            forceBlockSize = -1;

            resetAirwinByName(selectedFX, false);
        }

        switch (polyphonyMode)
        {
        case MONOPHONIC:
            processMono(args, false);
            break;
        case MIXMASTER_TO_MONOPHONIC:
            processMono(args, true);
            break;
        case POLYPHONIC:
            processPoly(args, false, false);
            break;
        case POLYPHONIC_MIXMASTER:
            processPoly(args, true, true);
            break;
        case MIXMASTER_TO_STEREO_POLY:
            processPoly(args, true, false);
            break;
        case STEREO_POLY_TO_MIXMASTER:
            processPoly(args, false, true);
            break;
        }
    }
    void processMono(const ProcessArgs &args, bool inputIsMixmaster)
    {
        auto rc = inputs[INPUT_R].isConnected() ? INPUT_R : INPUT_L;

        auto ig = params[IN_LEVEL].getValue();
        auto og = params[OUT_LEVEL].getValue();

        if (inputIsMixmaster)
        {
            monoIO.in[0][monoIO.inPos] = 0;
            monoIO.in[1][monoIO.inPos] = 0;
            for (int c = 0; c < inputs[INPUT_L].getChannels(); c += 2)
            {
                monoIO.in[0][monoIO.inPos] += inputs[INPUT_L].getVoltage(c) * 0.2 * ig;
                monoIO.in[1][monoIO.inPos] += inputs[INPUT_L].getVoltage(c + 1) * 0.2 * ig;
            }
            for (int c = 0; c < inputs[INPUT_R].getChannels(); c += 2)
            {
                monoIO.in[0][monoIO.inPos] += inputs[INPUT_R].getVoltage(c) * 0.2 * ig;
                monoIO.in[1][monoIO.inPos] += inputs[INPUT_R].getVoltage(c + 1) * 0.2 * ig;
            }
        }
        else
        {
            monoIO.in[0][monoIO.inPos] = inputs[INPUT_L].getVoltageSum() * 0.2 * ig;
            monoIO.in[1][monoIO.inPos] = inputs[rc].getVoltageSum() * 0.2 * ig;
        }
        monoIO.inPos++;
        if (monoIO.inPos >= blockSize)
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
            airwin->processReplacing(monoIO.in, monoIO.out, blockSize);
            monoIO.outPos = 0;
            monoIO.inPos = 0;
        }

        outputs[OUTPUT_L].setVoltage(monoIO.out[0][monoIO.outPos] * 5 * og);
        outputs[OUTPUT_R].setVoltage(monoIO.out[1][monoIO.outPos] * 5 * og);
        monoIO.outPos++;
    }

    void processPoly(const ProcessArgs &args, bool inputIsMixmaster, bool outputIsMixmaster)
    {
        int chanCt = std::max({1, inputs[INPUT_R].getChannels(), inputs[INPUT_L].getChannels()});
        outputs[OUTPUT_L].setChannels(chanCt);
        outputs[OUTPUT_R].setChannels(chanCt);

        auto ig = params[IN_LEVEL].getValue();
        auto og = params[OUT_LEVEL].getValue();

        auto rc = inputs[INPUT_R].isConnected() ? INPUT_R : INPUT_L;

        float sv[maxParams];
        bool isMod[maxParams];
        for (int i = 0; i < nParams; ++i)
        {
            sv[i] = paramQuantities[PARAM_0 + i]->getSmoothValue();
            isMod[i] = inputs[CV_0 + i].isConnected();
        }

        for (int c = 0; c < chanCt; ++c)
        {
            if (inputIsMixmaster)
            {
                if (c < 8)
                {
                    polyIO[c].in[0][polyIO[c].inPos] = inputs[INPUT_L].getVoltage(c * 2) * 0.2 * ig;
                    polyIO[c].in[1][polyIO[c].inPos] =
                        inputs[INPUT_L].getVoltage(c * 2 + 1) * 0.2 * ig;
                }
                else
                {
                    polyIO[c].in[0][polyIO[c].inPos] =
                        inputs[INPUT_R].getVoltage((c - 8) * 2) * 0.2 * ig;
                    polyIO[c].in[1][polyIO[c].inPos] =
                        inputs[INPUT_R].getVoltage((c - 8) * 2 + 1) * 0.2 * ig;
                }
            }
            else
            {
                polyIO[c].in[0][polyIO[c].inPos] = inputs[INPUT_L].getVoltage(c) * 0.2 * ig;
                polyIO[c].in[1][polyIO[c].inPos] = inputs[rc].getVoltage(c) * 0.2 * ig;
            }
            polyIO[c].inPos++;
            if (polyIO[c].inPos >= blockSize)
            {
                for (int i = 0; i < nParams; ++i)
                {
                    auto pv = sv[i];
                    if (isMod[i])
                    {
                        auto v = inputs[CV_0 + i].getVoltage(
                                     inputs[CV_0 + i].getChannels() == 1 ? 0 : c) *
                                 0.2 * params[ATTENUVERTER_0 + i].getValue();
                        pv = std::clamp(pv + v, 0., 1.);
                    }
                    poly_airwin[c]->setParameter(i, pv);
                }
                poly_airwin[c]->processReplacing(polyIO[c].in, polyIO[c].out, blockSize);
                polyIO[c].outPos = 0;
                polyIO[c].inPos = 0;
            }

            if (outputIsMixmaster)
            {
                if (c < 8)
                {
                    outputs[OUTPUT_L].setVoltage(polyIO[c].out[0][polyIO[c].outPos] * 5 * og,
                                                 c * 2);
                    outputs[OUTPUT_L].setVoltage(polyIO[c].out[1][polyIO[c].outPos] * 5 * og,
                                                 c * 2 + 1);
                }
                else
                {
                    outputs[OUTPUT_R].setVoltage(polyIO[c].out[0][polyIO[c].outPos] * 5 * og,
                                                 (c - 8) * 2);
                    outputs[OUTPUT_R].setVoltage(polyIO[c].out[1][polyIO[c].outPos] * 5 * og,
                                                 (c - 8) * 2 + 1);
                }
            }
            else
            {
                outputs[OUTPUT_L].setVoltage(polyIO[c].out[0][polyIO[c].outPos] * 5 * og, c);
                outputs[OUTPUT_R].setVoltage(polyIO[c].out[1][polyIO[c].outPos] * 5 * og, c);
            }
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

    enum MenuOrdering
    {
        ALPHA,
        CHRIS
    } menuOrdering{ALPHA};

    std::string collection{"All"};

    void changeTo(Skin s, bool write)
    {
        skin = s;

        if (write)
        {
            writeConfig();
        }
    }

    void changeOrderingTo(MenuOrdering o)
    {
        menuOrdering = o;
        writeConfig();
    }

    void writeConfig()
    {
        std::string defaultsFile = rack::asset::user("Airwin2Rack.json");

        json_t *rootJ = json_object();
        json_object_set_new(rootJ, "defaultSkin", json_integer(skin));
        json_object_set_new(rootJ, "defaultMenuOrdering", json_integer(menuOrdering));
        json_object_set_new(rootJ, "collection", json_string(collection.c_str()));

        FILE *f = std::fopen(defaultsFile.c_str(), "w");
        if (f)
        {
            json_dumpf(rootJ, f, JSON_INDENT(2));
            std::fclose(f);
        }
        json_decref(rootJ);
    }

    void readConfig()
    {
        std::string defaultsFile = rack::asset::user("Airwin2Rack.json");
        json_error_t error;
        json_t *rootJ{nullptr};
        auto *fptr = std::fopen(defaultsFile.c_str(), "r");
        if (fptr)
        {
            rootJ = json_loadf(fptr, 0, &error);
            DEFER({ std::fclose(fptr); });
        }
        if (!rootJ)
        {
            changeTo(DARK, false);
            menuOrdering = ALPHA;
        }
        else
        {
            changeTo(
                (Skin)sst::rackhelpers::json::jsonSafeGet<int>(rootJ, "defaultSkin").value_or(DARK),
                false);
            menuOrdering =
                (MenuOrdering)sst::rackhelpers::json::jsonSafeGet<int>(rootJ, "defaultMenuOrdering")
                    .value_or(ALPHA);
            collection = sst::rackhelpers::json::jsonSafeGet<std::string>(rootJ, "collection").value_or("All");
        }
    }

    std::string fontPath, fontPathMedium;
    bool initialized{false};
    AWSkin() {}

    void intialize()
    {
        if (initialized)
            return;
        initialized = true;

        fontPath = rack::asset::plugin(pluginInstance, "res/PlusJakartaSans-SemiBold.ttf");
        fontPathMedium = rack::asset::plugin(pluginInstance, "res/PlusJakartaSans-Medium.ttf");
        readConfig();
    }

    template <typename T> T dl(const T &dark, const T &light)
    {
        if (skin == DARK)
            return dark;
        else
            return light;
    }

#define COL(n, d, l)                                                                               \
    NVGcolor n() { return dl(d, l); }

    COL(knobCenter, nvgRGB(110, 110, 120), nvgRGB(185, 185, 220));
    COL(knobEdge, nvgRGB(110, 110, 130), nvgRGB(190, 190, 225));
    COL(knobStroke, nvgRGB(20, 20, 20), nvgRGB(50, 50, 60));
    COL(knobValueFill, nvgRGB(240, 240, 240), nvgRGB(20, 20, 20));
    COL(knobValueStroke, nvgRGB(20, 20, 20), nvgRGB(20, 20, 20));

    COL(labeLText, nvgRGB(220, 220, 220), nvgRGB(20, 20, 20));
    COL(labelRule, nvgRGB(110, 110, 120), nvgRGB(150, 150, 160));

    COL(deactivatedJogStroke, nvgRGB(60, 60, 60), nvgRGB(60, 60, 60));
    COL(deactivatedJogFill, nvgRGB(40, 40, 40), nvgRGB(40, 40, 40));
    COL(jogFill, nvgRGB(190, 190, 190), nvgRGB(190, 190, 190));
    COL(jogFillHover, nvgRGB(240, 240, 100), nvgRGB(240, 240, 100));
    COL(jogStroke, nvgRGB(220, 220, 220), nvgRGB(220, 220, 220));

    COL(helpOpen, nvgRGB(220, 220, 220), nvgRGB(220, 220, 220));
    COL(helpClose, nvgRGB(120, 120, 120), nvgRGB(120, 120, 120));

    COL(selectorFill, nvgRGB(20, 20, 30), nvgRGB(20, 20, 30));
    COL(selectorOutline, nvgRGB(0, 0, 0), nvgRGB(0, 0, 0));
    COL(selectorOutlineHighlight, nvgRGB(140, 140, 160), nvgRGB(140, 140, 160));
    COL(selectorEffect, nvgRGB(240, 240, 240), nvgRGB(240, 240, 240));
    COL(selectorCategory, nvgRGB(210, 210, 210), nvgRGB(210, 210, 210));
    COL(selectorPoly, nvgRGB(140, 140, 140), nvgRGB(140, 140, 140));

    COL(helpBorder, nvgRGB(180, 180, 180), nvgRGB(180, 180, 180));
    COL(helpBG, nvgRGB(20, 20, 20), nvgRGB(20, 20, 20));
    COL(helpText, nvgRGB(220, 220, 225), nvgRGB(220, 220, 225));

    COL(panelGradientStart, nvgRGB(50, 50, 60), nvgRGB(225, 225, 230));
    COL(panelGradientEnd, nvgRGB(70, 70, 75), nvgRGB(235, 235, 245));

    COL(panelBottomRegion, nvgRGB(160, 160, 170), nvgRGB(160, 160, 170));
    COL(panelBottomStroke, nvgRGB(0, 0, 0), nvgRGB(0, 0, 0));

    COL(panelInputFill, nvgRGB(190, 190, 200), nvgRGB(190, 190, 200));
    COL(panelInputBorder, nvgRGB(140, 140, 150), nvgRGB(140, 140, 150));
    COL(panelInputText, nvgRGB(40, 40, 50), nvgRGB(40, 40, 50));

    COL(panelOutputFill, nvgRGB(60, 60, 70), nvgRGB(60, 60, 70));
    COL(panelOutputBorder, nvgRGB(40, 40, 50), nvgRGB(40, 40, 50));
    COL(panelOutputText, nvgRGB(190, 190, 200), nvgRGB(190, 190, 200));

    COL(panelBrandText, nvgRGB(0, 0, 0), nvgRGB(0, 0, 0));

    float svgAlpha() { return dl(0.73, 0.23); }

    COL(moduleOutline, nvgRGB(100, 100, 100), nvgRGB(100, 100, 100));
};

AWSkin awSkin;

struct AWPort : public sst::rackhelpers::module_connector::PortConnectionMixin<rack::app::SvgPort>
{
    AWPort() { setPortActive(true); }

    bool active{true};
    void setPortActive(bool b)
    {
        active = b;
        if (awSkin.skin == AWSkin::DARK)
        {
            if (b)
            {
                setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/port_on.svg")));
            }
            else
            {
                setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/port_off.svg")));
            }
        }
        else
        {
            if (b)
            {
                setSvg(
                    rack::Svg::load(rack::asset::plugin(pluginInstance, "res/port_on_light.svg")));
            }
            else
            {
                setSvg(
                    rack::Svg::load(rack::asset::plugin(pluginInstance, "res/port_off_light.svg")));
            }
        }
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    void step() override
    {
        bool dirty{false};
        if (lastSkin != awSkin.skin)
            setPortActive(active);
        lastSkin = awSkin.skin;

        rack::Widget::step();
    }
};

void pushFXChange(AW2RModule *module, int newIndex)
{
    auto h = new rack::history::ModuleChange;
    h->name = "change preset to " + AirwinRegistry::registry[newIndex].name;
    h->moduleId = module->id;
    h->oldModuleJ = module->toJson();

    module->forceSelect = newIndex;
    // Wait for process to flush
    int ct{0}, ctMax{100000};
    while (module->forceSelect == -1 && ct < ctMax)
    {
        ct++;
    }
    if (ct == ctMax)
    {
    }
    h->newModuleJ = module->toJson();
    APP->history->push(h);
}

template <int px, bool bipolar = false> struct PixelKnob : rack::Knob
{
    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bdw{nullptr};
    bool stripMenuTypein{false};
    PixelKnob()
    {
        box.size = rack::Vec(px + 3, px + 3);
        float angleSpreadDegrees = 40.0;

        minAngle = -M_PI * (180 - angleSpreadDegrees) / 180;
        maxAngle = M_PI * (180 - angleSpreadDegrees) / 180;

        bdw = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(
            rack::Vec(0, 0), box.size, [this](auto vg) { drawKnob(vg); });
        addChild(bdw);
    }

    void drawKnob(NVGcontext *vg)
    {
        float radius = px * 0.48;
        nvgBeginPath(vg);
        nvgEllipse(vg, box.size.x * 0.5, box.size.y * 0.5, radius, radius);
        nvgFillPaint(vg,
                     nvgRadialGradient(vg, box.size.x * 0.5, box.size.y * 0.5, box.size.x * 0.1,
                                       box.size.x * 0.4, awSkin.knobCenter(), awSkin.knobEdge()));
        nvgStrokeColor(vg, awSkin.knobStroke());
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

        auto valueFill = awSkin.knobValueFill();

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
        nvgStrokeColor(vg, awSkin.knobValueStroke());
        nvgStrokeWidth(vg, 0.5);
        nvgStroke(vg);
        nvgFill(vg);
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    float lastVal{0.f};
    void step() override
    {
        bool dirty{false};
        if (lastSkin != awSkin.skin)
            dirty = true;
        lastSkin = awSkin.skin;

        auto pq = getParamQuantity();
        if (pq)
        {
            if (lastVal != pq->getValue())
                dirty = true;
            lastVal = pq->getValue();
        }

        if (bdw && dirty)
            bdw->dirty = dirty;

        rack::Widget::step();
    }

    void appendContextMenu(rack::Menu *menu) override
    {
        if (stripMenuTypein && menu->children.size() >= 2)
        {
            auto tgt = std::next(menu->children.begin());
            menu->removeChild(*tgt);
            delete *tgt;
        }
    }
};

struct AttenSlider : rack::Knob
{
    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bdw{nullptr};
    AttenSlider() {}

    static AttenSlider *create(const rack::Vec &pos, int w, AW2RModule *module, int paramId)
    {
        auto res = new AttenSlider();
        res->module = module;
        res->paramId = paramId;
        res->box.size = rack::Vec(w, 6);
        res->box.pos = pos;
        res->bdw = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(
            rack::Vec(0, 0), res->box.size, [res](auto vg) { res->drawSlider(vg); });
        res->addChild(res->bdw);
        return res;
    }

    void drawSlider(NVGcontext *vg)
    {
        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.knobCenter());
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 2);
        nvgFill(vg);

        float val{1.f};
        if (getParamQuantity())
            val = std::clamp(getParamQuantity()->getValue(), 0.f, 1.f);
        nvgSave(vg);
        nvgScissor(vg, 0, 0, box.size.x * val, box.size.y);
        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.knobValueFill());
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 2);
        nvgFill(vg);
        nvgRestore(vg);

        nvgBeginPath(vg);
        nvgStrokeColor(vg, awSkin.knobStroke());
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 2);
        nvgStroke(vg);
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    float lastVal{0.f};
    void step() override
    {
        bool dirty{false};
        if (lastSkin != awSkin.skin)
            dirty = true;
        lastSkin = awSkin.skin;

        auto pq = getParamQuantity();
        if (pq)
        {
            if (lastVal != pq->getValue())
                dirty = true;
            lastVal = pq->getValue();
        }

        if (bdw && dirty)
            bdw->dirty = dirty;

        rack::Knob::step();
    }
};

struct AWLabel : rack::Widget
{
    float px{11};
    std::string label{"label"};
    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bdw{nullptr};
    AWLabel() {}
    void setup()
    {
        bdw = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(
            rack::Vec(0, 0), box.size, [this](auto vg) { drawLabel(vg); });
        addChild(bdw);
    }
    void drawLabel(NVGcontext *vg)
    {
        auto fid = APP->window->loadFont(awSkin.fontPath)->handle;
        nvgBeginPath(vg);

        nvgFillColor(vg, awSkin.labeLText());

        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_LEFT);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, px);

        nvgText(vg, 0, box.size.y * 0.5, label.c_str(), nullptr);
        float bnd[4];
        nvgTextBounds(vg, 0, box.size.y * 0.5, label.c_str(), nullptr, bnd);
        nvgBeginPath(vg);
        nvgMoveTo(vg, bnd[2] + 4, box.size.y * 0.5);
        nvgLineTo(vg, box.size.x - 4, box.size.y * 0.5);
        nvgStrokeColor(vg, awSkin.labelRule());
        nvgStrokeWidth(vg, 0.5);
        nvgStroke(vg);
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    std::string lastLabel{};
    void step() override
    {
        if (bdw)
        {
            if (lastLabel != label || lastSkin != awSkin.skin)
            {
                bdw->dirty = true;
            }
            lastLabel = label;
            lastSkin = awSkin.skin;
        }
        rack::Widget::step();
    }
};

struct AWJog : rack::Widget
{
    AW2RModule *module;
    int dir{1};
    bool hovered{false};
    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bdw{nullptr};
    void setup()
    {
        bdw = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(
            rack::Vec(0, 0), box.size, [this](auto vg) { drawArrow(vg); });
        addChild(bdw);
    }
    void drawArrow(NVGcontext *vg)
    {
        bool transparent{false};
        if (module && module->lockedType)
            transparent = true;

        auto cy = box.size.y * 0.5;
        auto xp = 3;
        auto sx = box.size.x - 1.8 * xp;
        nvgBeginPath(vg);
        if (dir == -1)
        {
            nvgMoveTo(vg, xp, cy);
            nvgLineTo(vg, xp + sx, cy - sx * 0.5);
            nvgLineTo(vg, xp + sx, cy + sx * 0.5);
            nvgLineTo(vg, xp, cy);
        }
        else
        {
            nvgMoveTo(vg, xp + sx, cy);
            nvgLineTo(vg, xp, cy - sx * 0.5);
            nvgLineTo(vg, xp, cy + sx * 0.5);
            nvgLineTo(vg, xp + sx, cy);
        }
        if (transparent)
        {
            nvgStrokeColor(vg, awSkin.deactivatedJogStroke());
            nvgFillColor(vg, awSkin.deactivatedJogFill());
            nvgFill(vg);
            nvgStroke(vg);
        }
        else
        {
            if (hovered)
                nvgFillColor(vg, awSkin.jogFillHover());
            else
                nvgFillColor(vg, awSkin.jogFill());
            nvgStrokeColor(vg, awSkin.jogStroke());
            nvgFill(vg);
            nvgStroke(vg);
        }
    }

    void onButton(const ButtonEvent &e) override
    {
        Widget::onButton(e);
        if (module && !module->lockedType && e.action == GLFW_PRESS &&
            e.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            std::unordered_set<std::string> coll;
            if (awSkin.collection != "All" && (AirwinRegistry::namesByCollection.find(awSkin.collection) != AirwinRegistry::namesByCollection.end()))
                    coll = AirwinRegistry::namesByCollection[awSkin.collection];

            if (coll.empty())
            {
                // FIXME : move to a static helper somewhere
                auto n = AirwinRegistry::neighborIndexFor(module->selectedFX, dir);
                pushFXChange(module, n);
            }
            else
            {
                int sidx = AirwinRegistry::nameToIndex[module->selectedFX];
                auto nx = AirwinRegistry::neighborIndexFor(sidx, dir);
                while (nx != sidx)
                {
                    auto rg = AirwinRegistry::registry[nx];
                    if (coll.find(rg.name) != coll.end())
                    {
                        pushFXChange(module, nx);
                        break;
                    }

                    nx = AirwinRegistry::neighborIndexFor(nx, dir);
                }
            }
            e.consume(this);
        }
    }

    bool cacheLock{false};
    std::string lastSelected{false};
    void step() override
    {
        if (module && module->lockedType != cacheLock)
        {
            cacheLock = module->lockedType;
            bdw->dirty = true;
        }
        if (module && module->selectedFX != lastSelected)
        {
            bdw->dirty = true;
            setTooltipText();
            lastSelected = module->selectedFX;
        }
        rack::Widget::step();
    }

    void onHover(const HoverEvent &e) override { e.consume(this); }
    void onEnter(const EnterEvent &e) override
    {
        e.consume(this);
        hovered = true;
        bdw->dirty = true;

        if (!rack::settings::tooltips)
            return;
        if (toolTip)
            return;

        toolTip = new rack::ui::Tooltip;
        setTooltipText();
        APP->scene->addChild(toolTip);
    }
    void onLeave(const LeaveEvent &e) override
    {
        e.consume(this);
        hovered = false;
        bdw->dirty = true;

        if (!toolTip)
            return;
        APP->scene->removeChild(toolTip);
        delete toolTip;
        toolTip = nullptr;
    }

    void setTooltipText()
    {
        if (!module)
            return;

        if (!toolTip)
            return;
        auto n = AirwinRegistry::neighborIndexFor(module->selectedFX, dir);
        const auto &r = AirwinRegistry::registry[n];
        toolTip->text = r.name + "\n(" + r.category + ")";
    }

    rack::ui::Tooltip *toolTip{nullptr};
};

struct AWHelp : rack::Widget
{
    std::function<bool()> isOpen{[]() { return false; }};
    std::function<void()> toggleHelp{[]() {}};

    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bdw{nullptr};
    void setup()
    {
        bdw = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(
            rack::Vec(0, 0), box.size, [this](auto vg) { drawHelp(vg); });
        addChild(bdw);
    }
    void drawHelp(NVGcontext *vg)
    {
        auto fid = APP->window->loadFont(awSkin.fontPath)->handle;

        if (isOpen())
        {
            nvgBeginPath(vg);
            nvgFillColor(vg, awSkin.helpClose());
            nvgFontFaceId(vg, fid);
            nvgFontSize(vg, 12);
            nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            nvgText(vg, box.size.x * 0.5, box.size.y * 0.5, "?", nullptr);
        }
        else
        {
            nvgBeginPath(vg);
            nvgFillColor(vg, awSkin.helpOpen());
            nvgFontFaceId(vg, fid);
            nvgFontSize(vg, 12);
            nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            nvgText(vg, box.size.x * 0.5, box.size.y * 0.5, "?", nullptr);
        }
    }

    void onButton(const ButtonEvent &e) override
    {
        if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            toggleHelp();
            e.consume(this);
        }
        Widget::onButton(e);
    }

    bool cacheOpen{false};
    void step() override
    {
        if (cacheOpen != isOpen())
        {
            cacheOpen = isOpen();
            bdw->dirty = true;
        }
        rack::Widget::step();
    }
};

struct AWSelector : rack::Widget
{
    AW2RModule *module;

    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bdw{nullptr};
    AWJog *leftJ{nullptr}, *rightJ{nullptr};
    static constexpr int jogButttonSize{12};
    void setup()
    {
        bdw = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(
            rack::Vec(0, 0), box.size, [this](auto vg) { drawSelector(vg); });
        addChild(bdw);

        auto asz{jogButttonSize - 1};
        auto downShift{10};
        leftJ = new AWJog;
        leftJ->module = module;
        leftJ->dir = -1;
        leftJ->box.pos = {1, 0};
        leftJ->box.size = box.size;
        leftJ->box.size.x = asz;
        leftJ->box.pos.y += downShift;
        leftJ->box.size.y -= downShift;
        leftJ->setup();
        addChild(leftJ);

        rightJ = new AWJog;
        rightJ->module = module;
        rightJ->dir = 1;
        rightJ->box.pos = {0, 0};
        rightJ->box.size = box.size;
        rightJ->box.size.x = asz;
        rightJ->box.pos.x = box.size.x - asz - 2;
        rightJ->box.pos.y += downShift;
        rightJ->box.size.y -= downShift;
        rightJ->setup();
        addChild(rightJ);
    }

    struct SearchField : rack::ui::TextField
    {
        rack::WeakPtr<rack::ui::Menu> menu{nullptr};
        rack::WeakPtr<AWSelector> selector{nullptr};
        std::string lastText{};
        void step() override
        {
            if (text != lastText)
            {
                if (selector)
                    selector->resetMenuForSearch(text, menu);
            }
            lastText = text;
            // Keep selected
            APP->event->setSelectedWidget(this);
            TextField::step();
        }
    };
    void drawSelector(NVGcontext *vg)
    {
        // auto fid = APP->window->loadFont(fontPath)->handle;
        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.selectorOutlineHighlight());
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y, 2);
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.selectorFill());
        nvgStrokeColor(vg, awSkin.selectorOutline());
        nvgRoundedRect(vg, 0, 0, box.size.x, box.size.y - 1, 2);
        nvgFill(vg);
        nvgStrokeWidth(vg, 0.5);
        nvgStroke(vg);

        auto fid = APP->window->loadFont(awSkin.fontPath)->handle;

        // Find font size to make sure we fit in the box
        auto fontSize = 14.5;
        bool fits{false};
        while (!fits && fontSize > 9)
        {
            nvgFontFaceId(vg, fid);
            nvgFontSize(vg, fontSize);
            float bnd[4];
            nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
            nvgTextBounds(vg, box.size.x * 0.5, box.size.y * 0.65, lastName.c_str(), nullptr, bnd);

            if (bnd[0] < jogButttonSize + 5)
                fontSize -= 0.5;
            else
                fits = true;
        }

        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.selectorEffect());
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, fontSize);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.65, lastName.c_str(), nullptr);

        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.selectorCategory());
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 8.5);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.22, lastCat.c_str(), nullptr);

        if (lastPoly == AW2RModule::POLYPHONIC || lastPoly == AW2RModule::POLYPHONIC_MIXMASTER ||
            lastPoly == AW2RModule::MIXMASTER_TO_STEREO_POLY ||
            lastPoly == AW2RModule::STEREO_POLY_TO_MIXMASTER)
        {
            nvgBeginPath(vg);
            nvgFillColor(vg, awSkin.selectorPoly());
            nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);
            nvgFontFaceId(vg, fid);
            nvgFontSize(vg, 8.5);
            nvgText(vg, 3, 1, "poly", nullptr);
        }
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    std::string lastName{"Airwindows"}, lastCat{"Multi-Effect"};
    AW2RModule::PolyphonyMode lastPoly{AW2RModule::MONOPHONIC};
    void step() override
    {
        if (module && bdw)
        {
            if (lastName != module->selectedFX || lastCat != module->selectedCat ||
                lastSkin != awSkin.skin || lastPoly != module->polyphonyMode)
            {
                bdw->dirty = true;
            }
            lastPoly = module->polyphonyMode;
            lastName = module->selectedFX;
            lastCat = module->selectedCat;
            lastSkin = awSkin.skin;
        }
        rack::Widget::step();
    }

    void onButton(const ButtonEvent &e) override
    {
        Widget::onButton(e);
        if (!e.isConsumed() && module && e.action == GLFW_PRESS)
        {
            showSelectorMenu();
            e.consume(this);
        }
    }

    void showSelectorMenu()
    {
        if (!module)
            return;

        auto m = rack::createMenu();
        m->addChild(rack::createMenuLabel("Airwindows Selector"));
        m->addChild(new rack::MenuSeparator);

        m->addChild(rack::createMenuLabel("Search"));
        auto sf = new SearchField;
        sf->box.size.x = 100;
        sf->menu = m;
        sf->selector = this;
        m->addChild(sf);

        m->addChild(rack::createSubmenuItem("Filter by Collection", "",
                                            [this](auto *m) { createCollectionMenu(m); }));

        m->addChild(new rack::MenuSeparator);
        buildCategoryMenuOnto(m);
    }

    void resetMenuForSearch(const std::string &msg, rack::Menu *m)
    {
        if (!m)
            return;
        // First entries are label separator label search separator
        auto pos = m->children.begin();
        for (int i = 0; i < 5 && pos != m->children.end(); ++i)
            pos = std::next(pos);

        std::vector<rack::Widget *> toDelete;
        while (pos != m->children.end())
        {
            toDelete.push_back(*pos);
            pos = std::next(pos);
        }
        for (const auto &p : toDelete)
        {
            m->removeChild(p);
            delete p;
        }
        if (msg.empty())
        {
            buildCategoryMenuOnto(m);
        }
        else
        {
            std::unordered_set<std::string> coll;
            if (awSkin.collection != "All" && (AirwinRegistry::namesByCollection.find(awSkin.collection) != AirwinRegistry::namesByCollection.end()))
                coll = AirwinRegistry::namesByCollection[awSkin.collection];

            auto st = rack::string::lowercase(msg);
            std::map<std::string, int> result;
            for (auto i = 0U; i < AirwinRegistry::registry.size(); ++i)
            {
                const auto &r = AirwinRegistry::registry[i];
                if (coll.empty() || (coll.find(r.name) != coll.end()))
                {
                    auto tg = rack::string::lowercase(r.name);
                    if (tg.find(st) != std::string::npos)
                        result[r.name] = i;
                }
            }

            int maxEntries = 25;

            if (result.empty())
            {
                m->addChild(rack::createMenuLabel("No Match: '" + msg + "'"));
            }
            else
            {
                int ct = 0;
                for (const auto &[name, index] : result)
                {
                    const auto &r = AirwinRegistry::registry[index];
                    if (ct++ > maxEntries)
                        break;
                    m->addChild(rack::createMenuItem(
                        name, r.category, [this, i = index]() { pushFXChange(module, i); }));
                }
                if (ct > maxEntries)
                {
                    m->addChild(rack::createMenuLabel(std::to_string(result.size() - maxEntries) +
                                                      " more matches..."));
                }
            }
        }
    }

    void buildCategoryMenuOnto(rack::Menu *m)
    {
        std::unordered_set<std::string> coll;
        if (awSkin.collection != "All" && (AirwinRegistry::namesByCollection.find(awSkin.collection) != AirwinRegistry::namesByCollection.end()))
            coll = AirwinRegistry::namesByCollection[awSkin.collection];

        for (const auto &cat : AirwinRegistry::categories)
        {
            bool include = true;
            if (!coll.empty())
            {
                for (const auto &reg : AirwinRegistry::registry)
                {
                    if (reg.category == cat)
                    {
                        if (coll.find(reg.name) != coll.end())
                        {
                            include = true;
                        }
                    }
                }
            }
            if (include)
            {
                m->addChild(rack::createSubmenuItem(
                    cat, "", [this, cat](auto *m) { createCategoryMenu(m, cat); }));
            }
        }

        m->addChild(new rack::MenuSeparator);
        m->addChild(rack::createMenuItem("Pick An Effect At Random", "", [this]() {
            auto r = rack::random::u32() % AirwinRegistry::registry.size();
            pushFXChange(module, r);
        }));
        m->addChild(new rack::MenuSeparator);
        m->addChild(rack::createMenuItem("Lock Effect Choice", CHECKMARK(module->lockedType),
                                         [this]() { module->lockedType = !module->lockedType; }));
        m->addChild(rack::createMenuItem("Randomize Changes FX Choice",
                                         CHECKMARK(module->randomizeFX),
                                         [this]() { module->randomizeFX = !module->randomizeFX; }));
        m->addChild(new rack::MenuSeparator);
        m->addChild(rack::createMenuItem("Categories in Alphabetical Order",
                                         CHECKMARK(awSkin.menuOrdering == AWSkin::ALPHA),
                                         []() { awSkin.changeOrderingTo(AWSkin::ALPHA); }));
        m->addChild(rack::createMenuItem("Categories in 'Chris' (quality) Order",
                                         CHECKMARK(awSkin.menuOrdering == AWSkin::CHRIS),
                                         []() { awSkin.changeOrderingTo(AWSkin::CHRIS); }));
    }
    void createCollectionMenu(rack::Menu *m)
    {
        auto coll = awSkin.collection;
        for (const auto &[c, _] : AirwinRegistry::namesByCollection)
        {
            m->addChild(rack::createMenuItem(c, CHECKMARK(coll == c), [c](){
                awSkin.collection = c;
                awSkin.writeConfig();
            }));
        }
        m->addChild(new rack::MenuSeparator());
        m->addChild(rack::createMenuItem("All Effects", CHECKMARK(coll == "All"), [](){
            awSkin.collection = "All";
            awSkin.writeConfig();
        }));
    }
    void createCategoryMenu(rack::Menu *m, const std::string &cat)
    {
        std::unordered_set<std::string> coll;
        if (awSkin.collection != "All" && (AirwinRegistry::namesByCollection.find(awSkin.collection) != AirwinRegistry::namesByCollection.end()))
            coll = AirwinRegistry::namesByCollection[awSkin.collection];

        if (awSkin.menuOrdering == AWSkin::CHRIS)
        {
            int idx = 0;
            std::vector<std::pair<int, int>> catIndexAndChris;
            for (const auto &item : AirwinRegistry::registry)
            {
                if (item.category == cat)
                {
                    catIndexAndChris.emplace_back(idx, item.catChrisOrdering);
                }
                idx++;
            }
            std::sort(catIndexAndChris.begin(), catIndexAndChris.end(),
                      [](const auto &a, const auto &b) { return a.second < b.second; });

            for (const auto &[ridx, ord] : catIndexAndChris)
            {
                const auto &r = AirwinRegistry::registry[ridx];
                const auto &name = r.name;
                if (coll.empty() || (coll.find(name) != coll.end()))
                {
                    auto checked = name == module->selectedFX;
                    auto mi = rack::createMenuItem(name, CHECKMARK(checked),
                                                   [this, i = ridx]() { pushFXChange(module, i); });
                    mi->disabled = module->lockedType;
                    m->addChild(mi);
                }
            }
        }
        else
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
                if (coll.empty() || (coll.find(name) != coll.end()))
                {
                    auto mi = rack::createMenuItem(name, CHECKMARK(checked),
                                                   [this, i = idx]() { pushFXChange(module, i); });
                    mi->disabled = module->lockedType;
                    m->addChild(mi);
                }
            }
        }
    }

    void onHover(const HoverEvent &e) override
    {
        rack::Widget::onHover(e);
        if (!e.isConsumed())
            e.consume(this);
    }
    void onEnter(const EnterEvent &e) override
    {
        rack::Widget::onEnter(e);
        if (e.isConsumed())
        {
            return;
        }
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
        toolTip->text = splitTo(module->selectedWhat, 32);
        APP->scene->addChild(toolTip);
    }

    std::string splitTo(const std::string &s, int n)
    {
        auto q = s;
        std::ostringstream res;
        while ((int)q.size() > n)
        {
            auto ps = n;
            while (q[ps] != ' ' && ps > 0)
                ps--;

            if (ps == 0)
                return s;

            res << q.substr(0, ps) << "\n";
            q = q.substr(ps + 1);
        }
        res << q;
        return res.str();
    }

    void onLeave(const LeaveEvent &e) override
    {
        rack::Widget::onLeave(e);
        if (e.isConsumed())
        {
            return;
        }

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
    std::array<PixelKnob<20> *, M::maxParams> parKnobs;
    std::array<rack::ParamWidget *, M::maxParams> attenKnobs;
    std::array<AWPort *, M::maxParams> cvPorts;

    std::shared_ptr<rack::Svg> clipperSvg;
    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bg{nullptr};

    AW2RModuleWidget(M *m)
    {
        awSkin.intialize();
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 10, RACK_HEIGHT);

        clipperSvg = rack::Svg::load(rack::asset::plugin(pluginInstance, "res/clipper.svg"));

        bg = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
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

        auto hs = new AWHelp;
        hs->box.size = rack::Vec(12, 12);
        hs->box.pos.x = tlab->box.pos.x + tlab->box.size.x - 14;
        hs->box.pos.y = tlab->box.pos.y + 1;
        hs->isOpen = [this]() { return helpShowing; };
        hs->toggleHelp = [this]() { toggleHelp(); };
        hs->setup();
        addChild(hs);

        auto pPos = headerSize + 1, dPP = 27;

        for (int i = 0; i < M::maxParams; ++i)
        {
            auto tlab = new AWLabel;
            tlab->px = 12;
            tlab->box.pos.x = 5;
            tlab->box.pos.y = pPos;
            tlab->box.size.x = box.size.x - 76;
            tlab->box.size.y = dPP;
            tlab->label = "Param " + std::to_string(i);
            tlab->setup();
            parLabels[i] = tlab;
            addChild(tlab);

            auto bp = box.size.x - 60;
            auto k = rack::createParamCentered<PixelKnob<20>>(rack::Vec(bp, pPos + dPP * 0.5),
                                                              module, M::PARAM_0 + i);
            k->stripMenuTypein = true;
            parKnobs[i] = k;
            addParam(parKnobs[i]);

            attenKnobs[i] = rack::createParamCentered<PixelKnob<12, true>>(
                rack::Vec(bp + 20, pPos + dPP * 0.5), module, M::ATTENUVERTER_0 + i);
            addParam(attenKnobs[i]);

            cvPorts[i] = rack::createInputCentered<AWPort>(rack::Vec(bp + 42, pPos + dPP * 0.5),
                                                           module, M::CV_0 + i);
            addInput(cvPorts[i]);

            pPos += dPP;
        }

        auto q = RACK_HEIGHT - 42 - 9;
        auto c1 = box.size.x * 0.25;
        auto dc = box.size.x * 0.11;
        auto c2 = box.size.x * 0.75;
        auto inl = rack::createInputCentered<AWPort>(rack::Vec(c1 - dc, q), module, M::INPUT_L);
        inl->connectAsInputFromMixmaster = true;
        inl->mixMasterStereoCompanion = M::INPUT_R;
        auto inr = rack::createInputCentered<AWPort>(rack::Vec(c1 + dc, q), module, M::INPUT_R);
        inr->connectAsInputFromMixmaster = true;
        inr->mixMasterStereoCompanion = M::INPUT_L;

        auto outl = rack::createOutputCentered<AWPort>(rack::Vec(c2 - dc, q), module, M::OUTPUT_L);
        outl->connectAsOutputToMixmaster = true;
        outl->mixMasterStereoCompanion = M::OUTPUT_R;
        outl->connectOutputToNeighbor = true;

        auto outr = rack::createOutputCentered<AWPort>(rack::Vec(c2 + dc, q), module, M::OUTPUT_R);
        outr->connectAsOutputToMixmaster = true;
        outr->mixMasterStereoCompanion = M::OUTPUT_L;
        outr->connectOutputToNeighbor = true;

        addInput(inl);
        addInput(inr);
        addOutput(outl);
        addOutput(outr);

        auto w = box.size.x * 0.5 - 16;
        auto inAt = AttenSlider::create(rack::Vec(8, q + 24), w, m, M::IN_LEVEL);
        addParam(inAt);
        auto outAt =
            AttenSlider::create(rack::Vec(8 + box.size.x * 0.5, q + 24), w, m, M::OUT_LEVEL);
        addParam(outAt);

        // Add sliders here
    }

    ~AW2RModuleWidget()
    {
        if (helpWidget)
        {
            APP->scene->removeChild(helpWidget);
            delete helpWidget;
        }
    }

    void blockSizeMenu(rack::Menu *menu)
    {
        auto awm = dynamic_cast<AW2RModule *>(module);

        if (!awm)
            return;

        menu->addChild(rack::createMenuLabel("Block Size"));
        menu->addChild(new rack::MenuSeparator);
        int bs = 4;
        while (bs <= AW2RModule::maxBlockSize)
        {
            auto s = std::to_string(bs);
            if (bs == 4)
                s += " (Lowest Latency)";
            if (bs == AW2RModule::maxBlockSize)
                s += " (Less CPU)";
            menu->addChild(rack::createMenuItem(s, CHECKMARK(awm->blockSize == bs),
                                                [awm, bs]() { awm->forceBlockSize = bs; }));
            bs = bs << 1;
        }
    }
    void appendContextMenu(rack::Menu *menu) override
    {
        auto awm = dynamic_cast<AW2RModule *>(module);
        menu->addChild(new rack::MenuSeparator);
        menu->addChild(rack::createMenuItem("Panic Reset", "", [awm]() {
            if (awm)
            {
                awm->panicReset = true;
            }
        }));
        menu->addChild(new rack::MenuSeparator);
        menu->addChild(rack::createMenuItem("Light Mode", CHECKMARK(awSkin.skin == AWSkin::LIGHT),
                                            []() { awSkin.changeTo(AWSkin::LIGHT, true); }));
        menu->addChild(rack::createMenuItem("Dark Mode", CHECKMARK(awSkin.skin == AWSkin::DARK),
                                            []() { awSkin.changeTo(AWSkin::DARK, true); }));

        if (awm)
        {
            menu->addChild(new rack::MenuSeparator);
            menu->addChild(rack::createMenuItem(
                "Monophonic (Sum Inputs)", CHECKMARK(awm->polyphonyMode == AW2RModule::MONOPHONIC),
                [awm, this]() {
                    awm->stagePolyReset(AW2RModule::MONOPHONIC);
                    bg->dirty = true;
                }));
            menu->addChild(rack::createMenuItem(
                "Monophonic (MixMaster Sum Input)",
                CHECKMARK(awm->polyphonyMode == AW2RModule::MIXMASTER_TO_MONOPHONIC),
                [awm, this]() {
                    awm->stagePolyReset(AW2RModule::MIXMASTER_TO_MONOPHONIC);
                    bg->dirty = true;
                }));
            menu->addChild(rack::createMenuItem(
                "Polyphonic (Stereo to Stereo)",
                CHECKMARK(awm->polyphonyMode == AW2RModule::POLYPHONIC), [awm, this]() {
                    awm->stagePolyReset(AW2RModule::POLYPHONIC);
                    bg->dirty = true;
                }));
            menu->addChild(rack::createMenuItem(
                "Polyphonic (MixMaster to MixMaster)",
                CHECKMARK(awm->polyphonyMode == AW2RModule::POLYPHONIC_MIXMASTER), [awm, this]() {
                    awm->stagePolyReset(AW2RModule::POLYPHONIC_MIXMASTER);
                    bg->dirty = true;
                }));
            menu->addChild(rack::createMenuItem(
                "Polyphonic (MixMaster to Stereo)",
                CHECKMARK(awm->polyphonyMode == AW2RModule::MIXMASTER_TO_STEREO_POLY),
                [awm, this]() {
                    awm->stagePolyReset(AW2RModule::MIXMASTER_TO_STEREO_POLY);
                    bg->dirty = true;
                }));
            menu->addChild(rack::createMenuItem(
                "Polyphonic (Stereo to MixMaster)",
                CHECKMARK(awm->polyphonyMode == AW2RModule::STEREO_POLY_TO_MIXMASTER),
                [awm, this]() {
                    awm->stagePolyReset(AW2RModule::STEREO_POLY_TO_MIXMASTER);
                    bg->dirty = true;
                }));

            menu->addChild(new rack::MenuSeparator);
            auto s = "Block Size (" + std::to_string(awm->blockSize) + ")";
            menu->addChild(rack::createSubmenuItem(s, "", [this](auto m) { blockSizeMenu(m); }));

            menu->addChild(new rack::MenuSeparator);
            menu->addChild(rack::createMenuItem("Lock Effect Choice", CHECKMARK(awm->lockedType),
                                                [awm]() { awm->lockedType = !awm->lockedType; }));
            menu->addChild(rack::createMenuItem("Randomize Changes FX Choice",
                                                CHECKMARK(awm->randomizeFX),
                                                [awm]() { awm->randomizeFX = !awm->randomizeFX; }));

#define SHOW_STATS 0
#if SHOW_STATS
            menu->addChild(rack::createMenuItem("Library Stats to Stdout", "",
                                                []() { AirwinRegistry::dumpStatsToStdout(); }));

#endif
        }

        menu->addChild(new rack::MenuSeparator);
        menu->addChild(rack::createMenuItem("Categories in Alphabetical Order",
                                            CHECKMARK(awSkin.menuOrdering == AWSkin::ALPHA),
                                            []() { awSkin.changeOrderingTo(AWSkin::ALPHA); }));
        menu->addChild(rack::createMenuItem("Categories in 'Chris' (quality) Order",
                                            CHECKMARK(awSkin.menuOrdering == AWSkin::CHRIS),
                                            []() { awSkin.changeOrderingTo(AWSkin::CHRIS); }));
    }

    struct HelpWidget : rack::Widget
    {
        double ctime;
        static constexpr int margin{3};
        rack::ui::ScrollWidget *sw{nullptr};
        HelpWidget() {}

        struct Render : rack::Widget
        {
            HelpWidget *hw{nullptr};
            void draw(const DrawArgs &args)
            {
                assert(hw);
                auto vg = args.vg;

                int yp = 3;
                auto fid = APP->window->loadFont(awSkin.fontPath)->handle;
                auto fidm = APP->window->loadFont(awSkin.fontPathMedium)->handle;

                nvgBeginPath(vg);
                nvgFillColor(vg, awSkin.helpText());
                nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);

                float bnd[6];
                nvgSave(vg);
                nvgScale(vg, APP->scene->rackScroll->getZoom(), APP->scene->rackScroll->getZoom());
                nvgFontSize(vg, 14);
                nvgFontFaceId(vg, fid);
                nvgTextBox(vg, margin + 2, margin + 2,
                           box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                           hw->helpTitle.c_str(), nullptr);
                nvgTextBoxBounds(vg, margin + 2, margin + 2,
                                 box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                                 hw->helpTitle.c_str(), nullptr, bnd);
                nvgRestore(vg);

                nvgSave(vg);
                nvgScale(vg, APP->scene->rackScroll->getZoom(), APP->scene->rackScroll->getZoom());
                nvgFontSize(vg, 12);
                nvgFontFaceId(vg, fidm);
                nvgTextBox(vg, margin + 2, bnd[3] + margin + 2,
                           box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                           hw->helpText.c_str(), nullptr);
                nvgTextBoxBounds(vg, margin + 2, bnd[3] + margin + 2,
                                 box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                                 hw->helpText.c_str(), nullptr, bnd);
                nvgRestore(vg);

                box.size.y = bnd[3];
            }
        } *render{nullptr};

        void setup()
        {
            ctime = rack::system::getTime();
            sw = new rack::ui::ScrollWidget;
            sw->box.pos = rack::Vec(margin, margin);
            sw->box.size = box.size;
            sw->box.size.x -= 2 * margin;
            sw->box.size.y -= 2 * margin;
            addChild(sw);

            render = new Render();
            render->hw = this;
            render->box.pos = rack::Vec(0, 0);
            render->box.size = sw->box.size;
            sw->container->addChild(render);
        }

        float pct = 0;
        void draw(const DrawArgs &args) override
        {
            auto vg = args.vg;
            nvgBeginPath(vg);
            nvgRect(vg, 0, 0, box.size.x, box.size.y);
            nvgFillColor(vg, awSkin.helpBorder());
            nvgFill(vg);

            nvgBeginPath(vg);
            nvgRect(vg, margin, margin, box.size.x - 2 * margin, box.size.y - 2 * margin);
            nvgFillColor(vg, awSkin.helpBG());
            nvgFill(vg);

            rack::Widget::draw(args);
        }

        std::string helpTitle, helpText;
        void setFX(const std::string &s)
        {
            helpText = "";
            auto fxp = rack::asset::plugin(pluginInstance, "res/awpdoc/" + s + ".txt");
            try
            {
                auto rf = rack::system::readFile(fxp);
                std::ostringstream oss;
                for (const auto &d : rf)
                {
                    if (d != '\r')
                        oss << (char)d;
                }

                std::stringstream ss(oss.str());
                std::string token;
                std::vector<std::string> tokens;
                while (std::getline(ss, token, '\n'))
                {
                    tokens.push_back(token);
                }

                if (tokens.empty())
                {
                }
                else if (tokens.size() < 2)
                {
                    helpTitle = tokens[0];
                }
                else
                {
                    helpTitle = tokens[0];
                    int start = tokens[1].empty() ? 1 : 2;
                    helpText = "";
                    for (int i = start; i < (int)tokens.size(); ++i)
                        helpText += tokens[i] + "\n";
                }

                if (helpTitle[0] == '#')
                    helpTitle = helpTitle.substr(2);
            }
            catch (rack::Exception e)
            {
                helpText = "No Help Available for " + s;
            }
            catch (...)
            {
                // For some reason the read throws something which isn't a rack::exception so...
                helpText = "No Help Available for " + s;
            }

            resetBounds();
        }

        void resetBounds()
        {
            sw->box.pos = rack::Vec(margin, margin);
            sw->box.size = box.size;
            sw->box.size.x -= 2 * margin;
            sw->box.size.y -= 2 * margin;

            render->box.size = sw->box.size;
        }

        float lastZoom{1.f};
        void step() override
        {
            if (APP->scene->rackScroll->getZoom() != lastZoom)
            {
                resetBounds();
            }
            lastZoom = APP->scene->rackScroll->getZoom();

            rack::Widget::step();
        }
    };

    HelpWidget *helpWidget{nullptr};
    bool helpShowing{false};
    void toggleHelp()
    {
        if (helpShowing)
        {
            if (helpWidget)
            {
                APP->scene->removeChild(helpWidget);
                delete helpWidget;
                helpWidget = nullptr;
            }
            helpShowing = false;
        }
        else
        {
            assert(!helpWidget);
            auto awm = dynamic_cast<AW2RModule *>(module);
            if (awm)
            {
                helpWidget = new HelpWidget;
                helpWidget->box.pos = getAbsoluteOffset(rack::Vec(box.size.x, 0));
                helpWidget->box.size.x = 300 * APP->scene->rackScroll->getZoom();
                helpWidget->box.size.y = RACK_HEIGHT * APP->scene->rackScroll->getZoom();
                helpWidget->setup();
                helpWidget->setFX(awm->selectedFX);
                helpShowing = true;
                APP->scene->addChild(helpWidget);
            }
        }
    }

    void drawBG(NVGcontext *vg)
    {
        auto cutPoint{67};

        // Main Gradient Background
        nvgBeginPath(vg);
        nvgFillPaint(vg, nvgLinearGradient(vg, 0, 50, 0, box.size.y - cutPoint,
                                           awSkin.panelGradientStart(), awSkin.panelGradientEnd()));

        nvgRect(vg, 0, 0, box.size.x, box.size.y - cutPoint);
        nvgFill(vg);
        nvgStroke(vg);

        // Draw the bottom region
        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.panelBottomRegion());
        nvgStrokeColor(vg, awSkin.panelBottomStroke());
        nvgStrokeWidth(vg, 0.5);
        nvgRect(vg, 0, box.size.y - cutPoint, box.size.x, cutPoint);
        nvgFill(vg);
        nvgStroke(vg);

        // Input region
        auto fid = APP->window->loadFont(awSkin.fontPath)->handle;
        nvgBeginPath(vg);
        nvgStrokeColor(vg, awSkin.panelInputBorder());
        nvgFillColor(vg, awSkin.panelInputFill());
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, 4, box.size.y - cutPoint + 3, box.size.x * 0.5 - 8, 37 + 9, 2);
        nvgFill(vg);
        nvgStroke(vg);

        auto dc = box.size.x * 0.11;

        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.panelInputText());
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 10);
        nvgText(vg, box.size.x * 0.25, box.size.y - cutPoint + 38, "IN", nullptr);

        auto awm = dynamic_cast<AW2RModule *>(module);
        // use nextPoly here since audio thread may have not swept it yet
        if (!awm || ((awm->nextPoly != AW2RModule::POLYPHONIC_MIXMASTER) &&
                     (awm->nextPoly != AW2RModule::MIXMASTER_TO_MONOPHONIC) &&
                     (awm->nextPoly != AW2RModule::MIXMASTER_TO_STEREO_POLY)))
        {
            nvgFontSize(vg, 10);
            nvgText(vg, box.size.x * 0.25 - dc, box.size.y - cutPoint + 38, "L", nullptr);
            nvgText(vg, box.size.x * 0.25 + dc, box.size.y - cutPoint + 38, "R", nullptr);
        }
        else
        {
            nvgFontSize(vg, 10);
            nvgText(vg, box.size.x * 0.25 - dc, box.size.y - cutPoint + 38, "1-8", nullptr);
            nvgText(vg, box.size.x * 0.25 + dc, box.size.y - cutPoint + 38, "9-16", nullptr);
        }
        // Output region
        nvgBeginPath(vg);
        nvgStrokeColor(vg, awSkin.panelOutputBorder());
        nvgFillColor(vg, awSkin.panelOutputFill());
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, box.size.x * 0.5 + 4, box.size.y - cutPoint + 3, box.size.x * 0.5 - 8,
                       37 + 9, 2);
        nvgFill(vg);
        nvgStroke(vg);

        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.panelOutputText());
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        if (!awm || ((awm->nextPoly != AW2RModule::POLYPHONIC_MIXMASTER) &&
                     (awm->nextPoly != AW2RModule::STEREO_POLY_TO_MIXMASTER)))
        {
            nvgFontSize(vg, 10);
            nvgText(vg, box.size.x * 0.75, box.size.y - cutPoint + 38, "OUT", nullptr);
            nvgFontSize(vg, 10);
            nvgText(vg, box.size.x * 0.75 - dc, box.size.y - cutPoint + 38, "L", nullptr);
            nvgText(vg, box.size.x * 0.75 + dc, box.size.y - cutPoint + 38, "R", nullptr);
        }
        else
        {
            nvgFontSize(vg, 10);
            nvgText(vg, box.size.x * 0.75, box.size.y - cutPoint + 38, "O", nullptr);
            nvgFontSize(vg, 10);
            nvgText(vg, box.size.x * 0.75 - dc, box.size.y - cutPoint + 38, "1-8", nullptr);
            nvgText(vg, box.size.x * 0.75 + dc, box.size.y - cutPoint + 38, "9-16", nullptr);
        }
        // Brand
        nvgBeginPath(vg);
        nvgFillColor(vg, awSkin.panelBrandText());
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
            nvgAlpha(vg, awSkin.svgAlpha());
            clipperSvg->draw(vg);
            nvgRestore(vg);
        }

        // Outline the module
        nvgBeginPath(vg);
        nvgStrokeColor(vg, awSkin.moduleOutline());
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

        if (helpWidget)
        {
            helpWidget->box.pos = getAbsoluteOffset(rack::Vec(box.size.x, 0));
            helpWidget->box.size.y = RACK_HEIGHT * APP->scene->rackScroll->getZoom();
            helpWidget->box.size.x = 300 * APP->scene->rackScroll->getZoom();
        }

        if (helpWidget && APP->scene->browser && APP->scene->browser->isVisible())
        {
            toggleHelp();
        }
        if (lastSkin != awSkin.skin)
        {
            lastSkin = awSkin.skin;
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
            parKnobs[i]->stripMenuTypein = !awm->airwin_display->canConvertParameterTextToValue(i);
            attenKnobs[i]->setVisible(true);
            cvPorts[i]->setVisible(true);
            cvPorts[i]->setPortActive(true);
        }
        for (int i = np; i < M::maxParams; ++i)
        {
            parLabels[i]->setVisible(false);
            parKnobs[i]->setVisible(false);
            attenKnobs[i]->setVisible(false);
            cvPorts[i]->setVisible(true);
            cvPorts[i]->setPortActive(false);
        }

        if (helpWidget)
        {
            helpWidget->setFX(awm->selectedFX);
        }
    }
};

rack::Model *airwin2RackModel = rack::createModel<AW2RModule, AW2RModuleWidget>("Airwin2Rack");