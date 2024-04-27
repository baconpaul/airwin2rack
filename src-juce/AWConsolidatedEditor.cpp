#include <algorithm>

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

    void drawPopupMenuBackgroundWithOptions(juce::Graphics &g, int width, int height,
                                            const juce::PopupMenu::Options &o) override
    {
        auto background = findColour(juce::PopupMenu::backgroundColourId);
        g.fillAll(background);

        g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.6f));
        g.drawRect(0, 0, width, height);
    }
};

struct Picker : public juce::Component
{
    struct Jog : public juce::Button
    {
        Picker *picker;
        int dir;
        Jog(Picker *p, int d)
            : juce::Button(juce::String("Jog ") + (d > 0 ? "Next" : "Previous")), picker(p), dir(d)
        {
            setAccessible(true);
        }
        void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override
        {
            auto p = juce::Path();
            auto jd = getLocalBounds().reduced(3, 5);
            if (dir == -1)
            {
                p.addTriangle(jd.getX(), jd.getY() + jd.getHeight(), jd.getX() + jd.getWidth(),
                              jd.getY() + jd.getHeight(), jd.getX() + 0.5 * jd.getWidth(),
                              jd.getY());
            }
            else
            {
                p.addTriangle(jd.getX(), jd.getY(), jd.getX() + jd.getWidth(), jd.getY(),
                              jd.getX() + 0.5 * jd.getWidth(), jd.getY() + jd.getHeight());
            }
            if (isHovered)
                g.setColour(juce::Colour(160, 160, 165));
            else
                g.setColour(juce::Colour(90, 90, 95));
            g.fillPath(p);
            g.setColour(juce::Colours::white);
            g.strokePath(p, juce::PathStrokeType(1));
        }

        bool isHovered{false};
        void mouseEnter(const juce::MouseEvent &) override
        {
            isHovered = true;
            repaint();
        }
        void mouseExit(const juce::MouseEvent &) override
        {
            isHovered = false;
            repaint();
        }

        void mouseDown(const juce::MouseEvent &) override { picker->doJog(dir); }
        void mouseUp(const juce::MouseEvent &) override { picker->stopJogHold(); }

        bool keyPressed(const juce::KeyPress &p) override
        {
            if (p.getKeyCode() == juce::KeyPress::returnKey)
            {
                // dojog does long hold stuff so go direct
                picker->editor->jog(dir);
                return true;
            }
            return false;
        }
    };
    std::unique_ptr<Jog> up, down;

    struct Hamburger : juce::Button
    {
        Picker *picker;
        Hamburger(Picker *p) : juce::Button("Main Menu"), picker(p) { setAccessible(true); }

        void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override
        {
            auto r = getLocalBounds().withHeight(getHeight() / 5);
            for (int i = 0; i < 3; ++i)
            {
                auto q = r.reduced(1).toFloat();
                if (isHovered)
                    g.setColour(juce::Colour(160, 160, 165));
                else
                    g.setColour(juce::Colour(90, 90, 95));
                g.fillRoundedRectangle(q, 1);
                g.setColour(juce::Colours::white);
                g.drawRoundedRectangle(q, 1, 1);

                r = r.translated(0, 2 * getHeight() / 5);
            }
        }

        bool isHovered{false};
        void mouseEnter(const juce::MouseEvent &) override
        {
            isHovered = true;
            repaint();
        }
        void mouseExit(const juce::MouseEvent &) override
        {
            isHovered = false;
            repaint();
        }
        void mouseDown(const juce::MouseEvent &) override { picker->editor->showMenu(); }

        bool keyPressed(const juce::KeyPress &p) override
        {
            if (p.getKeyCode() == juce::KeyPress::returnKey ||
                (p.getKeyCode() == juce::KeyPress::F10Key && p.getModifiers().isShiftDown()))
            {
                // dojog does long hold stuff so go direct
                picker->editor->showMenu();
                return true;
            }
            return false;
        }
    };
    std::unique_ptr<Hamburger> hamburger;

