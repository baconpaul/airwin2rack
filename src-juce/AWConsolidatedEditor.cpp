#include <algorithm>

#include <juce_gui_basics/juce_gui_basics.h>

#include "AWConsolidatedEditor.h"
#include "AirwinRegistry.h"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(awconsolidated_resources);
namespace awres = cmrc::awconsolidated_resources;

AWLookAndFeel::AWLookAndFeel()
{
    setToSystemTheme();
    try
    {

        auto fs = awres::get_filesystem();
        if (fs.is_file("res/PlusJakartaSans-Medium.ttf"))
        {
            auto f = fs.open("res/PlusJakartaSans-Medium.ttf");
            jakartaSansMedium = juce::Typeface::createSystemTypefaceFor(f.begin(), f.size());
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
    catch (const std::exception &e)
    {
        // shrug
    }
}

void AWLookAndFeel::setToSystemTheme()
{
    juce::Desktop::getInstance().isDarkModeActive() ? setDarkTheme() : setLightTheme();
}
void AWLookAndFeel::setDarkTheme()
{
    setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colour(10, 10, 15));
    setColour(juce::PopupMenu::ColourIds::headerTextColourId, juce::Colours::white);
    setColour(juce::PopupMenu::ColourIds::textColourId, juce::Colours::white);
    setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, juce::Colour(60, 60, 65));
    setColour(juce::PopupMenu::ColourIds::highlightedTextColourId, juce::Colours::white);
    setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colour(120, 120, 125));
    setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::white);

    setColour(ColourIds::gradientStart, juce::Colour(20, 20, 25));
    setColour(ColourIds::gradientStop, juce::Colour(50, 50, 55));

    setColour(ColourIds::jog, juce::Colour(90, 90, 95));
    setColour(ColourIds::jogHovered, juce::Colour(160, 160, 165));
    setColour(ColourIds::jogStroke, juce::Colours::white);

    setColour(ColourIds::hamburger, juce::Colour(90, 90, 95));
    setColour(ColourIds::hamburgerHovered, juce::Colour(160, 160, 165));
    setColour(ColourIds::hamburgerStroke, juce::Colours::white);

    setColour(ColourIds::pickerTypeinBackground, juce::Colour(10, 10, 15));
    setColour(ColourIds::pickerTypeinForeground, juce::Colours::white);

    setColour(ColourIds::pickerBackground, juce::Colours::black);
    setColour(ColourIds::pickerForeground, juce::Colours::white);
    setColour(ColourIds::pickerStroke, juce::Colours::lightgrey);
    setColour(ColourIds::pickerListBoxBackground, juce::Colour(10, 10, 20));
    setColour(ColourIds::pickerListBoxStroke, juce::Colours::red);

    setColour(ColourIds::typeaheadCategory, juce::Colours::white.darker(0.2));
    setColour(ColourIds::typeaheadName, juce::Colours::white);
    setColour(ColourIds::typeaheadStroke, juce::Colour(90, 90, 95));

    setColour(ColourIds::awLink, juce::Colours::black);
    setColour(ColourIds::awLinkHovered, juce::Colour(30, 30, 120));

    setColour(ColourIds::paramDispEditorBackground, juce::Colour(10, 10, 15));
    setColour(ColourIds::paramDispEditorForeground, juce::Colours::white);
    setColour(ColourIds::paramDispEditorStroke, juce::Colours::white.withAlpha(0.2f));
    setColour(ColourIds::paramDispEditorStrokeFocused, juce::Colours::white.withAlpha(0.2f));

    setColour(ColourIds::paramDispBackground, juce::Colours::black);
    setColour(ColourIds::paramDispForeground, juce::Colours::white);
    setColour(ColourIds::paramDispStroke, juce::Colours::white);

    setColour(ColourIds::paramKnob, juce::Colour(60, 60, 65));
    setColour(ColourIds::paramKnobHovered, juce::Colour(75, 75, 80));
    setColour(ColourIds::paramKnobValueStroke, juce::Colour(220, 220, 230));
    setColour(ColourIds::paramKnobGutter, juce::Colour(0, 0, 0));
    setColour(ColourIds::paramKnobStroke, juce::Colour(140, 140, 150));

    setColour(ColourIds::documentationHeader, juce::Colours::white);
    setColour(ColourIds::documentationBackground, juce::Colours::black.withAlpha(0.f));
    setColour(ColourIds::documentationForeground, juce::Colours::white.darker(0.2f));
    setColour(ColourIds::documentationStroke, juce::Colours::black.withAlpha(0.f));
    setColour(ColourIds::documentationStrokeFocused, juce::Colours::black.withAlpha(0.f));

    setColour(ColourIds::footerBackground, juce::Colour(160, 160, 170));
    setColour(ColourIds::footerForeground, juce::Colour(110, 110, 115));
    setColour(ColourIds::footerStroke, juce::Colours::black);
}

