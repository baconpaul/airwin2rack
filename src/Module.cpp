#include "Airwin2Rack.hpp"
#include "airwin2rackbase.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>
#include <atomic>

#include "AirwinRegistry.h"

// @TODO: Cleanup skin colors and fonts into methods
// @TODO: Scroll the Help Area and fonts
// @TODO: Cloud perlin

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
    static constexpr int maxParams{10};

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
        MAX_PARAMS_USED_TO_BE_11_DONT_BREAK_FOLKS = PARAM_0 + maxParams,
        ATTENUVERTER_0,
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
    std::atomic<bool> polyphonic{false}, lockedType{false}, randomizeFX{false};
    AW2RModule()
    {
        assert(!AirwinRegistry::registry.empty());
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

        configInput(INPUT_L, "Left / Mono Input");
        configInput(INPUT_L, "Right Input");
        configOutput(OUTPUT_L, "Left Output");
        configOutput(OUTPUT_L, "Right Output");
        configBypass(INPUT_L, OUTPUT_L);
        configBypass(INPUT_R, OUTPUT_R);

        configParam(MAX_PARAMS_USED_TO_BE_11_DONT_BREAK_FOLKS, 0, 1, 0, "Unused");

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

        monoIO.reset();
        for (int c = 0; c < MAX_POLY; ++c)
            polyIO[c].reset();

        resetCount++;
    }

    json_t *dataToJson() override
    {
        auto res = json_object();
        json_object_set(res, "airwindowSelectedFX", json_string(selectedFX.c_str()));
        json_object_set(res, "polyphonic", json_boolean(polyphonic));
        json_object_set(res, "lockedType", json_boolean(lockedType));
        json_object_set(res, "randomizeFX", json_boolean(randomizeFX));
        json_object_set(res, "blockSize", json_integer(blockSize));
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
        auto awlt = json_object_get(rootJ, "lockedType");
        if (awlt)
        {
            auto bl = json_boolean_value(awlt);
            lockedType = bl;
        }
        auto awrf = json_object_get(rootJ, "randomizeFX");
        if (awrf)
        {
            auto bl = json_boolean_value(awrf);
            randomizeFX = bl;
        }
        auto awbs = json_object_get(rootJ, "blockSize");
        if (awbs)
        {
            auto bl = json_integer_value(awbs);
            forceBlockSize = bl;
        }
    }

    bool nextPoly{polyphonic};
    void stagePolyReset(bool b) { nextPoly = b; }

    void resetPolyphony(bool isPoly)
    {
        if (isPoly == polyphonic)
            return;

        polyphonic = isPoly;
        nextPoly = isPoly;

        resetAirwinByName(selectedFX, false);
        if (polyphonic)
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
        if (nextPoly != polyphonic)
        {
            resetPolyphony(nextPoly);
        }
        if (forceSelect != -1) // a UI action doesn't warrant the compare_exchange rigamarole
        {
            resetAirwindowTo(forceSelect);
            forceSelect = -1;
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

        float sv[maxParams];
        bool isMod[maxParams];
        for (int i = 0; i < nParams; ++i)
        {
            sv[i] = paramQuantities[PARAM_0 + i]->getSmoothValue();
            isMod[i] = inputs[CV_0 + i].isConnected();
        }

        for (int c = 0; c < chanCt; ++c)
        {
            polyIO[c].in[0][polyIO[c].inPos] = inputs[INPUT_L].getVoltage(c) * 0.2;
            polyIO[c].in[1][polyIO[c].inPos] = inputs[rc].getVoltage(c) * 0.2;
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


struct AWPort : public rack::app::SvgPort
{
    AWPort() { setPortActive(true); }

    bool active{true};
    void setPortActive(bool b)
    {
        active=b;
        if (skinManager.skin == AWSkin::DARK)
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
                setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/port_on_light.svg")));
            }
            else
            {
                setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/port_off_light.svg")));
            }
        }
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    void step() override
    {
        bool dirty{false};
        if (lastSkin != skinManager.skin)
            setPortActive(active);
        lastSkin = skinManager.skin;

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
    BufferedDrawFunctionWidget *bdw{nullptr};
    bool stripMenuTypein{false};
    PixelKnob()
    {
        box.size = rack::Vec(px + 3, px + 3);
        float angleSpreadDegrees = 40.0;

        minAngle = -M_PI * (180 - angleSpreadDegrees) / 180;
        maxAngle = M_PI * (180 - angleSpreadDegrees) / 180;

        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawKnob(vg); });
        addChild(bdw);
    }

    void drawKnob(NVGcontext *vg)
    {
        float radius = px * 0.48;
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

    AWSkin::Skin lastSkin{AWSkin::DARK};
    float lastVal{0.f};
    void step() override
    {
        bool dirty{false};
        if (lastSkin != skinManager.skin)
            dirty = true;
        lastSkin = skinManager.skin;

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

struct AWLabel : rack::Widget
{
    float px{11};
    std::string label{"label"};
    BufferedDrawFunctionWidget *bdw{nullptr};
    AWLabel() {}
    void setup()
    {
        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawLabel(vg); });
        addChild(bdw);
    }
    void drawLabel(NVGcontext *vg)
    {
        auto fid = APP->window->loadFont(skinManager.fontPath)->handle;
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

struct AWJog : rack::Widget
{
    AW2RModule *module;
    int dir{1};
    bool hovered{false};
    BufferedDrawFunctionWidget *bdw{nullptr};
    void setup()
    {
        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawArrow(vg); });
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
            nvgStrokeColor(vg, nvgRGB(60, 60, 60));
            nvgFillColor(vg, nvgRGB(40, 40, 40));
            nvgFill(vg);
            nvgStroke(vg);
        }
        else
        {
            if (hovered)
                nvgFillColor(vg, nvgRGB(240, 240, 100));
            else
                nvgFillColor(vg, nvgRGB(190, 190, 190));
            nvgStrokeColor(vg, nvgRGB(220, 220, 220));
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
            // FIXME : move to a static helper somewhere
            auto n = AirwinRegistry::neighborIndexFor(module->selectedFX, dir);
            pushFXChange(module, n);
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

    BufferedDrawFunctionWidget *bdw{nullptr};
    void setup()
    {
        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawHelp(vg); });
        addChild(bdw);
    }
    void drawHelp(NVGcontext *vg)
    {
        auto fid = APP->window->loadFont(skinManager.fontPath)->handle;

        if (isOpen())
        {
            nvgBeginPath(vg);
            nvgFillColor(vg, nvgRGB(120, 120, 120));
            nvgFontFaceId(vg, fid);
            nvgFontSize(vg, 12);
            nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            nvgText(vg, box.size.x * 0.5, box.size.y * 0.5, "?", nullptr);
        }
        else
        {
            nvgBeginPath(vg);
            nvgFillColor(vg, nvgRGB(220, 220, 220));
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

    BufferedDrawFunctionWidget *bdw{nullptr};
    AWJog *leftJ{nullptr}, *rightJ{nullptr};
    static constexpr int jogButttonSize{12};
    void setup()
    {
        bdw = new BufferedDrawFunctionWidget(rack::Vec(0, 0), box.size,
                                             [this](auto vg) { drawSelector(vg); });
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

        auto fid = APP->window->loadFont(skinManager.fontPath)->handle;

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
        nvgFillColor(vg, nvgRGB(240, 240, 240));
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, fontSize);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.65, lastName.c_str(), nullptr);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(220, 220, 220));
        nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 8.5);
        nvgText(vg, box.size.x * 0.5, box.size.y * 0.22, lastCat.c_str(), nullptr);

        if (lastPoly)
        {
            nvgBeginPath(vg);
            nvgFillColor(vg, nvgRGB(140, 140, 140));
            nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);
            nvgFontFaceId(vg, fid);
            nvgFontSize(vg, 8.5);
            nvgText(vg, 3, 1, "poly", nullptr);
        }
    }

    AWSkin::Skin lastSkin{AWSkin::DARK};
    std::string lastName{"Airwindows"}, lastCat{"Multi-Effect"};
    bool lastPoly{false};
    void step() override
    {
        if (module && bdw)
        {
            if (lastName != module->selectedFX || lastCat != module->selectedCat ||
                lastSkin != skinManager.skin || lastPoly != module->polyphonic)
            {
                bdw->dirty = true;
            }
            lastPoly = module->polyphonic;
            lastName = module->selectedFX;
            lastCat = module->selectedCat;
            lastSkin = skinManager.skin;
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
        for (const auto &cat : AirwinRegistry::categories)
        {
            m->addChild(rack::createSubmenuItem(
                cat, "", [this, cat](auto *m) { createCategoryMenu(m, cat); }));
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
            auto mi = rack::createMenuItem(name, CHECKMARK(checked),
                                           [this, i = idx]() { pushFXChange(module, i); });
            mi->disabled = module->lockedType;
            m->addChild(mi);
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
    std::array<rack::ParamWidget *, M::maxParams> parKnobs, attenKnobs;
    std::array<AWPort *, M::maxParams> cvPorts;

    std::shared_ptr<rack::Svg> clipperSvg;
    BufferedDrawFunctionWidget *bg{nullptr};

    AW2RModuleWidget(M *m)
    {
        skinManager.intialize();
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 10, RACK_HEIGHT);

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

            cvPorts[i] = rack::createInputCentered<AWPort>(
                rack::Vec(bp + 42, pPos + dPP * 0.5), module, M::CV_0 + i);
            addInput(cvPorts[i]);

            pPos += dPP;
        }

        auto q = RACK_HEIGHT - 42;
        auto c1 = box.size.x * 0.25;
        auto dc = box.size.x * 0.11;
        auto c2 = box.size.x * 0.75;
        addInput(
            rack::createInputCentered<AWPort>(rack::Vec(c1 - dc, q), module, M::INPUT_L));
        addInput(
            rack::createInputCentered<AWPort>(rack::Vec(c1 + dc, q), module, M::INPUT_R));
        addOutput(rack::createOutputCentered<AWPort>(rack::Vec(c2 - dc, q), module,
                                                               M::OUTPUT_L));
        addOutput(rack::createOutputCentered<AWPort>(rack::Vec(c2 + dc, q), module,
                                                               M::OUTPUT_R));
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
    }

    struct HelpWidget : rack::Widget
    {
        double ctime;
        HelpWidget()
        {
            box.size = rack::Vec(400, 200);
            ctime = rack::system::getTime();
        }

        float pct = 0;
        void draw(const DrawArgs &args) override
        {
            auto vg = args.vg;
            int margin{3};
            nvgBeginPath(vg);
            nvgRect(vg, 0, 0, box.size.x, box.size.y);
            nvgFillColor(vg, nvgRGB(180, 180, 180));
            nvgFill(vg);

            nvgBeginPath(vg);
            nvgRect(vg, margin, margin, box.size.x - 2 * margin, box.size.y - 2 * margin);
            nvgFillColor(vg, nvgRGB(20, 20, 20));
            nvgFill(vg);

            int yp = 3;
            auto fid = APP->window->loadFont(skinManager.fontPath)->handle;
            auto fidm = APP->window->loadFont(skinManager.fontPathMedium)->handle;

            nvgBeginPath(vg);
            nvgFillColor(vg, nvgRGB(225, 225, 230));
            nvgTextAlign(vg, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);

            float bnd[6];
            nvgSave(vg);
            nvgScale(vg, APP->scene->rackScroll->getZoom(), APP->scene->rackScroll->getZoom());
            nvgFontSize(vg, 14);
            nvgFontFaceId(vg, fid);
            nvgScissor(vg, margin + 2, margin + 2,
                       box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                       box.size.y / APP->scene->rackScroll->getZoom() - 2 * (margin + 2));
            nvgTextBox(vg, margin + 2, margin + 2,
                       box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                       helpTitle.c_str(), nullptr);
            nvgTextBoxBounds(vg, margin + 2, margin + 2,
                             box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                             helpTitle.c_str(), nullptr, bnd);
            nvgRestore(vg);

            nvgSave(vg);
            nvgScale(vg, APP->scene->rackScroll->getZoom(), APP->scene->rackScroll->getZoom());
            nvgFontSize(vg, 10);
            nvgFontFaceId(vg, fidm);
            nvgScissor(vg, margin + 2, margin + 2,
                       box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                       box.size.y / APP->scene->rackScroll->getZoom() - 2 * (margin + 2));
            nvgTextBox(vg, margin + 2, bnd[3] / APP->scene->rackScroll->getZoom() + margin + 2,
                       box.size.x / APP->scene->rackScroll->getZoom() - 2 * (margin + 2),
                       helpText.c_str(), nullptr);
            nvgRestore(vg);
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
            catch (const rack::Exception &e)
            {
                helpText = "No Help Available for " + s;
            }

            resetBounds();
        }

        void resetBounds() {}

        float lastZoom{1.f};
        void step() override
        {
            if (APP->scene->rackScroll->getZoom() != lastZoom)
            {
                resetBounds();
            }
            lastZoom = APP->scene->rackScroll->getZoom();
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
                helpWidget->box.size.y = RACK_HEIGHT * APP->scene->rackScroll->getZoom();
                helpWidget->setFX(awm->selectedFX);
                helpShowing = true;
                APP->scene->addChild(helpWidget);
            }
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
        auto fid = APP->window->loadFont(skinManager.fontPath)->handle;
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(140, 140, 150));
        nvgFillColor(vg, nvgRGB(190, 190, 200));
        nvgStrokeWidth(vg, 1);
        nvgRoundedRect(vg, 4, box.size.y - cutPoint + 3, box.size.x * 0.5 - 8, 37, 2);
        nvgFill(vg);
        nvgStroke(vg);

        auto dc = box.size.x * 0.11;

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGB(40, 40, 50));
        nvgTextAlign(vg, NVG_ALIGN_BOTTOM | NVG_ALIGN_CENTER);
        nvgFontFaceId(vg, fid);
        nvgFontSize(vg, 10);
        nvgText(vg, box.size.x * 0.25, box.size.y - cutPoint + 38, "IN", nullptr);
        nvgText(vg, box.size.x * 0.25 - dc, box.size.y - cutPoint + 38, "L", nullptr);
        nvgText(vg, box.size.x * 0.25 + dc, box.size.y - cutPoint + 38, "R", nullptr);

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
        nvgText(vg, box.size.x * 0.75, box.size.y - cutPoint + 38, "OUT", nullptr);
        nvgText(vg, box.size.x * 0.75 - dc, box.size.y - cutPoint + 38, "L", nullptr);
        nvgText(vg, box.size.x * 0.75 + dc, box.size.y - cutPoint + 38, "R", nullptr);

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

        if (helpWidget)
        {
            helpWidget->box.pos = getAbsoluteOffset(rack::Vec(box.size.x, 0));
            helpWidget->box.size.y = RACK_HEIGHT * APP->scene->rackScroll->getZoom();
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