    Picker(AWConsolidatedAudioProcessorEditor *ed) : editor(ed)
    {
        setAccessible(true);
        setTitle("Select Airwindow");
        setDescription("Select Airwindow");
        setWantsKeyboardFocus(true);

        up = std::make_unique<Jog>(this, -1);
        down = std::make_unique<Jog>(this, 1);
        hamburger = std::make_unique<Hamburger>(this);

        addAndMakeVisible(*up);
        addAndMakeVisible(*down);
        addAndMakeVisible(*hamburger);
    }
    juce::Rectangle<float> titleBox;

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
        auto ga = juce::GlyphArrangement();
        auto tbx = bounds.reduced(8, 5);
        ga.addFittedText(juce::Font(editor->jakartaSansSemi).withHeight(28), rg.name, tbx.getX(),
                         tbx.getY(), tbx.getWidth(), tbx.getHeight(),
                         juce::Justification::centredBottom, 1);
        titleBox = ga.getBoundingBox(0, -1, true);

        auto catString = rg.category;
        g.setFont(juce::Font(editor->jakartaSansMedium).withHeight(18));
        g.drawText(catString, bounds.reduced(8, 3), juce::Justification::centredTop);
    }

    juce::Rectangle<int> jogUp, jogDown;
    void resized() override
    {
        auto b = getLocalBounds().reduced(8, 10);
        auto hh = b.getHeight() / 2;
        jogUp = b.withHeight(hh).withTrimmedLeft(b.getWidth() - hh);
        jogDown = jogUp.translated(0, hh);

        up->setBounds(jogUp);
        down->setBounds(jogDown);

        auto bx = getLocalBounds().reduced(8, 16);
        bx = bx.withWidth(bx.getHeight());
        hamburger->setBounds(bx);
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
    void doJog(int dir)
    {
        editor->jog(dir);
        startJogHold(dir);
    }

    void mouseDown(const juce::MouseEvent &e) override
    {
        if (titleBox.toFloat().contains(e.position))
        {
            std::cout << "Starting edit gesture" << std::endl;
        }
        else
        {
            editor->showMenu();
        }
    }

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

    void rebuild()
    {
        int idx = editor->processor.curentProcessorIndex;
        auto &rg = AirwinRegistry::registry[idx];

        setTitle(rg.name + " (" + rg.category + ")");
        if (getAccessibilityHandler())
            getAccessibilityHandler()->notifyAccessibilityEvent(
                juce::AccessibilityEvent::titleChanged);

        up->setTitle("Select Previous from " + rg.name);
        if (up->getAccessibilityHandler())
            up->getAccessibilityHandler()->notifyAccessibilityEvent(
                juce::AccessibilityEvent::titleChanged);
        down->setTitle("Select Next from " + rg.name);
        if (down->getAccessibilityHandler())
            down->getAccessibilityHandler()->notifyAccessibilityEvent(
                juce::AccessibilityEvent::titleChanged);
    }
    AWConsolidatedAudioProcessorEditor *editor{nullptr};
};

struct AWLink : public juce::Component
{
    juce::Typeface::Ptr ft;
    AWLink(juce::Typeface::Ptr f) : ft(f) {}
    void paint(juce::Graphics &g) override
    {
        g.setColour(juce::Colours::black);
        if (isHovered)
            g.setColour(juce::Colour(30, 30, 120));
        g.setFont(juce::Font(ft).withHeight(28));
        g.drawText("Airwindows", getLocalBounds(), juce::Justification::centred);
    }

    void mouseDown(const juce::MouseEvent &) override
    {
        auto url = juce::URL("https://airwindows.com");
        url.launchInDefaultBrowser();
    }

    bool isHovered{false};
    void mouseEnter(const juce::MouseEvent &) override
    {
        isHovered = true;
        repaint();
    }
    void mouseExit(const juce::MouseEvent &) override
    {
        isHovered = false;
        repaint();
    }
};

struct ParamKnob : juce::Component
{
    juce::AudioParameterFloat *weakParam{nullptr};
    const std::atomic<bool> &active;
    ParamKnob(const juce::String &cn, juce::AudioParameterFloat *param, const std::atomic<bool> &a)
        : juce::Component(cn), weakParam{param}, active{a}
    {
        refreshModel();
    }

    void refreshModel()
    {
        setAccessible(active);
        setWantsKeyboardFocus(active);
        if (active && weakParam)
            setTitle(weakParam->getName(64));
        if (getAccessibilityHandler())
        {
            getAccessibilityHandler()->notifyAccessibilityEvent(
                juce::AccessibilityEvent::valueChanged);
            getAccessibilityHandler()->notifyAccessibilityEvent(
                juce::AccessibilityEvent::textChanged);
        }
    }

