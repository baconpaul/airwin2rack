
#include <juce_gui_basics/juce_gui_basics.h>

#include "AWConsolidatedEditor.h"
#include "AirwinRegistry.h"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(awconsolidated_resources);
namespace awres = cmrc::awconsolidated_resources;

struct AWLookAndFeel : public juce::LookAndFeel_V4
{
    AWLookAndFeel()
    {
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour(10, 10, 15));
        setColour(juce::PopupMenu::textColourId, juce::Colours::white);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(60, 60, 65));
        setColour(juce::PopupMenu::highlightedTextColourId, juce::Colours::white);
        setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colour(120, 120, 125));

        auto fs = awres::get_filesystem();
        if (fs.is_file("res/PlusJakartaSans-Medium.ttf"))
        {
            auto f = fs.open("res/PlusJakartaSans-Medium.ttf");
            jakartaSansMedium = juce::Typeface::createSystemTypefaceFor(f.begin(), f.size());
        }
    }

    juce::Typeface::Ptr jakartaSansMedium;
    juce::Font getPopupMenuFont() override { return juce::Font(jakartaSansMedium).withHeight(16); }
};

struct Picker : public juce::Component
{
    Picker(AWConsolidatedAudioProcessorEditor *ed) : editor(ed)
    {
        setAccessible(true);
        setTitle("Select Airwindow");
        setDescription("Select Airwindow");
        setWantsKeyboardFocus(true);
    }
    void paint(juce::Graphics &g) override
    {
        int idx = editor->processor.curentProcessorIndex;
        auto &rg = AirwinRegistry::registry[idx];
        auto bounds = getLocalBounds().toFloat().reduced(2.f, 2.f);

        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(bounds, 5);
        g.setColour(juce::Colours::lightgrey);
        g.drawRoundedRectangle(bounds, 5, 1);
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(editor->jakartaSansSemi).withHeight(28));
        g.drawText(rg.name, bounds.reduced(8, 5), juce::Justification::centredBottom);

        g.setFont(juce::Font(editor->jakartaSansMedium).withHeight(18));
        g.drawText(rg.category, bounds.reduced(8, 3), juce::Justification::centredTop);

        auto p = juce::Path();

        auto jd = jogUp.reduced(3, 5);
        p.addTriangle(jd.getX(), jd.getY() + jd.getHeight(), jd.getX() + jd.getWidth(),
                      jd.getY() + jd.getHeight(), jd.getX() + 0.5 * jd.getWidth(), jd.getY());
        g.setColour(juce::Colour(90, 90, 95));
        g.fillPath(p);
        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(1));

        p = juce::Path();
        jd = jogDown.reduced(3, 5);
        p.addTriangle(jd.getX(), jd.getY(), jd.getX() + jd.getWidth(), jd.getY(),
                      jd.getX() + 0.5 * jd.getWidth(), jd.getY() + jd.getHeight());
        g.setColour(juce::Colour(90, 90, 95));
        g.fillPath(p);
        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(1));

        auto bx = getLocalBounds().reduced(8, 16);
        bx = bx.withWidth(bx.getHeight());
        auto r = bx.withHeight(bx.getHeight() / 5);
        for (int i = 0; i < 3; ++i)
        {
            auto q = r.reduced(1).toFloat();
            g.setColour(juce::Colour(90, 90, 95));
            g.fillRoundedRectangle(q, 1);
            g.setColour(juce::Colours::white);
            g.drawRoundedRectangle(q, 1, 1);

            r = r.translated(0, 2 * bx.getHeight() / 5);
        }
    }

    juce::Rectangle<int> jogUp, jogDown;
    void resized() override
    {
        auto b = getLocalBounds().reduced(8, 10);
        auto hh = b.getHeight() / 2;
        jogUp = b.withHeight(hh).withTrimmedLeft(b.getWidth() - hh);
        jogDown = jogUp.translated(0, hh);
    }

    bool keyPressed(const juce::KeyPress &p) override
    {
        if (p.getKeyCode() == juce::KeyPress::returnKey ||
            (p.getKeyCode() == juce::KeyPress::F10Key && p.getModifiers().isShiftDown()))
        {
            editor->showMenu();
            return true;
        }
        return false;
    }
    void mouseDown(const juce::MouseEvent &e) override
    {
        if (jogUp.toFloat().contains(e.position))
        {
            editor->jog(-1);
            startJogHold(-1);
        }
        else if (jogDown.toFloat().contains(e.position))
        {
            editor->jog(1);
            startJogHold(1);
        }
        else
        {
            editor->showMenu();
        }
    }
    void mouseUp(const juce::MouseEvent &e) override { stopJogHold(); }

    bool isJogHeld{false};
    void startJogHold(int dir)
    {
        isJogHeld = true;
        juce::Timer::callAfterDelay(800, [dir, w = juce::Component::SafePointer(this)]() {
            if (w)
            {
                w->doJogHold(dir);
            }
        });
    }

    void stopJogHold() { isJogHeld = false; }
    void doJogHold(int dir)
    {
        if (!isJogHeld)
            return;
        editor->jog(dir);
        juce::Timer::callAfterDelay(200, [dir, w = juce::Component::SafePointer(this)]() {
            if (w)
            {
                w->doJogHold(dir);
            }
        });
    }

    AWConsolidatedAudioProcessorEditor *editor{nullptr};