void AWLookAndFeel::setLightTheme()
{
    setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colour(245, 245, 240));
    setColour(juce::PopupMenu::ColourIds::headerTextColourId, juce::Colours::black);
    setColour(juce::PopupMenu::ColourIds::textColourId, juce::Colours::black);
    setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId,
              juce::Colour(195, 195, 190));
    setColour(juce::PopupMenu::ColourIds::highlightedTextColourId, juce::Colours::black);
    setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colour(135, 135, 130));
    setColour(juce::TextEditor::ColourIds::highlightedTextColourId, juce::Colours::black);

    setColour(ColourIds::gradientStart, juce::Colour(235, 235, 230));
    setColour(ColourIds::gradientStop, juce::Colour(205, 205, 200));

    setColour(ColourIds::jog, juce::Colour(165, 165, 160));
    setColour(ColourIds::jogHovered, juce::Colour(95, 95, 90));
    setColour(ColourIds::jogStroke, juce::Colours::black);

    setColour(ColourIds::hamburger, juce::Colour(165, 165, 160));
    setColour(ColourIds::hamburgerHovered, juce::Colour(95, 95, 90));
    setColour(ColourIds::hamburgerStroke, juce::Colours::black);

    setColour(ColourIds::pickerTypeinBackground, juce::Colour(245, 245, 240));
    setColour(ColourIds::pickerTypeinForeground, juce::Colours::black);

    setColour(ColourIds::pickerBackground, juce::Colours::white);
    setColour(ColourIds::pickerForeground, juce::Colours::black);
    setColour(ColourIds::pickerStroke, juce::Colours::darkgrey);
    setColour(ColourIds::pickerListBoxBackground, juce::Colour(245, 245, 235));
    setColour(ColourIds::pickerListBoxStroke, juce::Colours::red);

    setColour(ColourIds::typeaheadCategory, juce::Colours::black.darker(0.2));
    setColour(ColourIds::typeaheadName, juce::Colours::black);
    setColour(ColourIds::typeaheadStroke, juce::Colour(165, 165, 160));

    setColour(ColourIds::awLink, juce::Colours::white);
    setColour(ColourIds::awLinkHovered, juce::Colour(60, 60, 180));

    setColour(ColourIds::paramDispEditorBackground, juce::Colour(245, 245, 240));
    setColour(ColourIds::paramDispEditorForeground, juce::Colours::black);
    setColour(ColourIds::paramDispEditorStroke, juce::Colours::black.withAlpha(0.2f));
    setColour(ColourIds::paramDispEditorStrokeFocused, juce::Colours::black.withAlpha(0.2f));

    setColour(ColourIds::paramDispBackground, juce::Colours::white);
    setColour(ColourIds::paramDispForeground, juce::Colours::black);
    setColour(ColourIds::paramDispStroke, juce::Colours::black);

    setColour(ColourIds::paramKnob, juce::Colour(195, 195, 190));
    setColour(ColourIds::paramKnobHovered, juce::Colour(180, 180, 175));
    setColour(ColourIds::paramKnobValueStroke, juce::Colour(245, 245, 255));
    setColour(ColourIds::paramKnobGutter, juce::Colour(65, 65, 75));
    setColour(ColourIds::paramKnobStroke, juce::Colour(115, 115, 105));

    setColour(ColourIds::documentationHeader, juce::Colours::black);
    setColour(ColourIds::documentationBackground, juce::Colours::white.withAlpha(0.f));
    setColour(ColourIds::documentationForeground, juce::Colours::black.darker(0.2f));
    setColour(ColourIds::documentationStroke, juce::Colours::white.withAlpha(0.f));
    setColour(ColourIds::documentationStrokeFocused, juce::Colours::white.withAlpha(0.f));

    setColour(ColourIds::footerBackground, juce::Colour(95, 95, 85));
    setColour(ColourIds::footerForeground, juce::Colour(145, 145, 140));
    setColour(ColourIds::footerStroke, juce::Colours::white);
}