    float getValue() const { return weakParam ? weakParam->get() : 0.f; }
    void setValue(float to)
    {
        if (weakParam)
            weakParam->setValueNotifyingHost(to);
        if (getAccessibilityHandler())
            getAccessibilityHandler()->notifyAccessibilityEvent(
                juce::AccessibilityEvent::valueChanged);
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

        if (isHovered)
            g.setColour(juce::Colour(75, 75, 80));
        else
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

    bool isHovered{false};
    void mouseEnter(const juce::MouseEvent &) override
    {
        isHovered = true;
        repaint();
    }
    void mouseExit(const juce::MouseEvent &) override
    {
        isHovered = false;
        repaint();
    }

    struct AHValue : public juce::AccessibilityValueInterface
    {
        explicit AHValue(ParamKnob *s) : slider(s) {}

        ParamKnob *slider;

        bool isReadOnly() const override { return false; }
        double getCurrentValue() const override { return slider->getValue(); }
        void setValue(double newValue) override
        {
            slider->setValue(newValue);

            slider->repaint();
        }
        virtual juce::String getCurrentValueAsString() const override
        {
            if (slider->weakParam)
            {
                auto res = slider->weakParam->getCurrentValueAsText();
                return res;
            }
            return "";
        }
        void setValueAsString(const juce::String &) override {}

        AccessibleValueRange getRange() const override { return {{0, 1}, 0.01}; }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AHValue);
    };
    struct AH : juce::AccessibilityHandler
    {
        ParamKnob *slider{nullptr};
        AH(ParamKnob *c)
            : juce::AccessibilityHandler(
                  *c, juce::AccessibilityRole::slider, juce::AccessibilityActions(),
                  AccessibilityHandler::Interfaces{std::make_unique<AHValue>(c)}),
              slider(c)
        {
        }

        juce::String getTitle() const override { return slider->weakParam->getName(64); }
    };

    bool keyPressed(const juce::KeyPress &key) override
    {
        float amt = 0.05;
        if (key.getModifiers().isShiftDown())
            amt = 0.01;
        if (key.getKeyCode() == juce::KeyPress::upKey)
        {
            setValue(std::clamp((double)getValue() + amt, 0., 1.));
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::downKey)
        {
            setValue(std::clamp((double)getValue() - amt, 0., 1.));
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::homeKey)
        {
            setValue(1.);
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::endKey)
        {
            setValue(0.);
            return true;
        }
        return false;
    }

    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
    {
        return std::make_unique<AH>(this);
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
    setWantsKeyboardFocus(true);

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

    docAreaRect = getLocalBounds()
                      .withTrimmedLeft(margin * 3 + sz + 180)
                      .withTrimmedRight(margin * 2)
                      .withTrimmedTop(60 + 2 * margin)
                      .withTrimmedBottom(40 + margin);

    docBodyLabel = std::make_unique<juce::Label>("Documentation Header");
    docBodyLabel->setAccessible(true);
    docBodyLabel->setWantsKeyboardFocus(true);
    docBodyLabel->setFont(juce::Font(jakartaSansMedium).withHeight(18));
    docBodyLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    docBodyLabel->setTitle("Documentation Header");
    addAndMakeVisible(*docBodyLabel);

    docBodyEd = std::make_unique<juce::TextEditor>("Documentation");
    docBodyEd->setMultiLine(true);
    docBodyEd->setFont(juce::Font(jakartaSansMedium).withHeight(15));
    docBodyEd->setReadOnly(false);
    docBodyEd->setWantsKeyboardFocus(true);
    docBodyEd->setCaretVisible(true);
    docBodyEd->setAccessible(true);
    docBodyEd->setTitle("Documentation");
    docBodyEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                         juce::Colours::black.withAlpha(0.f));
    docBodyEd->setColour(juce::TextEditor::ColourIds::outlineColourId,
                         juce::Colours::black.withAlpha(0.f));
    docBodyEd->setColour(juce::TextEditor::ColourIds::focusedOutlineColourId,
                         juce::Colours::black.withAlpha(0.f));
    docBodyEd->setColour(juce::TextEditor::ColourIds::textColourId,
                         juce::Colours::white.darker(0.2f));
    addAndMakeVisible(*docBodyEd);
    awTag = std::make_unique<AWLink>(jakartaSansSemi);
    auto fa = getLocalBounds()
                  .withHeight(40)
                  .withY(getHeight() - 40)
                  .withTrimmedLeft(100)
                  .withTrimmedRight(100);
    awTag->setBounds(fa);
    addAndMakeVisible(*awTag);