#if 0
    struct AH : public juce::AccessibilityHandler
    {
        struct AHV : public juce::AccessibilityValueInterface
        {
            explicit AHV(Picker *s) : comp(s) {}

            Picker *comp;

            bool isReadOnly() const override { return true; }
            double getCurrentValue() const override { return 0.; }

            void setValue(double) override {}
            void setValueAsString(const juce::String &newValue) override {}
            AccessibleValueRange getRange() const override { return {{0, 1}, 1}; }
            juce::String getCurrentValueAsString() const override
            {
                return fx_type_names[comp->editor->processor.getEffectType()];
            }

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AHV);
        };

        explicit AH(Picker *s)
            : comp(s),
              juce::AccessibilityHandler(*s, juce::AccessibilityRole::button,
                                         juce::AccessibilityActions()
                                             .addAction(juce::AccessibilityActionType::press,
                                                        [this]() { comp->editor->showMenu(); })
                                             .addAction(juce::AccessibilityActionType::showMenu,
                                                        [this]() { comp->editor->showMenu(); }),
                                         AccessibilityHandler::Interfaces{std::make_unique<AHV>(s)})
        {
        }

        Picker *comp;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AH);
    };

    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
    {
        return std::make_unique<AH>(this);
    }
#endif
};

struct DocPanel : juce::Component
{
    AWConsolidatedAudioProcessorEditor *editor{nullptr};
    DocPanel(AWConsolidatedAudioProcessorEditor *ed) : editor(ed) {}
    void rebuild()
    {
        auto r = juce::Rectangle<int>(0, 0, targetWidth, 10000);

        auto tFont = juce::Font(editor->jakartaSansSemi).withHeight(18);
        juce::GlyphArrangement gaTitle;
        gaTitle.addFittedText(tFont, editor->docHeader.substring(2), r.getX(), r.getY(),
                              r.getWidth(), r.getHeight(), juce::Justification::topLeft, 3);
        auto bounds = gaTitle.getBoundingBox(0, -1, true);

        auto q = r.translated(0, bounds.getHeight() + 8);

        auto bFont = juce::Font(editor->jakartaSansSemi).withHeight(13);
        juce::GlyphArrangement gaBody;
        gaBody.addFittedText(bFont, editor->docString.trim(), q.getX(), q.getY(), q.getWidth(),
                             q.getHeight(), juce::Justification::topLeft, 1000);
        auto bodyBounds = gaBody.getBoundingBox(0, -1, true);
        r = r.withHeight(bodyBounds.getBottom());

        setSize(r.getWidth(), r.getHeight());
    }

    float targetWidth{10};