juce::Font AWLookAndFeel::getPopupMenuFont()
{
    return juce::Font(jakartaSansMedium).withHeight(16);
}

void AWLookAndFeel::drawPopupMenuBackgroundWithOptions(juce::Graphics &g, int width, int height,
                                                       const juce::PopupMenu::Options &o)
{
    auto background = findColour(juce::PopupMenu::backgroundColourId);
    g.fillAll(background);

    g.setColour(findColour(juce::PopupMenu::textColourId).withAlpha(0.6f));
    g.drawRect(0, 0, width, height);
}

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
                g.setColour(findColour(ColourIds::jogHovered));
            else
                g.setColour(findColour(ColourIds::jog));
            g.fillPath(p);
            g.setColour(findColour(ColourIds::jogStroke));
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
                    g.setColour(findColour(ColourIds::hamburgerHovered));
                else
                    g.setColour(findColour(ColourIds::hamburger));
                g.fillRoundedRectangle(q, 1);
                g.setColour(findColour(ColourIds::hamburgerStroke));
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
        typeinEd->setFont(editor->lnf->lookupFont(pluginName));
        typeinEd->setColour(juce::TextEditor::ColourIds::textColourId,
                            findColour(ColourIds::pickerTypeinForeground));
        typeinEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                            findColour(ColourIds::pickerTypeinBackground));
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

        g.setColour(findColour(ColourIds::pickerBackground));
        g.fillRoundedRectangle(bounds, 5);
        g.setColour(findColour(ColourIds::pickerStroke));
        g.drawRoundedRectangle(bounds, 5, 1);
        g.setColour(findColour(ColourIds::pickerForeground));
        g.setFont(editor->lnf->lookupFont(pluginName));
        if (!typeinEd->isVisible())
            g.drawText(rg.name, bounds.reduced(8, 5), juce::Justification::centredBottom);
        auto ga = juce::GlyphArrangement();
        auto tbx = bounds.reduced(8, 5);
        ga.addFittedText(editor->lnf->lookupFont(pluginName), rg.name, tbx.getX(),
                         tbx.getY(), tbx.getWidth(), tbx.getHeight(),
                         juce::Justification::centredBottom, 1);
        titleBox = ga.getBoundingBox(0, -1, true);

        auto catString = rg.category;
        g.setFont(editor->lnf->lookupFont(pluginCategory));
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
                listBox->setColour(juce::ListBox::backgroundColourId,
                                   findColour(ColourIds::pickerListBoxBackground));
                listBox->setColour(juce::ListBox::outlineColourId,
                                   findColour(ColourIds::pickerListBoxStroke));

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

            g.setFont(picker->editor->lnf->lookupFont(pluginTypeaheadName));
            g.setColour(picker->findColour(ColourIds::typeaheadName));
            g.drawText(rg.name, bx, juce::Justification::bottomLeft);

            g.setColour(picker->findColour(ColourIds::typeaheadCategory));

            g.setFont(picker->editor->lnf->lookupFont(pluginTypeaheadCategory));
            g.drawText(rg.category, bx, juce::Justification::topLeft);

            g.setFont(picker->editor->lnf->lookupFont(pluginTypeaheadWhat));
            g.drawFittedText(rg.whatText, bx.withTrimmedLeft(bx.getWidth() / 2),
                             juce::Justification::bottomRight, 3);

            g.setColour(picker->findColour(ColourIds::typeaheadStroke));
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
    AWLookAndFeel *lnf;
    AWLink(AWLookAndFeel *l) : lnf(l) {}
    void paint(juce::Graphics &g) override
    {
        g.setColour(findColour(ColourIds::awLink));
        if (isHovered)
            g.setColour(findColour(ColourIds::awLinkHovered));
        g.setFont(lnf->lookupFont(airwindowsFooter));
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

        resetColors();

        addChildComponent(*typeinEd);
    }

    void resetColors()
    {
        typeinEd->setFont(editor->lnf->lookupFont(paramValue));
        typeinEd->setColour(juce::TextEditor::ColourIds::textColourId,
                            findColour(ColourIds::paramDispEditorForeground));
        typeinEd->setColour(juce::TextEditor::ColourIds::outlineColourId,
                            findColour(ColourIds::paramDispEditorStroke));
        typeinEd->setColour(juce::TextEditor::ColourIds::focusedOutlineColourId,
                            findColour(ColourIds::paramDispEditorStrokeFocused));
        typeinEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                            findColour(ColourIds::paramDispEditorBackground));
        typeinEd->addListener(this);
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

    void refreshModel()
    {
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

    void mouseWheelMove(const juce::MouseEvent &event,
                        const juce::MouseWheelDetails &wheel) override;
    void paint(juce::Graphics &g) override
    {
        if (!active)
        {
            if (isP0)
            {
                auto b = getLocalBounds().withTrimmedRight(43);

                g.setColour(findColour(ColourIds::paramDispForeground));
                g.setFont(editor->lnf->lookupFont(paramNoParamas));
                g.drawText("No Parameters", b, juce::Justification::centredTop);
            }
            return;
        }
        auto rb = getLocalBounds().toFloat().reduced(1);
        g.setColour(findColour(ColourIds::paramDispBackground));
        g.fillRoundedRectangle(rb, 3);
        g.setColour(findColour(ColourIds::paramDispStroke));
        g.drawRoundedRectangle(rb, 3, 1);

        auto bounds = getLocalBounds().reduced(5, 2);
        g.setFont(editor->lnf->lookupFont(paramValue));
        g.drawText(weakParam->getCurrentValueAsText().trim(), bounds.withTrimmedBottom(2),
                   juce::Justification::bottomLeft);

        g.setFont(editor->lnf->lookupFont(paramTitle));
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
            g.setColour(findColour(ColourIds::paramKnobHovered));
        else
            g.setColour(findColour(ColourIds::paramKnob));

        g.fillEllipse(knobHandle.reduced(2));

        g.setColour(findColour(ColourIds::paramKnobStroke));
        g.strokePath(arc(-0.01f, 1.01f), juce::PathStrokeType(6));

        g.setColour(findColour(ColourIds::paramKnobGutter));
        g.strokePath(arc(0.f, 1.f), juce::PathStrokeType(4));

        g.setColour(findColour(ColourIds::paramKnobValueStroke));
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

    void mouseWheelMove(const juce::MouseEvent &event,
                        const juce::MouseWheelDetails &wheel) override
    {
#if JUCE_MAC
        auto scaleFac = 1.0;
#else
        auto scaleFac = 0.1;
#endif
        auto amt = wheel.deltaY * scaleFac;
        if (wheel.isReversed)
            amt = -amt;

        if (event.mods.isShiftDown())
            amt *= 0.1;

        auto nv = std::clamp(weakParam->get() + amt, 0., 1.);
        weakParam->beginChangeGesture();
        weakParam->setValueNotifyingHost(nv);
        weakParam->endChangeGesture();
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

void ParamDisp::mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel)
{
    if (editor->knobs[index])
        editor->knobs[index]->mouseWheelMove(event, wheel);
}
//==============================================================================
AWConsolidatedAudioProcessorEditor::AWConsolidatedAudioProcessorEditor(
    AWConsolidatedAudioProcessor &p)
    : AudioProcessorEditor(&p), processor(p)
{
    juce::Desktop::getInstance().addDarkModeSettingListener(this);
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
    docBodyLabel->setFont(lnf->lookupFont(documentationLabel));
    docBodyLabel->setColour(juce::Label::textColourId, findColour(ColourIds::documentationHeader));
    docBodyLabel->setTitle("Documentation Header");
    addAndMakeVisible(*docBodyLabel);

    docBodyEd = std::make_unique<juce::TextEditor>("Documentation");
    docBodyEd->setMultiLine(true);
    docBodyEd->setFont(lnf->lookupFont(documentationBody));
    docBodyEd->setReadOnly(false);
    docBodyEd->setWantsKeyboardFocus(true);
    docBodyEd->setCaretVisible(true);
    docBodyEd->setAccessible(true);
    docBodyEd->setTitle("Documentation");
    docBodyEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                         findColour(ColourIds::documentationBackground));
    docBodyEd->setColour(juce::TextEditor::ColourIds::outlineColourId,
                         findColour(ColourIds::documentationStroke));
    docBodyEd->setColour(juce::TextEditor::ColourIds::focusedOutlineColourId,
                         findColour(ColourIds::documentationStrokeFocused));
    docBodyEd->setColour(juce::TextEditor::ColourIds::textColourId,
                         findColour(ColourIds::documentationForeground));
    addAndMakeVisible(*docBodyEd);
    awTag = std::make_unique<AWLink>(lnf.get());
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
#if JUCE_LINUX
    options.folderName = ".config/AirwindowsConsolidated";
#else
    options.folderName = "AirwindowsConsolidated";
#endif

    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Preferences";
    properties = std::make_unique<juce::PropertiesFile>(options);
    lnf->propFileWeak = properties.get();

    auto isRO = properties->getBoolValue("editorIsReadOnly", true);
    docBodyEd->setReadOnly(isRO);

    auto cs = properties->getIntValue("colorStrategy", (int)ColorStrategy::FOLLOW_SYSTEM);
    updateColorStrategy((ColorStrategy)cs, false);
}

AWConsolidatedAudioProcessorEditor::~AWConsolidatedAudioProcessorEditor()
{
    lnf->propFileWeak = nullptr;
    juce::Desktop::getInstance().removeDarkModeSettingListener(this);
    idleTimer->stopTimer();
}

void AWConsolidatedAudioProcessorEditor::updateColorStrategy(AWConsolidatedAudioProcessorEditor::ColorStrategy s, bool writeProperties)
{
    if (!lnf)
        return;
    currentColorStrategy = s;
    if (writeProperties)
        properties->setValue("colorStrategy", (int)s);
    switch(s)
    {
    case ALWAYS_DARK:
        lnf->setDarkTheme();
        break;
    case ALWAYS_LIGHT:
        lnf->setLightTheme();
        break;
    case FOLLOW_SYSTEM:
        lnf->setToSystemTheme();
        break;
    }
    darkModeSettingChanged();
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
    auto tFont = lnf->lookupFont(documentationLabel);
    juce::GlyphArrangement gaTitle;
    // use a slightly narrower box to force an extra line to simulate the label insets
    gaTitle.addFittedText(tFont, docHeader.substring(2), r.getX() + 5, r.getY(), r.getWidth() - 10,
                          r.getHeight(), juce::Justification::topLeft, 3);
    auto bounds = gaTitle.getBoundingBox(0, -1, true);

    docBodyLabel->setFont(lnf->lookupFont(documentationLabel));
    docBodyLabel->setBounds(r.withHeight(bounds.getHeight() + 4));
    docBodyLabel->setText(docHeader.substring(2), juce::NotificationType::dontSendNotification);
    if (docBodyLabel->getAccessibilityHandler())
        docBodyLabel->getAccessibilityHandler()->notifyAccessibilityEvent(
            juce::AccessibilityEvent::valueChanged);

    auto q = r.withTrimmedTop(bounds.getHeight() + 8);

    docBodyEd->setBounds(q);
    docBodyEd->clear();
    docBodyEd->setFont(lnf->lookupFont(documentationBody));
    docBodyEd->setText(docString, false);
}

void AWConsolidatedAudioProcessorEditor::handleAsyncUpdate() {}

void AWConsolidatedAudioProcessorEditor::resized() {}

void AWConsolidatedAudioProcessorEditor::paint(juce::Graphics &g)
{
    auto b = getLocalBounds();
    auto gr =
        juce::ColourGradient(findColour(ColourIds::gradientStart), {0.f, 0.f},
                             findColour(ColourIds::gradientStop), {0.f, 1.f * getHeight()}, false);
    g.setGradientFill(gr);
    g.fillAll();

    static constexpr float footerHeight{40};
    auto fa = b.withHeight(footerHeight).withY(getHeight() - footerHeight);
    g.setColour(findColour(ColourIds::footerBackground));
    g.fillRect(fa);
    g.setColour(findColour(ColourIds::footerStroke));
    g.drawLine(fa.getX(), fa.getY(), fa.getX() + fa.getWidth(), fa.getY(), 1);

    g.setFont(lnf->lookupFont(dateFooter));
    g.setColour(findColour(ColourIds::footerForeground));

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

    auto csMenu = juce::PopupMenu();
    csMenu.addItem("Follow System Settings", true, currentColorStrategy == FOLLOW_SYSTEM,
                   [w = juce::Component::SafePointer(this)]() {
                       if (w)
                           w->updateColorStrategy(FOLLOW_SYSTEM, true);
                   });
    csMenu.addItem("Always Dark", true, currentColorStrategy == ALWAYS_DARK,
                   [w = juce::Component::SafePointer(this)]() {
                       if (w)
                           w->updateColorStrategy(ALWAYS_DARK, true);
                   });
    csMenu.addItem("Always Light", true, currentColorStrategy == ALWAYS_LIGHT,
                   [w = juce::Component::SafePointer(this)]() {
                       if (w)
                           w->updateColorStrategy(ALWAYS_LIGHT, true);
                   });
    settingsMenu.addSubMenu("Color Scheme", csMenu);

    auto fsMenu = juce::PopupMenu();
    int fontOffset = 0;
    if (properties)
        fontOffset = properties->getIntValue("docFontSize", 0);

    for (const auto &[off, nm] : { std::make_pair(-2, std::string("Small")),
                                  {0, "Regular"},
                                  {2, "Large"},
                                  {4, "Extra-Large"}})
    {
        fsMenu.addItem(nm, true, fontOffset == off, [o = off, w = juce::Component::SafePointer(this)](){
            if (!w)
                return;
            w->properties->setValue("docFontSize", o);
            w->resizeDocArea();
        } );
    }

    settingsMenu.addSubMenu("Documentation Font", fsMenu);

    settingsMenu.addSeparator();

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

    p.addSeparator();
    p.addSubMenu("Settings", settingsMenu);

    const auto mousePos = juce::Desktop::getInstance().getMousePosition();
    const auto targetArea = juce::Rectangle<int>{}.withPosition (mousePos);
    p.showMenuAsync(juce::PopupMenu::Options().withMaximumNumColumns(1).withTargetComponent(this).withTargetScreenArea(targetArea));
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

void AWConsolidatedAudioProcessorEditor::darkModeSettingChanged()
{
    if (currentColorStrategy == FOLLOW_SYSTEM)
        juce::Desktop::getInstance().isDarkModeActive() ? lnf->setDarkTheme() : lnf->setLightTheme();

    if (docBodyLabel)
    {
        docBodyLabel->setColour(juce::Label::textColourId,
                                findColour(ColourIds::documentationHeader));
    }

    if (docBodyEd)
    {
        docBodyEd->applyColourToAllText(findColour(ColourIds::documentationForeground));
    }

    if (menuPicker)
    {
        if (menuPicker->typeinEd)
        {
            menuPicker->typeinEd->setColour(juce::TextEditor::ColourIds::textColourId,
                                            findColour(ColourIds::pickerTypeinForeground));
            menuPicker->typeinEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                                            findColour(ColourIds::pickerTypeinBackground));
            menuPicker->typeinEd->applyColourToAllText(
                findColour(ColourIds::pickerTypeinForeground));
        }

        if (menuPicker->listBox)
        {
            menuPicker->listBox->setColour(juce::ListBox::backgroundColourId,
                                           findColour(ColourIds::pickerListBoxBackground));
            menuPicker->listBox->setColour(juce::ListBox::outlineColourId,
                                           findColour(ColourIds::pickerListBoxStroke));
        }
    }

    for (const auto &p : labels)
    {
        p->resetColors();
    }

    repaint();
}

juce::Font AWLookAndFeel::lookupFont(FontIDs fid) const
{
    int fontOffset = 0;
    if (propFileWeak)
        fontOffset = propFileWeak->getIntValue("docFontSize", 0);

    switch (fid)
    {
    case pluginName:
        return juce::Font(jakartaSansSemi).withHeight(28);
    case pluginCategory:
        return juce::Font(jakartaSansMedium).withHeight(18);
    case pluginTypeaheadName:
        return juce::Font(jakartaSansSemi).withHeight(20);
    case pluginTypeaheadCategory:
        return juce::Font(jakartaSansMedium).withHeight(14);
    case pluginTypeaheadWhat:
        return juce::Font(jakartaSansMedium).withHeight(14);

    case paramValue:
        return juce::Font(firaMono).withHeight(18);
    case paramTitle:
        return juce::Font(jakartaSansMedium).withHeight(14);
    case paramNoParamas:
        return juce::Font(jakartaSansSemi).withHeight(20);

    case documentationLabel:
        return juce::Font(jakartaSansMedium).withHeight(19 + fontOffset);
    case documentationBody:
        return juce::Font(jakartaSansMedium).withHeight(16 + fontOffset);

    case airwindowsFooter:
        return juce::Font(jakartaSansSemi).withHeight(28);
    case dateFooter:
        return juce::Font(firaMono).withHeight(14);

    default:
        return juce::Font("Comic Sans MS", 24, juce::Font::bold);
    }
}