    accessibleOrderWeakRefs.push_back(menuPicker.get());
    accessibleOrderWeakRefs.push_back(menuPicker->hamburger.get());
    accessibleOrderWeakRefs.push_back(menuPicker->up.get());
    accessibleOrderWeakRefs.push_back(menuPicker->down.get());
    for (auto &k : knobs)
        accessibleOrderWeakRefs.push_back(k.get());
    accessibleOrderWeakRefs.push_back(docBodyLabel.get());
    accessibleOrderWeakRefs.push_back(docBodyEd.get());

    resizeDocArea();

    juce::PropertiesFile::Options options;
    options.applicationName = "AirwindowsConsolidated";
    options.folderName = "AirwindowsConsolidated";
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Preferences";
    properties = std::make_unique<juce::PropertiesFile>(options);

    auto isRO = properties->getBoolValue("editorIsReadOnly", true);
    docBodyEd->setReadOnly(isRO);
}

AWConsolidatedAudioProcessorEditor::~AWConsolidatedAudioProcessorEditor()
{
    idleTimer->stopTimer();
}

void AWConsolidatedAudioProcessorEditor::idle()
{
    if (processor.rebuildUI.exchange(false))
    {
        resizeDocArea();

        for (auto &k : knobs)
            k->refreshModel();

        menuPicker->rebuild();
        repaint();
    }

    if (processor.refreshUI.exchange(false))
    {
        repaint();
    }
}

void AWConsolidatedAudioProcessorEditor::resizeDocArea()
{
    docString = AirwinRegistry::documentationStringFor(processor.curentProcessorIndex);
    docHeader = docString.upToFirstOccurrenceOf("\n", false, false);
    docString = docString.fromFirstOccurrenceOf("\n", false, false).trim();

    auto r = docAreaRect;
    auto tFont = juce::Font(jakartaSansSemi).withHeight(18);
    juce::GlyphArrangement gaTitle;
    gaTitle.addFittedText(tFont, docHeader.substring(2), r.getX(), r.getY(), r.getWidth(),
                          r.getHeight(), juce::Justification::topLeft, 3);
    auto bounds = gaTitle.getBoundingBox(0, -1, true);

    docBodyLabel->setBounds(r.withHeight(bounds.getHeight() + 4));
    docBodyLabel->setText(docHeader.substring(2), juce::NotificationType::dontSendNotification);
    if (docBodyLabel->getAccessibilityHandler())
        docBodyLabel->getAccessibilityHandler()->notifyAccessibilityEvent(
            juce::AccessibilityEvent::valueChanged);

    auto q = r.withTrimmedTop(bounds.getHeight() + 8);

    docBodyEd->setBounds(q);
    docBodyEd->setText(docString, false);
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
    auto coll = properties->getValue("collection", "All");

    if (coll == "All")
    {
        auto nx = AirwinRegistry::neighborIndexFor(processor.curentProcessorIndex, dir);
        processor.pushResetTypeFromUI(nx);
    }
    else
    {
        int sidx = processor.curentProcessorIndex;
        auto nx = AirwinRegistry::neighborIndexFor(processor.curentProcessorIndex, dir);
        while (nx != sidx)
        {
            auto rg = AirwinRegistry::registry[nx];
            for (auto c : rg.collections)
            {
                if (c == coll)
                {
                    processor.pushResetTypeFromUI(nx);
                    return;
                }
            }

            nx = AirwinRegistry::neighborIndexFor(nx, dir);
        }
    }
}