    void paint(juce::Graphics &g)
    {
        g.setColour(juce::Colours::white);
        auto r = getLocalBounds();
        auto tFont = juce::Font(editor->jakartaSansSemi).withHeight(18);
        juce::GlyphArrangement gaTitle;
        gaTitle.addFittedText(tFont, editor->docHeader.substring(2), r.getX(), r.getY(),
                              r.getWidth(), r.getHeight(), juce::Justification::topLeft, 3);
        auto bounds = gaTitle.getBoundingBox(0, -1, true);
        r = r.translated(0, bounds.getHeight() + 4);
        gaTitle.draw(g);

        g.setColour(juce::Colour(120, 120, 125));
        g.drawLine(r.getX(), r.getY(), r.getX() + r.getWidth(), r.getY());
        r = r.translated(0, 4);

        g.setColour(juce::Colours::white.darker(0.3f));

        auto q = r;
        auto bFont = juce::Font(editor->jakartaSansMedium).withHeight(13);
        juce::GlyphArrangement gaBody;
        gaBody.addFittedText(bFont, editor->docString.trim(), q.getX(), q.getY(), q.getWidth(),
                             q.getHeight(), juce::Justification::topLeft, 1000);
        gaBody.draw(g);
    }
};

struct ParamKnob : juce::Component
{
    juce::AudioParameterFloat *weakParam{nullptr};
    const std::atomic<bool> &active;
    ParamKnob(const juce::String &cn, juce::AudioParameterFloat *param, const std::atomic<bool> &a)
        : juce::Component(cn), weakParam{param}, active{a}
    {
    }

    float getValue() const { return weakParam ? weakParam->get() : 0.f; }
    void setValue(float to)
    {
        if (weakParam)
            weakParam->setValueNotifyingHost(to);
    }

    void paint(juce::Graphics &g) override
    {
        auto knobHandle = getLocalBounds().reduced(4).toFloat();
        if (!active)
        {
            // g.setColour(juce::Colours::grey.withAlpha(0.3f));
            // g.fillEllipse(knobHandle.toFloat());
            return;
        }

        auto arc = [&](auto startV, auto endV) {
            float dPath = 0.2;
            float dAng = juce::MathConstants<float>::pi * (1 - dPath);
            float startA = dAng * (2 * startV - 1);
            float endA = dAng * (2 * endV - 1);

            auto region = knobHandle;
            auto p = juce::Path();
            p.startNewSubPath(region.getCentre().toFloat());
            p.addArc(region.getX(), region.getY(), region.getWidth(), region.getHeight(), startA,
                     endA, true);
            return p;
        };

        g.setColour(juce::Colour(60, 60, 65));
        g.fillEllipse(knobHandle.reduced(2));

        g.setColour(juce::Colour(140, 140, 150));
        g.strokePath(arc(-0.01f, 1.01f), juce::PathStrokeType(6));

        g.setColour(juce::Colour(0, 0, 0));
        g.strokePath(arc(0.f, 1.f), juce::PathStrokeType(4));

        g.setColour(juce::Colour(220, 220, 230));
        g.strokePath(arc(0.f, getValue()), juce::PathStrokeType(4));
    }

    juce::Point<float> mousePos;
    void mouseDown(const juce::MouseEvent &event) override
    {
        mousePos = event.position;
        weakParam->beginChangeGesture();
    }

    void mouseUp(const juce::MouseEvent &event) override { weakParam->endChangeGesture(); }

    void mouseDrag(const juce::MouseEvent &event) override
    {
        auto dy = -event.position.getY() + mousePos.getY();
        mousePos = event.position;

        float mul = 0.01;
        if (event.mods.isShiftDown())
            mul *= 0.1;

        auto nv = std::clamp(weakParam->get() + dy * mul, 0.f, 1.f);

        weakParam->setValueNotifyingHost(nv);
    }
};

struct ParamDisp : juce::Component
{
    juce::AudioParameterFloat *weakParam{nullptr};
    const std::atomic<bool> &active;
    bool isP0{false};
    AWConsolidatedAudioProcessorEditor *editor{nullptr};

    ParamDisp(const juce::String &cn, juce::AudioParameterFloat *param, const std::atomic<bool> &a,
              AWConsolidatedAudioProcessorEditor *ed)
        : juce::Component(cn), weakParam{param}, active(a), editor(ed)
    {
    }

    float getValue() const { return weakParam ? weakParam->get() : 0.f; }

