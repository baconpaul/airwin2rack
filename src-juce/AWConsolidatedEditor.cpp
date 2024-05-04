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

struct Picker : public juce::Component, public juce::TextEditor::Listener
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

        typeinEd = std::make_unique<juce::TextEditor>("Typeahead");
        typeinEd->setFont(juce::Font(editor->jakartaSansMedium).withHeight(28));
        typeinEd->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::white);
        typeinEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                            juce::Colour(10, 10, 15));
        typeinEd->addListener(this);
        addChildComponent(*typeinEd);
    }
    ~Picker()
    {
        if (listBox)
            listBox->setModel(nullptr);
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
        if (!typeinEd->isVisible())
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

        auto bd = getLocalBounds().reduced(90, 0).withHeight(32).translated(0, getHeight() - 38);
        typeinEd->setBounds(bd);
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
            int idx = editor->processor.curentProcessorIndex;
            auto &rg = AirwinRegistry::registry[idx];

            typeinEd->setVisible(true);
            typeinEd->grabKeyboardFocus();
            typeinEd->selectAll();
            typeinEd->setText(rg.name, juce::NotificationType::dontSendNotification);

            if (!listBox)
            {
                listBoxModel = std::make_unique<TALBM>(this);
                listBox = std::make_unique<juce::ListBox>();
                listBox->setModel(listBoxModel.get());
                listBox->setRowHeight(40);
                listBox->setColour(juce::ListBox::backgroundColourId, juce::Colour(10, 10, 20));
                listBox->setColour(juce::ListBox::outlineColourId, juce::Colour(120, 120, 125));

                getParentComponent()->addAndMakeVisible(*listBox);
            }

            listBox->setVisible(true);
            auto teb = typeinEd->getBounds();
            teb.setX(teb.getX() + getBounds().getX());
            teb.setY(teb.getY() + getBounds().getY() + teb.getHeight());
            teb = teb.withHeight(400);
            listBox->setBounds(teb);

            populateForTypein();
        }
        else
        {
            editor->showMenu();
        }
    }

    uint64_t jogHoldCounter{0};
    void startJogHold(int dir)
    {
        juce::Timer::callAfterDelay(800, [hc = jogHoldCounter, dir, w = juce::Component::SafePointer(this)]() {
            if (w)
            {
                w->doJogHold(dir, hc);
            }
        });
    }

    void stopJogHold() { jogHoldCounter ++; }
    void doJogHold(int dir, uint64_t hc)
    {
        if (hc != jogHoldCounter)
        {
            return;
        }
        editor->jog(dir);
        juce::Timer::callAfterDelay(200, [hc, dir, w = juce::Component::SafePointer(this)]() {
            if (w)
            {
                w->doJogHold(dir, hc);
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

        typeinEd->clear();
        typeinEd->setText(rg.name, juce::NotificationType::dontSendNotification);
        typeinEd->setJustification(juce::Justification::centred);
    }
    AWConsolidatedAudioProcessorEditor *editor{nullptr};

    std::unique_ptr<juce::TextEditor> typeinEd;
    std::unique_ptr<juce::ListBox> listBox;

    void populateForTypein()
    {
        if (!listBoxModel)
            return;
        std::vector<int> rr;
        auto m = typeinEd->getText().toLowerCase().toStdString();
        auto coll = editor->getCurrentCollection();
        std::unordered_set<std::string> inCol;
        if (AirwinRegistry::namesByCollection.find(coll) != AirwinRegistry::namesByCollection.end())
            inCol = AirwinRegistry::namesByCollection.at(coll);

        for (auto r : AirwinRegistry::fxAlphaOrdering)
        {
            auto n = AirwinRegistry::registry[r].name;
            std::transform(n.begin(), n.end(), n.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            auto cat = AirwinRegistry::registry[r].category;
            std::transform(cat.begin(), cat.end(), cat.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            if (n.find(m) != std::string::npos || cat.find(m) != std::string::npos)
            {
                if (coll == editor->allCollection || inCol.empty())
                {
                    rr.push_back(r);
                }
                else
                {
                    if (inCol.find(AirwinRegistry::registry[r].name) != inCol.end())
                    {
                        rr.push_back(r);
                    }
                }
            }
            if (rr.size() > 30)
                break;
        }
        listBoxModel->entries = rr;
        listBox->updateContent();
    }

    void dismissTE()
    {
        typeinEd->setVisible(false);
        listBox->setVisible(false);
    }

    void selectTE(int which)
    {
        editor->processor.pushResetTypeFromUI(which);
        dismissTE();
    }

    void textEditorTextChanged(juce::TextEditor &editor) override { populateForTypein(); }

    void textEditorReturnKeyPressed(juce::TextEditor &editor) override
    {
        if (listBoxModel->entries.empty())
            dismissTE();
        selectTE(listBoxModel->entries[0]);
    }

    void textEditorEscapeKeyPressed(juce::TextEditor &editor) override { dismissTE(); }

    void textEditorFocusLost(juce::TextEditor &editor) override { dismissTE(); }

    struct TALBM : juce::ListBoxModel
    {
        std::vector<int> entries;

        Picker *picker{nullptr};
        TALBM(Picker *p) : picker(p) {}
        int getNumRows() override { return entries.size(); }
        void paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height,
                              bool rowIsSelected) override
        {
            if (rowNumber < 0 || rowNumber >= (int)entries.size())
                return;

            auto bx = juce::Rectangle<int>(0, 0, width, height).reduced(2);

            auto &rg = AirwinRegistry::registry[entries[rowNumber]];

            g.setFont(juce::Font(picker->editor->jakartaSansSemi).withHeight(22));
            g.setColour(juce::Colours::white);
            g.drawText(rg.name, bx, juce::Justification::bottomLeft);

            g.setColour(juce::Colours::white.darker(0.2));

            g.setFont(juce::Font(picker->editor->jakartaSansMedium).withHeight(14));
            g.drawText(rg.category, bx, juce::Justification::topLeft);

            g.setFont(juce::Font(picker->editor->jakartaSansMedium).withHeight(14));
            g.drawFittedText(rg.whatText, bx.withTrimmedLeft(bx.getWidth() / 2),
                             juce::Justification::bottomRight, 3);

            g.setColour(juce::Colour(90, 90, 95));
            g.drawLine(5, height, width - 5, height, 1);
        }

        void listBoxItemClicked(int row, const juce::MouseEvent &event) override
        {
            if (row < 0 || row >= (int)entries.size())
                return;

            picker->selectTE(entries[row]);
        }
    };
    std::unique_ptr<TALBM> listBoxModel;
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

struct ParamDisp : juce::Component, juce::TextEditor::Listener
{
    AWConsolidatedAudioProcessor::AWParam *weakParam{nullptr};
    const std::atomic<bool> &active;
    bool isP0{false};
    int index{0};
    AWConsolidatedAudioProcessorEditor *editor{nullptr};

    ParamDisp(const juce::String &cn, AWConsolidatedAudioProcessor::AWParam *param,
              const std::atomic<bool> &a, int idx, AWConsolidatedAudioProcessorEditor *ed)
        : juce::Component(cn), weakParam{param}, active(a), index(idx), editor(ed)
    {
        typeinEd = std::make_unique<juce::TextEditor>("Editor");

        typeinEd->setFont(juce::Font(editor->firaMono).withHeight(18));
        typeinEd->setColour(juce::TextEditor::ColourIds::textColourId, juce::Colours::white);
        typeinEd->setColour(juce::TextEditor::ColourIds::outlineColourId,
                            juce::Colours::white.withAlpha(0.2f));
        typeinEd->setColour(juce::TextEditor::ColourIds::focusedOutlineColourId,
                            juce::Colours::white.withAlpha(0.2f));
        typeinEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                            juce::Colour(10, 10, 15));
        typeinEd->addListener(this);

        addChildComponent(*typeinEd);
    }

    float getValue() const { return weakParam ? weakParam->get() : 0.f; }

    std::unique_ptr<juce::TextEditor> typeinEd;

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(5, 2);
        typeinEd->setBounds(
            bounds.withTrimmedTop(bounds.getHeight() - 24).expanded(1).translated(-3, 0));
    }

    void showEd()
    {
        bool go{false};
        {
            std::lock_guard<std::mutex> g(editor->processor.displayProcessorMutex);
            go = editor->processor.awDisplayProcessor->canConvertParameterTextToValue(index);
        }

        if (!go)
            return;

        typeinEd->setVisible(true);
        typeinEd->setText(weakParam->getCurrentValueAsText().trim(),
                          juce::NotificationType::dontSendNotification);
        typeinEd->selectAll();
        typeinEd->grabKeyboardFocus();
    }
    void dismissEd();

    void refreshModel() {
        typeinEd->setTitle("Edit " + weakParam->getName(64));
        dismissEd();
    }

    void textEditorReturnKeyPressed(juce::TextEditor &ed) override
    {
        if (ed.getText().trim().isEmpty())
        {
            if (weakParam)
                weakParam->setValueNotifyingHost(weakParam->getDefaultValue());
            if (getAccessibilityHandler())
                getAccessibilityHandler()->notifyAccessibilityEvent(
                    juce::AccessibilityEvent::valueChanged);
        }
        else
        {
            float f{0};
            bool worked{false};
            {
                std::lock_guard<std::mutex> g(editor->processor.displayProcessorMutex);
                worked = editor->processor.awDisplayProcessor->parameterTextToValue(
                    index, ed.getText().toRawUTF8(), f);
            }

            if (worked)
            {
                if (weakParam)
                    weakParam->setValueNotifyingHost(f);
                if (getAccessibilityHandler())
                    getAccessibilityHandler()->notifyAccessibilityEvent(
                        juce::AccessibilityEvent::valueChanged);
            }
        }

        dismissEd();
    }
    void textEditorEscapeKeyPressed(juce::TextEditor &editor) override { dismissEd(); }
    void textEditorFocusLost(juce::TextEditor &editor) override { dismissEd(); }

    void mouseDown(const juce::MouseEvent &e) override
    {
        if (active)
            showEd();
    }

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

struct ParamKnob : juce::Component
{
    AWConsolidatedAudioProcessor::AWParam *weakParam{nullptr};
    const std::atomic<bool> &active;
    AWConsolidatedAudioProcessorEditor *editor{nullptr};
    int index{0};
    ParamKnob(const juce::String &cn, AWConsolidatedAudioProcessor::AWParam *param,
              const std::atomic<bool> &a, AWConsolidatedAudioProcessorEditor *ed, int idx)
        : juce::Component(cn), weakParam{param}, active{a}, editor{ed}, index{idx}
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

        if (key.getKeyCode() == juce::KeyPress::deleteKey && weakParam)
        {
            setValue(weakParam->getDefaultValue());
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::F10Key && key.getModifiers().isShiftDown())
        {
            editor->labels[index]->showEd();
            return true;
        }
        return false;
    }

    void mouseDoubleClick(const juce::MouseEvent &event) override
    {
        if (weakParam)
        {
            setValue(weakParam->getDefaultValue());
        }
    }

    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
    {
        return std::make_unique<AH>(this);
    }
};

void ParamDisp::dismissEd()
{
    typeinEd->setVisible(false);
    editor->knobs[index]->grabKeyboardFocus();
}

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
                                              processor.fxParams[i], processor.active[i], this, i);
        sl->setBounds(kb);
        addAndMakeVisible(*sl);

        knobs[i] = std::move(sl);

        auto lb = std::make_unique<ParamDisp>(juce::String("lb") + std::to_string(i),
                                              processor.fxParams[i], processor.active[i], i, this);
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

        for (auto &k : labels)
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
    // use a slightly narrower box to force an extra line to simulate the label insets
    gaTitle.addFittedText(tFont, docHeader.substring(2), r.getX() + 5, r.getY(), r.getWidth() - 10,
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
    auto coll = getCurrentCollection();

    if (coll == allCollection ||
        AirwinRegistry::namesByCollection.find(coll) == AirwinRegistry::namesByCollection.end())
    {
        auto nx = AirwinRegistry::neighborIndexFor(processor.curentProcessorIndex, dir);
        processor.pushResetTypeFromUI(nx);
    }
    else
    {
        int sidx = processor.curentProcessorIndex;
        auto &collFX = AirwinRegistry::namesByCollection.at(coll);
        auto nx = AirwinRegistry::neighborIndexFor(processor.curentProcessorIndex, dir);
        while (nx != sidx)
        {
            auto rg = AirwinRegistry::registry[nx];
            if (collFX.find(rg.name) != collFX.end())
            {
                processor.pushResetTypeFromUI(nx);
                return;
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
    auto ccoll = getCurrentCollection();
    for (const auto &[c, _] : AirwinRegistry::namesByCollection)
    {
        collMenu.addItem(c, true, c == ccoll, [cv = c, w = juce::Component::SafePointer(this)]() {
            if (!w)
                return;
            w->setCurrentCollection(cv);
        });
    }
    collMenu.addSeparator();
    collMenu.addItem("All Plugins", true, ccoll == allCollection,
                     [w = juce::Component::SafePointer(this)]() {
                         if (!w)
                             return;
                         w->setCurrentCollection(w->allCollection);
                     });
    p.addSubMenu("Filter by Collection", collMenu);
    p.addSeparator();

    const auto *order = &AirwinRegistry::fxByCategory;

    bool isChrisOrder = properties->getValue("ordering") == "chris";
    if (isChrisOrder)
        order = &AirwinRegistry::fxByCategoryChrisOrder;

    auto coll = getCurrentCollection();

    std::unordered_set<std::string> inCol;
    if (AirwinRegistry::namesByCollection.find(coll) != AirwinRegistry::namesByCollection.end())
        inCol = AirwinRegistry::namesByCollection.at(coll);

    for (const auto &[cat, set] : *order)
    {
        juce::PopupMenu sub;
        for (const auto &nm : set)
        {
            bool include{false};
            if (coll == allCollection || inCol.empty())
            {
                include = true;
            }
            else
            {
                auto rg = AirwinRegistry::registry[AirwinRegistry::nameToIndex.at(nm)];
                if (inCol.find(rg.name) != inCol.end())
                {
                    include = true;
                }
            }
            if (include)
                sub.addItem(
                    nm, true, nm == ent.name, [nm, w = juce::Component::SafePointer(this)]() {
                        if (w)
                            w->processor.pushResetTypeFromUI(AirwinRegistry::nameToIndex.at(nm));
                    });
        }
        if (sub.getNumItems() > 0)
            p.addSubMenu(cat, sub, true, nullptr, cat == ent.category);
    }

    p.addSeparator();

    p.addItem("Read the Plugin Manual", []() {
        juce::URL("https://github.com/baconpaul/airwin2rack/blob/main/doc/manualdaw.md")
            .launchInDefaultBrowser();
    });

    p.addItem("Visit Airwindows.com",
              []() { juce::URL("https://www.airwindows.com").launchInDefaultBrowser(); });

    auto settingsMenu = juce::PopupMenu();
    auto isRO = properties->getBoolValue("editorIsReadOnly");
    settingsMenu.addItem("Use Accessible Documentation Component", true, !isRO,
                         [isRO, w = juce::Component::SafePointer(this)]() {
                             w->properties->setValue("editorIsReadOnly", !isRO);
                             w->docBodyEd->setReadOnly(!isRO);
                         });

    settingsMenu.addSeparator();

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

        // OK so readonly state is !RO and if it is readonly then
        // it is bad for accessibility so !ro means acc off

        if (getAccessibilityHandler())
            getAccessibilityHandler()->postAnnouncement(
                juce::String() + "Accessible Documentation Component is " + (!isRO ? "Off" : "On"),
                juce::AccessibilityHandler::AnnouncementPriority::medium);
    }
    return false;
}

std::unique_ptr<juce::ComponentTraverser>
AWConsolidatedAudioProcessorEditor::createKeyboardFocusTraverser()
{
    return std::make_unique<FxFocusTrav>(this);
}