void AWConsolidatedAudioProcessorEditor::showMenu()
{
    auto p = juce::PopupMenu();
    auto ent = AirwinRegistry::registry[processor.curentProcessorIndex];

    p.addSectionHeader("Airwindows Consolidated");
    p.addSeparator();
    auto collMenu = juce::PopupMenu();
    std::set<std::string> colls;
    for (auto &rg : AirwinRegistry::registry)
    {
        for (auto s : rg.collections)
        {
            colls.insert(s);
        }
    }
    for (auto c : colls)
    {
        collMenu.addItem(c, [c, w = juce::Component::SafePointer(this)]() {
            if (!w)
                return;
            w->properties->setValue("collection", juce::String(c));
        });
    }
    collMenu.addSeparator();
    collMenu.addItem("All Plugins", [w = juce::Component::SafePointer(this)]() {
        if (!w)
            return;
        w->properties->setValue("collection", juce::String("All"));
    });
    p.addSubMenu("Filter by Collection", collMenu);
    p.addSeparator();

    const auto *order = &AirwinRegistry::fxByCategory;

    bool isChrisOrder = properties->getValue("ordering") == "chris";
    if (isChrisOrder)
        order = &AirwinRegistry::fxByCategoryChrisOrder;

    auto coll = properties->getValue("collection", "All");

    for (const auto &[cat, set] : *order)
    {
        juce::PopupMenu sub;
        for (const auto &nm : set)
        {
            bool include{false};
            if (coll == "All")
            {
                include = true;
            }
            else
            {
                auto rg = AirwinRegistry::registry[AirwinRegistry::nameToIndex.at(nm)];
                for (auto c : rg.collections)
                {
                    if (c == coll)
                        include = true;
                }
            }
            if (include)
                sub.addItem(nm, true, nm == ent.name, [nm, w = juce::Component::SafePointer(this)]() {
                    if (w)
                        w->processor.pushResetTypeFromUI(AirwinRegistry::nameToIndex.at(nm));
                });
        }
        if (sub.getNumItems() > 0)
            p.addSubMenu(cat, sub, true, nullptr, cat == ent.category);
    }

    p.addSeparator();

    p.addItem("Visit Airwindows.com",
              []() { juce::URL("https://www.airwindows.com").launchInDefaultBrowser(); });

    auto settingsMenu = juce::PopupMenu();
    auto isRO = properties->getBoolValue("editorIsReadOnly");
    settingsMenu.addItem(isRO ? "Make Documentation Editable for Screen Readers"
                              : "Make Documentation Read-Only",
                         [isRO, w = juce::Component::SafePointer(this)]() {
                             w->properties->setValue("editorIsReadOnly", !isRO);
                             w->docBodyEd->setReadOnly(!isRO);
                         });

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

    p.showMenuAsync(juce::PopupMenu::Options().withMaximumNumColumns(1));
}

struct FxFocusTrav : public juce::ComponentTraverser
{
    FxFocusTrav(AWConsolidatedAudioProcessorEditor *ed) : editor(ed) {}
    juce::Component *getDefaultComponent(juce::Component *parentComponent) override
    {
        return editor->menuPicker.get();
    }
    juce::Component *searchDir(juce::Component *from, int dir)
    {
        const auto iter = std::find(editor->accessibleOrderWeakRefs.cbegin(),
                                    editor->accessibleOrderWeakRefs.cend(), from);
        if (iter == editor->accessibleOrderWeakRefs.cend())
        {
            return nullptr;
        }

        switch (dir)
        {
        case 1:
        {
            auto res = std::next(iter);

            while (res != editor->accessibleOrderWeakRefs.cend() && !(*res)->isAccessible())
            {
                res = std::next(res);
            }
            if (res != editor->accessibleOrderWeakRefs.cend())
            {
                return *res;
            }
        }
        break;
        case -1:
        {
            auto res = iter;

            while (res != editor->accessibleOrderWeakRefs.cbegin() &&
                   !(*std::prev(res))->isAccessible())
            {
                res = std::prev(res);
            }
            if (res != editor->accessibleOrderWeakRefs.cbegin())
                return *std::prev(res);
        }
        break;
        }
        return nullptr;
    }
    juce::Component *getNextComponent(juce::Component *current) override
    {
        return searchDir(current, 1);
    }
    juce::Component *getPreviousComponent(juce::Component *current) override
    {
        return searchDir(current, -1);
    }
    std::vector<juce::Component *> getAllComponents(juce::Component *parentComponent) override
    {
        return editor->accessibleOrderWeakRefs;
    }
    AWConsolidatedAudioProcessorEditor *editor{nullptr};
};

bool AWConsolidatedAudioProcessorEditor::keyPressed(const juce::KeyPress &k)
{
    if (k.getKeyCode() == juce::KeyPress::F7Key)
    {
        auto isRO = properties->getBoolValue("editorIsReadOnly");

        properties->setValue("editorIsReadOnly", !isRO);
        docBodyEd->setReadOnly(!isRO);
    }
    return false;
}

std::unique_ptr<juce::ComponentTraverser>
AWConsolidatedAudioProcessorEditor::createKeyboardFocusTraverser()
{
    return std::make_unique<FxFocusTrav>(this);
}