    void paint(juce::Graphics &g) override
    {
        if (!active)
        {
            if (isP0)
            {
                auto b = getLocalBounds().withTrimmedRight(43);

                g.setColour(juce::Colours::white);
                g.setFont(juce::Font(editor->jakartaSansSemi).withHeight(20));
                g.drawText("No Parameters", b, juce::Justification::centredTop);
            }
            // g.setColour(juce::Colours::black.withAlpha(0.1f));
            // g.fillRoundedRectangle(getLocalBounds().toFloat(), 3);
            // g.setColour(juce::Colours::white.withAlpha(0.2f));
            // g.drawRoundedRectangle(getLocalBounds().toFloat(), 3, 1);
            return;
        }
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 3);
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 3, 1);

        auto bounds = getLocalBounds().reduced(5, 2);
        g.setFont(juce::Font(editor->firaMono).withHeight(18));
        g.drawText(weakParam->getCurrentValueAsText().trim(), bounds.withTrimmedBottom(2),
                   juce::Justification::bottomLeft);

        g.setFont(juce::Font(editor->jakartaSansMedium).withHeight(14));
        g.drawText(weakParam->getName(64), bounds, juce::Justification::topLeft);
    }
};
//==============================================================================
AWConsolidatedAudioProcessorEditor::AWConsolidatedAudioProcessorEditor(
    AWConsolidatedAudioProcessor &p)
    : AudioProcessorEditor(&p), processor(p)
{
    lnf = std::make_unique<AWLookAndFeel>();
    juce::LookAndFeel::setDefaultLookAndFeel(lnf.get());
    setAccessible(true);
    setFocusContainerType(juce::Component::FocusContainerType::keyboardFocusContainer);

    setSize(baseHeight, baseHeight);

    auto fs = awres::get_filesystem();
    try
    {
        if (fs.is_file("res/clipper.svg"))
        {
            auto f = fs.open("res/clipper.svg");
            clipperIcon = juce::Drawable::createFromImageData(f.begin(), f.size());
        }

        if (fs.is_file("res/PlusJakartaSans-Medium.ttf"))
        {
            auto f = fs.open("res/PlusJakartaSans-Medium.ttf");
            jakartaSansMedium = juce::Typeface::createSystemTypefaceFor(f.begin(), f.size());
        }

        if (fs.is_file("res/PlusJakartaSans-SemiBold.ttf"))
        {
            auto f = fs.open("res/PlusJakartaSans-SemiBold.ttf");
            jakartaSansSemi = juce::Typeface::createSystemTypefaceFor(f.begin(), f.size());
        }

        if (fs.is_file("res/FiraMono-Regular.ttf"))
        {
            auto f = fs.open("res/FiraMono-Regular.ttf");
            firaMono = juce::Typeface::createSystemTypefaceFor(f.begin(), f.size());
        }
    }
    catch (std::exception &e)
    {
    }

    auto margin{5};
    menuPicker = std::make_unique<Picker>(this);
    addAndMakeVisible(*menuPicker);
    menuPicker->setBounds(getLocalBounds().reduced(margin).withHeight(60));
    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000 / 60);

    auto sz{40};
    auto kb = getLocalBounds().withHeight(sz).withWidth(sz).translated(margin, 60 + 2 * margin);

    for (int i = 0; i < AWConsolidatedAudioProcessor::nAWParams; ++i)
    {
        auto sl = std::make_unique<ParamKnob>(juce::String("kb") + std::to_string(i),
                                              processor.fxParams[i], processor.active[i]);
        sl->setBounds(kb);
        addAndMakeVisible(*sl);

        knobs[i] = std::move(sl);

        auto lb = std::make_unique<ParamDisp>(juce::String("lb") + std::to_string(i),
                                              processor.fxParams[i], processor.active[i], this);
        lb->isP0 = (i == 0);
        lb->setBounds(kb.withWidth(180).translated(sz + margin, 0));
        addAndMakeVisible(*lb);
        labels[i] = std::move(lb);

        kb = kb.translated(0, sz + margin);
    }

    auto da = std::make_unique<DocPanel>(this);
    auto db = getLocalBounds()
                  .withTrimmedLeft(margin * 3 + sz + 180)
                  .withTrimmedRight(margin * 2)
                  .withTrimmedTop(60 + 2 * margin)
                  .withTrimmedBottom(40 + margin);

    da->targetWidth = db.getWidth() - 10;
    da->rebuild();
    docArea = std::move(da);

    docView = std::make_unique<juce::Viewport>();
    docView->setBounds(db);
    docView->setViewedComponent(docArea.get(), false);
    addAndMakeVisible(*docView);

    juce::PropertiesFile::Options options;
    options.applicationName = "AirwindowsConsolidated";
    options.folderName = "AirwindowsConsolidated";
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Preferences";
    properties = std::make_unique<juce::PropertiesFile>(options);
}

AWConsolidatedAudioProcessorEditor::~AWConsolidatedAudioProcessorEditor()
{
    idleTimer->stopTimer();
}

void AWConsolidatedAudioProcessorEditor::idle()
{
    if (processor.refreshUI.exchange(false))
    {
        docString = AirwinRegistry::documentationStringFor(processor.curentProcessorIndex);
        docHeader = docString.upToFirstOccurrenceOf("\n", false, false);
        docString = docString.fromFirstOccurrenceOf("\n", false, false).trim();
        if (docArea)
            docArea->rebuild();

        repaint();
    }
}

void AWConsolidatedAudioProcessorEditor::handleAsyncUpdate() {}

void AWConsolidatedAudioProcessorEditor::resized() {}

void AWConsolidatedAudioProcessorEditor::paint(juce::Graphics &g)
{
    auto b = getLocalBounds();
    auto gr = juce::ColourGradient(juce::Colour(20, 20, 25), {0.f, 0.f}, juce::Colour(50, 50, 55),
                                   {0.f, 1.f * getHeight()}, false);
    g.setGradientFill(gr);
    g.fillAll();

    static constexpr float footerHeight{40};
    auto fa = b.withHeight(footerHeight).withY(getHeight() - footerHeight);
    g.setColour(juce::Colour(160, 160, 170));
    g.fillRect(fa);
    g.setColour(juce::Colours::black);
    g.drawLine(fa.getX(), fa.getY(), fa.getX() + fa.getWidth(), fa.getY(), 1);
    g.setFont(juce::Font(jakartaSansSemi).withHeight(28));
    g.drawText("Airwindows", fa, juce::Justification::centred);

    g.setFont(juce::Font(jakartaSansMedium).withHeight(12));
    g.setColour(juce::Colour(110, 110, 115));

    g.drawText(std::string("Build : ") + __DATE__, fa.reduced(3), juce::Justification::bottomRight);

    if (clipperIcon)
    {
        auto ss = juce::Graphics::ScopedSaveState(g);
        g.addTransform(juce::AffineTransform().scaled(0.4).translated(10, 420));
        clipperIcon->draw(g, 0.6);
    }
}

void AWConsolidatedAudioProcessorEditor::jog(int dir)
{
    auto nx = AirwinRegistry::neighborIndexFor(processor.curentProcessorIndex, dir);
    processor.pushResetTypeFromUI(nx);
}

void AWConsolidatedAudioProcessorEditor::showMenu()
{
    auto p = juce::PopupMenu();
    auto ent = AirwinRegistry::registry[processor.curentProcessorIndex];

    p.addSectionHeader("Airwindows Consolidated");
    p.addSeparator();
    const auto *order = &AirwinRegistry::fxByCategory;

    bool isChrisOrder = properties->getValue("ordering") == "chris";
    if (isChrisOrder)
        order = &AirwinRegistry::fxByCategoryChrisOrder;

    for (const auto &[cat, set] : *order)
    {
        juce::PopupMenu sub;
        for (const auto &nm : set)
        {
            sub.addItem(nm, true, nm == ent.name, [nm, w = juce::Component::SafePointer(this)]() {
                if (w)
                    w->processor.pushResetTypeFromUI(AirwinRegistry::nameToIndex.at(nm));
            });
        }
        p.addSubMenu(cat, sub, true, nullptr, cat == ent.category);
    }

    p.addSeparator();

    auto settingsMenu = juce::PopupMenu();
    settingsMenu.addItem("Alphabetical Order Menus", true, !isChrisOrder,
                         [w = juce::Component::SafePointer(this)]() {
                             if (w)
                             {
                                 w->properties->setValue("ordering", "alpha");
                             }
                         });
    settingsMenu.addItem("Chris (Quality) Order Menus", true, isChrisOrder,
                         [w = juce::Component::SafePointer(this)]() {
                             if (w)
                             {
                                 w->properties->setValue("ordering", "chris");
                             }
                         });

    p.addSubMenu("Settings", settingsMenu);

    p.showMenuAsync(juce::PopupMenu::Options().withParentComponent(this));
}
