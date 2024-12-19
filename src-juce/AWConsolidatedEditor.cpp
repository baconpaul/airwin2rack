#include <algorithm>
#include <cmath>

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

    setColour(ColourIds::favoriteActive, juce::Colour(240, 100, 100));

    setColour(ColourIds::help, juce::Colour(20, 20, 25));
    setColour(ColourIds::helpHovered, juce::Colour(40, 40, 75));

    setColour(ColourIds::hamburger, juce::Colour(90, 90, 95));
    setColour(ColourIds::hamburgerHovered, juce::Colour(160, 160, 165));
    setColour(ColourIds::hamburgerStroke, juce::Colours::white);

    setColour(ColourIds::pickerTypeinBackground, juce::Colour(10, 10, 15));
    setColour(ColourIds::pickerTypeinForeground, juce::Colours::white);

    setColour(ColourIds::pickerBackground, juce::Colours::black);
    setColour(ColourIds::pickerForeground, juce::Colours::white);
    setColour(ColourIds::pickerStroke, juce::Colours::lightgrey);
    setColour(ColourIds::pickerListBoxBackground, juce::Colour(10, 10, 20));
    setColour(ColourIds::pickerListBoxStroke, juce::Colours::lightgrey);

    setColour(ColourIds::typeaheadCategory, juce::Colours::white.darker(0.2));
    setColour(ColourIds::typeaheadName, juce::Colours::white);
    setColour(ColourIds::typeaheadStroke, juce::Colours::lightgrey);

    setColour(ColourIds::footerBrightLabel, juce::Colours::black);

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
    setColour(ColourIds::paramKnobLabelBelow, juce::Colours::black);
    setColour(ColourIds::paramKnobLabelWithin, juce::Colour(160, 160, 180));

    setColour(ColourIds::documentationHeader, juce::Colours::white);
    setColour(ColourIds::documentationBackground, juce::Colours::black.withAlpha(0.f));
    setColour(ColourIds::documentationForeground, juce::Colours::white.darker(0.2f));
    setColour(ColourIds::documentationStroke, juce::Colours::black.withAlpha(0.f));
    setColour(ColourIds::documentationStrokeFocused, juce::Colours::black.withAlpha(0.f));

    setColour(ColourIds::footerBackground, juce::Colour(160, 160, 170));
    setColour(ColourIds::footerForeground, juce::Colour(110, 110, 115));
    setColour(ColourIds::footerStroke, juce::Colours::black);

    setColour(ColourIds::settingCogOutline, juce::Colours::black);
    setColour(ColourIds::settingCogFill, juce::Colour(200, 200, 210));
    setColour(ColourIds::settingCogHover, juce::Colour(240, 240, 240));

    setColour(ColourIds::bypassButtonOutline, juce::Colours::black);
    setColour(ColourIds::bypassButtonOn, juce::Colour(230, 230, 240));
    setColour(ColourIds::bypassButtonOnHover, juce::Colour(240, 240, 255));
    setColour(ColourIds::bypassButtonBypassed, juce::Colour(130, 130, 140));
    setColour(ColourIds::bypassButtonBypassedHover, juce::Colour(170, 170, 180));
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

    setColour(ColourIds::favoriteActive, juce::Colour(240, 100, 100));

    setColour(ColourIds::help, juce::Colour(220, 220, 245));
    setColour(ColourIds::helpHovered, juce::Colours::white);

    setColour(ColourIds::hamburger, juce::Colour(165, 165, 160));
    setColour(ColourIds::hamburgerHovered, juce::Colour(95, 95, 90));
    setColour(ColourIds::hamburgerStroke, juce::Colours::black);

    setColour(ColourIds::pickerTypeinBackground, juce::Colour(245, 245, 240));
    setColour(ColourIds::pickerTypeinForeground, juce::Colours::black);

    setColour(ColourIds::pickerBackground, juce::Colours::white);
    setColour(ColourIds::pickerForeground, juce::Colours::black);
    setColour(ColourIds::pickerStroke, juce::Colours::darkgrey);
    setColour(ColourIds::pickerListBoxBackground, juce::Colour(245, 245, 235));
    setColour(ColourIds::pickerListBoxStroke, juce::Colours::darkgrey);

    setColour(ColourIds::typeaheadCategory, juce::Colours::black.darker(0.2));
    setColour(ColourIds::typeaheadName, juce::Colours::black);
    setColour(ColourIds::typeaheadStroke, juce::Colours::darkgrey);

    setColour(ColourIds::footerBrightLabel, juce::Colours::white);

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
    setColour(ColourIds::paramKnobLabelBelow, juce::Colours::white);
    setColour(ColourIds::paramKnobLabelWithin, juce::Colour(65, 65, 75));

    setColour(ColourIds::documentationHeader, juce::Colours::black);
    setColour(ColourIds::documentationBackground, juce::Colours::white.withAlpha(0.f));
    setColour(ColourIds::documentationForeground, juce::Colours::black.brighter(0.2f));
    setColour(ColourIds::documentationStroke, juce::Colours::white.withAlpha(0.f));
    setColour(ColourIds::documentationStrokeFocused, juce::Colours::white.withAlpha(0.f));

    setColour(ColourIds::footerBackground, juce::Colour(95, 95, 85));
    setColour(ColourIds::footerForeground, juce::Colour(145, 145, 140));
    setColour(ColourIds::footerStroke, juce::Colours::white);

    setColour(ColourIds::settingCogOutline, juce::Colours::white);
    setColour(ColourIds::settingCogFill, juce::Colour(60, 60, 65));
    setColour(ColourIds::settingCogHover, juce::Colour(100, 100, 105));

    setColour(ColourIds::bypassButtonOutline, juce::Colours::black);
    setColour(ColourIds::bypassButtonOn, juce::Colour(230, 230, 240));
    setColour(ColourIds::bypassButtonOnHover, juce::Colour(240, 240, 255));
    setColour(ColourIds::bypassButtonBypassed, juce::Colour(130, 130, 140));
    setColour(ColourIds::bypassButtonBypassedHover, juce::Colour(170, 170, 180));
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

    struct Heart : juce::ToggleButton, juce::ToggleButton::Listener
    {
        Picker *picker;
        Heart(Picker *p) : juce::ToggleButton("Favorite"), picker(p)
        {
            setAccessible(true);
            addListener(this); // i know its a bit sloppy to self listen
        }

        void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override
        {
            auto gs = juce::Graphics::ScopedSaveState(g);
            float w = getWidth();
            float h = getHeight();
            g.addTransform(juce::AffineTransform().scaled(0.8).translated(w * 0.1, h * 0.1));
            auto p = juce::Path();

            auto afac{1.25};
            p.addCentredArc(w / 4, h / 3, w / 4, h / 4, 0, -afac * M_PI / 2, -M_PI / 2, true);
            p.addCentredArc(w / 4, h / 3, w / 4, h / 3, 0, -M_PI / 2, M_PI / 2, false);
            p.addCentredArc(3 * w / 4, h / 3, w / 4, h / 3, 0, -M_PI / 2, M_PI / 2, false);
            p.addCentredArc(3 * w / 4, h / 3, w / 4, h / 4, 0, M_PI / 2, afac * M_PI / 2, false);
            p.lineTo(w / 2, h);
            p.closeSubPath();

            if (getToggleState())
            {
                g.setColour(findColour(favoriteActive));
                g.fillPath(p);
            }
            g.setColour(findColour(jogStroke));
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

        void buttonClicked(juce::Button *) override
        {
            if (getToggleState())
            {
                picker->editor->addCurrentAsFavorite();
            }
            else
            {
                picker->editor->removeCurrentAsFavorite();
            }
        }
    };
    std::unique_ptr<Heart> heartButton;

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
        void mouseDown(const juce::MouseEvent &e) override { picker->editor->showEffectsMenu(); }

        bool keyPressed(const juce::KeyPress &p) override
        {
            if (p.getKeyCode() == juce::KeyPress::returnKey ||
                (p.getKeyCode() == juce::KeyPress::F10Key && p.getModifiers().isShiftDown()))
            {
                // dojog does long hold stuff so go direct
                picker->editor->showEffectsMenu();
                return true;
            }
            return false;
        }
    };
    std::unique_ptr<Hamburger> hamburger;

    bool isCurrentEffectFavorite()
    {
        int idx = editor->processor.curentProcessorIndex;
        auto &rg = AirwinRegistry::registry[idx];
        auto nm = rg.name;

        return editor->favoritesList.find(nm) != editor->favoritesList.end();
    }

    Picker(AWConsolidatedAudioProcessorEditor *ed) : editor(ed)
    {
        setAccessible(true);
        setTitle("Select Airwindow");
        setDescription("Select Airwindow");
        setWantsKeyboardFocus(true);

        up = std::make_unique<Jog>(this, -1);
        down = std::make_unique<Jog>(this, 1);
        hamburger = std::make_unique<Hamburger>(this);
        heartButton = std::make_unique<Heart>(this);

        addAndMakeVisible(*up);
        addAndMakeVisible(*down);
        addAndMakeVisible(*hamburger);
        addAndMakeVisible(*heartButton);

        typeinEd = std::make_unique<juce::TextEditor>("Typeahead");
        typeinEd->addListener(this);
        addChildComponent(*typeinEd);
    }
    ~Picker()
    {
        if (listBox)
            listBox->setModel(nullptr);
    }
    juce::Rectangle<float> titleBox;
    juce::Rectangle<float> categoryBox;

    void parentHierarchyChanged() override
    {
        // We will ahve our LnF changed
        if (getParentComponent())
            resetColors();
    }
    void resetColors()
    {
        typeinEd->setFont(editor->lnf->lookupFont(pluginName));
        typeinEd->setColour(juce::TextEditor::ColourIds::textColourId,
                            findColour(ColourIds::pickerTypeinForeground));
        typeinEd->setColour(juce::TextEditor::ColourIds::backgroundColourId,
                            findColour(ColourIds::pickerTypeinBackground));
        typeinEd->setColour(juce::TextEditor::ColourIds::outlineColourId,
                            findColour(ColourIds::pickerListBoxStroke));
    }

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
        ga.addFittedText(editor->lnf->lookupFont(pluginName), rg.name, tbx.getX(), tbx.getY(),
                         tbx.getWidth(), tbx.getHeight(), juce::Justification::centredBottom, 1);
        titleBox = ga.getBoundingBox(0, -1, true);

        auto catString = rg.category;
        g.setFont(editor->lnf->lookupFont(pluginCategory));
        g.drawText(catString, bounds.reduced(8, 3), juce::Justification::centredTop);

        auto cga = juce::GlyphArrangement();
        cga.addFittedText(editor->lnf->lookupFont(pluginCategory), rg.category, bounds.getX(),
                          bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                          juce::Justification::centredTop, 1);
        categoryBox = cga.getBoundingBox(0, -1, true).expanded(10, 3);
    }

    juce::Rectangle<int> jogUp, jogDown;
    void resized() override
    {
        auto b = getLocalBounds().reduced(8, 10);
        auto hh = b.getHeight() / 2;
        jogUp = b.withHeight(hh).withTrimmedLeft(b.getWidth() - hh);
        jogDown = jogUp.translated(0, hh);

        auto hbSize{22};
        auto heartPos = jogUp.translated(-hbSize - 1, hh / 2)
                            .withWidth(hh)
                            .withHeight(hh)
                            .expanded((hbSize - hh) / 2);

        up->setBounds(jogUp);
        down->setBounds(jogDown);
        heartButton->setBounds(heartPos);

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
            editor->showEffectsMenu();
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
                listBox->setOutlineThickness(1);
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
            if (!editor->isDocDisplayed())
            {
                auto exp = getWidth() - teb.getWidth();
                teb = teb.expanded(exp * 0.5 - 5, 0);
            }
            listBox->setBounds(teb);

            populateForTypein();
        }
        else
        {
            editor->showEffectsMenu(categoryBox.contains(e.position));
        }
    }

    uint64_t jogHoldCounter{0};
    void startJogHold(int dir)
    {
        juce::Timer::callAfterDelay(
            800, [hc = jogHoldCounter, dir, w = juce::Component::SafePointer(this)]() {
                if (w)
                {
                    w->doJogHold(dir, hc);
                }
            });
    }

    void stopJogHold() { jogHoldCounter++; }
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

        heartButton->setToggleState(isCurrentEffectFavorite(),
                                    juce::NotificationType::dontSendNotification);
        auto coll = editor->getCurrentCollection();
        if (coll == editor->favoritesCollection)
        {
            heartButton->setEnabled(false);
        }
        else
        {
            heartButton->setEnabled(true);
        }

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

            auto bx = juce::Rectangle<int>(0, 0, width, height).reduced(4, 2);

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

struct SettingsCog : public juce::Button
{
    AWConsolidatedAudioProcessorEditor *editor;
    SettingsCog(AWConsolidatedAudioProcessorEditor *p)
        : juce::Button(juce::String("Settings")), editor(p)
    {
        setAccessible(true);
        setTitle("Settings");
    }
    void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override
    {
        juce::Path p;
        auto c = getLocalBounds().getCentre();
        auto rad = std::min(getWidth(), getHeight()) * 0.45;
        int nGears = 6;
        for (int i = 0; i < 2 * nGears; ++i)
        {
            auto sh = 0.5;
            if (i % 2 == 1)
                sh = 0.31;
            auto sp = (i - sh) * 2 * M_PI / (2 * nGears);
            auto ep = (i + sh) * 2 * M_PI / (2 * nGears);
            auto r = (i % 2 == 1 ? rad * 0.72 : rad);
            p.addCentredArc(c.x, c.y, r, r, 0, sp, ep, i == 0);
        }
        p.closeSubPath();

        g.setColour(isHovered ? editor->lnf->findColour(settingCogHover)
                              : editor->lnf->findColour(settingCogFill));
        g.fillPath(p);

        g.setColour(editor->lnf->findColour(settingCogOutline));
        g.strokePath(p, juce::PathStrokeType(1));

        g.setColour(editor->lnf->findColour(footerBackground));
        auto off = 0.47;
        g.fillEllipse(c.x - rad * off, c.y - rad * off, rad * off * 2, rad * off * 2);
        g.setColour(editor->lnf->findColour(settingCogOutline));
        g.drawEllipse(c.x - rad * off, c.y - rad * off, rad * off * 2, rad * off * 2, 1);
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

    void mouseDown(const juce::MouseEvent &e) override
    {
        showMenu(-e.position.x, -e.position.y - 3);
    }

    void showMenu(int xo = 0, int yo = 0)
    {
        auto pm = editor->makeSettingsMenu(true);

        const auto mousePos = juce::Desktop::getInstance().getMousePosition().translated(xo, yo);
        const auto targetArea = juce::Rectangle<int>{}.withPosition(mousePos);
        pm.showMenuAsync(juce::PopupMenu::Options()
                             .withMaximumNumColumns(1)
                             .withTargetComponent(editor)
                             .withTargetScreenArea(targetArea));
    }
    bool keyPressed(const juce::KeyPress &p) override
    {
        if (p.getKeyCode() == juce::KeyPress::returnKey ||
            (p.getKeyCode() == juce::KeyPress::F10Key && p.getModifiers().isShiftDown()))
        {
            showMenu();
            return true;
        }
        return false;
    }
};

struct BypassButton : public juce::ToggleButton
{
    AWConsolidatedAudioProcessorEditor *editor;
    BypassButton(AWConsolidatedAudioProcessorEditor *p)
        : juce::ToggleButton(juce::String("Bypass")), editor(p)
    {
        setAccessible(true);
        setTitle("Bypass");
    }
    void paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override
    {
        juce::Path p;
        auto c = getLocalBounds().getCentre();
        auto rad = std::min(getWidth(), getHeight()) * 0.4;

        auto da = 0.85;
        auto dr = 0.7;
        p.addCentredArc(c.x, c.y, rad, rad, 0, -M_PI * da, M_PI * da, true);
        p.addCentredArc(c.x, c.y, rad * dr, rad * dr, 0, M_PI * da, -M_PI * da, false);

        p.closeSubPath();

        juce::Colour fc;
        if (isOver())
        {
            if (getToggleState())
                fc = editor->lnf->findColour(bypassButtonBypassedHover);
            else
                fc = editor->lnf->findColour(bypassButtonOnHover);
        }
        else
        {
            if (getToggleState())
                fc = editor->lnf->findColour(bypassButtonBypassed);
            else
                fc = editor->lnf->findColour(bypassButtonOn);
        }

        g.setColour(fc);
        g.fillPath(p);

        g.setColour(editor->lnf->findColour(bypassButtonOutline));
        g.strokePath(p, juce::PathStrokeType(1));

        auto tr = getLocalBounds()
                      .reduced(getWidth() * 0.5 - 2, 0)
                      .withTrimmedTop(getHeight() * 0.55)
                      .withTrimmedBottom(2);

        g.setColour(fc);
        g.fillRect(tr);

        g.setColour(editor->lnf->findColour(bypassButtonOutline));
        g.drawRect(tr);
    }

    void buttonStateChanged() override
    {
        repaint();

        if (getToggleState() != editor->processor.bypassParam->get())
        {
            editor->processor.bypassParam->beginChangeGesture();
            editor->processor.bypassParam->setValueNotifyingHost(getToggleState());
            editor->processor.bypassParam->endChangeGesture();
        }
    }

    bool keyPressed(const juce::KeyPress &p) override
    {
        if (p.getKeyCode() == juce::KeyPress::returnKey)
        {
            setToggleState(!getToggleState(), juce::NotificationType::sendNotification);
            return true;
        }
        return false;
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

        addChildComponent(*typeinEd);
    }

    void parentHierarchyChanged() override
    {
        // We will ahve our LnF changed
        if (getParentComponent())
            resetColors();
    };

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
            LOCK(editor->processor.displayProcessorMutex);
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
                LOCK(editor->processor.displayProcessorMutex);
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
    AWConsolidatedAudioProcessor::APFPublicDefault *weakParam{nullptr};
    const std::atomic<bool> &active;
    AWConsolidatedAudioProcessorEditor *editor{nullptr};
    int index{0};
    bool showValueBelow{false};
    std::string showValuePrefix{};
    ParamKnob(const juce::String &cn, AWConsolidatedAudioProcessor::APFPublicDefault *param,
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

        if (showValueBelow)
            knobHandle = knobHandle.expanded(2).reduced(3, 0).withTrimmedBottom(8);

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

        int wo = showValueBelow ? 3 : 4;

        g.setColour(findColour(ColourIds::paramKnobStroke));
        g.strokePath(arc(-0.01f, 1.01f), juce::PathStrokeType(wo + 2));

        g.setColour(findColour(ColourIds::paramKnobGutter));
        g.strokePath(arc(0.f, 1.f), juce::PathStrokeType(wo));

        g.setColour(findColour(ColourIds::paramKnobValueStroke));
        g.strokePath(arc(0.f, getValue()), juce::PathStrokeType(wo));

        if (showValueBelow)
        {
            g.setFont(editor->lnf->lookupFont(documentationLabel).withHeight(8.5));
            g.setColour(findColour(ColourIds::paramKnobLabelBelow));
            std::string txt = weakParam->getCurrentValueAsText().toStdString();
            g.drawText(txt, getLocalBounds(), juce::Justification::centredBottom);

            g.setColour(findColour(ColourIds::paramKnobLabelWithin));
            g.drawText(showValuePrefix, knobHandle, juce::Justification::centred);
        }
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
        repaint();
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
        repaint();
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
            repaint();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::downKey)
        {
            setValue(std::clamp((double)getValue() - amt, 0., 1.));
            repaint();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::homeKey)
        {
            setValue(1.);
            repaint();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::endKey)
        {
            setValue(0.);
            repaint();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::deleteKey && weakParam)
        {
            setValue(weakParam->getDefaultValue());
            repaint();
            return true;
        }

        if (key.getKeyCode() == juce::KeyPress::F10Key && key.getModifiers().isShiftDown())
        {
            if (index > 0)
            {
                editor->labels[index]->showEd();
            }
            return true;
        }
        return false;
    }

    void mouseDoubleClick(const juce::MouseEvent &event) override
    {
        if (weakParam)
        {
            setValue(weakParam->getDefaultValue());
            repaint();
        }
    }

    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
    {
        return std::make_unique<AH>(this);
    }
};

struct DocHeader : juce::Component
{
    AWConsolidatedAudioProcessorEditor *editor{nullptr};
    DocHeader(AWConsolidatedAudioProcessorEditor *e) : editor(e)
    {
        setAccessible(true);
        setWantsKeyboardFocus(true);
        setTitle("Documentation header");
    }

    juce::String txt;
    void setText(const juce::String &s)
    {
        txt = s;
        repaint();
    }

    void paint(juce::Graphics &g) override
    {
        g.setColour(editor->lnf->findColour(documentationHeader));
        g.setFont(editor->lnf->lookupFont(documentationLabel));
        g.drawFittedText(txt, 5, 0, getWidth() - 10, getHeight(), juce::Justification::topLeft, 10);
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
        DocHeader *dh{nullptr};
        AH(DocHeader *c)
            : juce::AccessibilityHandler(*c, juce::AccessibilityRole::label,
                                         juce::AccessibilityActions(),
                                         AccessibilityHandler::Interfaces{}),
              dh(c)
        {
        }

        juce::String getTitle() const override { return dh->txt; }
    };

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
    // juce::LookAndFeel::setDefaultLookAndFeel(lnf.get());
    setLookAndFeel(lnf.get());
    setAccessible(true);
    setFocusContainerType(juce::Component::FocusContainerType::keyboardFocusContainer);
    setWantsKeyboardFocus(true);

    unstreamFavorites();

    setSize(baseWidth, baseHeight);

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

    inLevel = std::make_unique<ParamKnob>("inLevel", processor.inLev, inActive, this, -1);
    inLevel->setAccessible(true);
    inLevel->showValueBelow = true;
    inLevel->showValuePrefix = "in";
    addAndMakeVisible(*inLevel);

    outLevel = std::make_unique<ParamKnob>("outLevel", processor.outLev, outActive, this, -1);
    outLevel->setAccessible(true);

    outLevel->showValueBelow = true;
    outLevel->showValuePrefix = "out";
    addAndMakeVisible(*outLevel);

    docAreaRect = getLocalBounds()
                      .withTrimmedLeft(margin * 3 + sz + 180)
                      .withTrimmedRight(margin * 2)
                      .withTrimmedTop(60 + 2 * margin)
                      .withTrimmedBottom(40 + margin);

    docBodyLabel = std::make_unique<DocHeader>(this);
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

    settingsCog = std::make_unique<SettingsCog>(this);
    addAndMakeVisible(*settingsCog);

    bypassButton = std::make_unique<BypassButton>(this);
    addAndMakeVisible(*bypassButton);

    accessibleOrderWeakRefs.push_back(menuPicker.get());
    accessibleOrderWeakRefs.push_back(menuPicker->hamburger.get());
    accessibleOrderWeakRefs.push_back(menuPicker->heartButton.get());
    accessibleOrderWeakRefs.push_back(menuPicker->up.get());
    accessibleOrderWeakRefs.push_back(menuPicker->down.get());
    for (auto &k : knobs)
        accessibleOrderWeakRefs.push_back(k.get());
    accessibleOrderWeakRefs.push_back(docBodyLabel.get());
    accessibleOrderWeakRefs.push_back(docBodyEd.get());
    accessibleOrderWeakRefs.push_back(settingsCog.get());
    accessibleOrderWeakRefs.push_back(inLevel.get());
    accessibleOrderWeakRefs.push_back(outLevel.get());
    accessibleOrderWeakRefs.push_back(bypassButton.get());

    lnf->propFileWeak = processor.properties.get();

    auto isRO = processor.properties->getBoolValue("editorIsReadOnly", true);
    docBodyEd->setReadOnly(isRO);

    auto cs = processor.properties->getIntValue("colorStrategy", (int)ColorStrategy::FOLLOW_SYSTEM);
    updateColorStrategy((ColorStrategy)cs, false);

    resized();
    sizeBasedOnDocAreaDisplay();
}

AWConsolidatedAudioProcessorEditor::~AWConsolidatedAudioProcessorEditor()
{
    // release any references to the look and feel
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());

    // detatch from processor prop file
    lnf->propFileWeak = nullptr;

    // stop lsitenres and timers
    juce::Desktop::getInstance().removeDarkModeSettingListener(this);
    idleTimer->stopTimer();
}

void AWConsolidatedAudioProcessorEditor::updateColorStrategy(
    AWConsolidatedAudioProcessorEditor::ColorStrategy s, bool writeproperties)
{
    if (!lnf)
        return;
    currentColorStrategy = s;
    if (writeproperties)
        processor.properties->setValue("colorStrategy", (int)s);
    switch (s)
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

        if (postRebuildFocus != NOTHING_SPECIAL)
        {
            switch (postRebuildFocus)
            {
            case JOG_UP:
                menuPicker->up->grabKeyboardFocus();
                break;

            case JOG_DOWN:
                menuPicker->down->grabKeyboardFocus();
                break;

            case PICKER_MENU:
                menuPicker->hamburger->grabKeyboardFocus();
                break;

            case NOTHING_SPECIAL:
                menuPicker->grabKeyboardFocus();
                break;
            }
        }
        postRebuildFocus = NOTHING_SPECIAL;
    }

    if (processor.refreshUI.exchange(false))
    {
        bypassButton->setToggleState(processor.bypassParam->get(),
                                     juce::NotificationType::dontSendNotification);
        repaint();
    }
}

void AWConsolidatedAudioProcessorEditor::resizeDocArea()
{
    if (!isDocDisplayed())
    {
        docBodyLabel->setVisible(false);
        docBodyEd->setVisible(false);
        return;
    }
    else
    {
        docBodyLabel->setVisible(true);
        docBodyEd->setVisible(true);
    }
    docString = AirwinRegistry::documentationStringFor(processor.curentProcessorIndex);
    if (docString.isEmpty())
    {
        docHeader = "# " + AirwinRegistry::registry[processor.curentProcessorIndex].whatText;
        docString = "No documentation available.";
    }
    else
    {
        docHeader = docString.upToFirstOccurrenceOf("\n", false, false);
        docString = docString.fromFirstOccurrenceOf("\n", false, false).trim();
    }

    // Load custom manual
    juce::String customDocumentationContent;
    if (loadCustomDocumentation(AirwinRegistry::registry[processor.curentProcessorIndex].name,
                                customDocumentationContent))
    {
        if (!customDocumentationContent.isEmpty())
        {
            docString = customDocumentationContent + "\n\n# --------------------\n\n" + docString;
        }
    }

    auto r = docAreaRect;
    auto tFont = lnf->lookupFont(documentationLabel);
    juce::GlyphArrangement gaTitle;
    // use a slightly narrower box to force an extra line to simulate the label insets
    gaTitle.addFittedText(tFont, docHeader.substring(2), r.getX() + 5, r.getY(), r.getWidth() - 10,
                          r.getHeight(), juce::Justification::topLeft, 3);
    auto bounds = gaTitle.getBoundingBox(0, -1, true);

    docBodyLabel->setBounds(r.withHeight(bounds.getHeight() + 4));
    docBodyLabel->setText(docHeader.substring(2));
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

void AWConsolidatedAudioProcessorEditor::resized()
{
    if (!processor.properties || !menuPicker)
    {
        // not set up yet
        return;
    }

    auto idd = isDocDisplayed();
    resizeDocArea();

    menuPicker->setBounds(getLocalBounds().reduced(5).withHeight(60));

    for (const auto &l : labels)
    {
        if (idd)
            l->setSize(180, l->getHeight());
        else
        {
            auto w = getWidth() - 5 - l->getX();
            l->setSize(w, l->getHeight());
        }
    }

    auto ta = getLocalBounds()
                  .withTrimmedTop(getHeight() - 40)
                  .withTrimmedLeft(2)
                  .withWidth(40)
                  .reduced(4);
    settingsCog->setBounds(ta);
    inLevel->setBounds(ta.translated(ta.getWidth() + 4, 1));
    ta = ta.translated(getWidth() - 40 - 4 - 2 - 2, 0);
    outLevel->setBounds(ta.translated(-ta.getWidth() - 4, 1));
    bypassButton->setBounds(ta);
}

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

    g.setFont(lnf->lookupFont(airwindowsFooter));
    g.setColour(findColour(ColourIds::footerBrightLabel));
    g.drawText("Airwindows", fa.withTrimmedTop(0), juce::Justification::centredTop);

    g.setFont(lnf->lookupFont(dateFooter));
    g.setColour(findColour(ColourIds::footerForeground));

    auto fs = juce::String(__DATE__) + " " + BUILD_HASH;
    g.drawText(fs, fa.withTrimmedBottom(2), juce::Justification::centredBottom);

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

    bool isChrisOrder = processor.properties->getValue("ordering") == "chris";
    auto neighbor = [isChrisOrder](int idx, int dir) {
        if (isChrisOrder)
        {
            return AirwinRegistry::neighborChrisIndexFor(idx, dir);
        }
        else
        {
            return AirwinRegistry::neighborIndexFor(idx, dir);
        }
    };

    if (dir == 1)
        postRebuildFocus = JOG_DOWN;
    else
        postRebuildFocus = JOG_UP;

    if (coll == favoritesCollection && !favoritesList.empty())
    {
        int sidx = processor.curentProcessorIndex;
        auto &rg = AirwinRegistry::registry[sidx];
        // Theres lots of ways to do this but for now lets do it the crude way
        std::vector<std::string> v(favoritesList.begin(), favoritesList.end());
        int idx{-1}, c{0};
        for (auto &vn : v)
        {
            if (vn == rg.name)
            {
                idx = c;
            }
            c++;
        }
        auto nidx = idx + dir;
        if (nidx < 0)
            nidx = v.size() - 1;
        if (nidx >= (int)v.size())
            nidx = 0;
        auto nfidx = AirwinRegistry::nameToIndex[v[nidx]];
        processor.pushResetTypeFromUI(nfidx);
    }
    else if (coll == allCollection || AirwinRegistry::namesByCollection.find(coll) ==
                                          AirwinRegistry::namesByCollection.end())
    {
        auto nx = neighbor(processor.curentProcessorIndex, dir);
        processor.pushResetTypeFromUI(nx);
    }
    else
    {
        int sidx = processor.curentProcessorIndex;
        auto &collFX = AirwinRegistry::namesByCollection.at(coll);
        auto nx = neighbor(processor.curentProcessorIndex, dir);
        while (nx != sidx)
        {
            auto rg = AirwinRegistry::registry[nx];
            if (collFX.find(rg.name) != collFX.end())
            {
                processor.pushResetTypeFromUI(nx);
                return;
            }

            nx = neighbor(nx, dir);
        }
    }
}

void AWConsolidatedAudioProcessorEditor::showEffectsMenu(bool justCurrentCategory)
{
    auto p = juce::PopupMenu();
    const auto &ent = AirwinRegistry::registry[processor.curentProcessorIndex];

    if (justCurrentCategory)
        p.addSectionHeader("Airwindows - " + ent.category);
    else
        p.addSectionHeader("Airwindows Consolidated");
    p.addSeparator();

    auto coll = getCurrentCollection();

    if (!favoritesList.empty() && coll != favoritesCollection)
    {
        auto sm = juce::PopupMenu();
        sm.addSectionHeader("Favorites");
        sm.addSeparator();
        for (auto f : favoritesList)
        {
            auto n2i = AirwinRegistry::nameToIndex.find(f);
            if (n2i != AirwinRegistry::nameToIndex.end())
            {
                auto ig = AirwinRegistry::registry[n2i->second];

                sm.addItem(f + " (" + ig.category + ")", true, f == ent.name,
                           [f, w = juce::Component::SafePointer(this)]() {
                               if (w)
                               {
                                   w->postRebuildFocus = PICKER_MENU;
                                   w->processor.pushResetTypeFromUI(
                                       AirwinRegistry::nameToIndex.at(f));
                               }
                           });
            }
        }
        p.addSubMenu("Favorites", sm);
        p.addSeparator();
    }
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
    if (!favoritesList.empty())
    {
        collMenu.addItem("Favorites", true, ccoll == favoritesCollection,
                         [w = juce::Component::SafePointer(this)]() {
                             if (!w)
                                 return;
                             w->setCurrentCollection(w->favoritesCollection);
                         });
    }
    collMenu.addItem("All Plugins", true, ccoll == allCollection,
                     [w = juce::Component::SafePointer(this)]() {
                         if (!w)
                             return;
                         w->setCurrentCollection(w->allCollection);
                     });
    p.addSubMenu("Filter by Collection", collMenu);
    p.addSeparator();

    const auto *order = &AirwinRegistry::fxByCategory;

    bool isChrisOrder = processor.properties->getValue("ordering") == "chris";
    if (isChrisOrder)
        order = &AirwinRegistry::fxByCategoryChrisOrder;

    std::unordered_set<std::string> inCol;
    if (AirwinRegistry::namesByCollection.find(coll) != AirwinRegistry::namesByCollection.end())
        inCol = AirwinRegistry::namesByCollection.at(coll);

    for (const auto &[cat, set] : *order)
    {
        if (justCurrentCategory && cat != ent.category)
            continue;

        juce::PopupMenu sub;
        auto *target = &sub;
        if (justCurrentCategory)
            target = &p;
        for (const auto &nm : set)
        {
            bool include{false};
            if (coll == favoritesCollection && !favoritesList.empty())
            {
                if (favoritesList.find(nm) != favoritesList.end())
                {
                    include = true;
                }
            }
            else if (coll == allCollection || inCol.empty())
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
                target->addItem(
                    nm, true, nm == ent.name, [nm, w = juce::Component::SafePointer(this)]() {
                        if (w)
                        {
                            w->postRebuildFocus = PICKER_MENU;
                            w->processor.pushResetTypeFromUI(AirwinRegistry::nameToIndex.at(nm));
                        }
                    });
        }
        if (!justCurrentCategory && sub.getNumItems() > 0)
            p.addSubMenu(cat, sub, true, nullptr, cat == ent.category);
    }

    p.addSeparator();

    p.addItem("Read the Plugin Manual", []() {
        juce::URL("https://github.com/baconpaul/airwin2rack/blob/main/doc/manualdaw.md")
            .launchInDefaultBrowser();
    });

    const auto mousePos = juce::Desktop::getInstance().getMousePosition();
    const auto targetArea = juce::Rectangle<int>{}.withPosition(mousePos);
    p.showMenuAsync(juce::PopupMenu::Options()
                        .withMaximumNumColumns(1)
                        .withTargetComponent(this)
                        .withTargetScreenArea(targetArea));
}

juce::PopupMenu AWConsolidatedAudioProcessorEditor::makeSettingsMenu(bool withHeader)
{
    bool isChrisOrder = processor.properties->getValue("ordering") == "chris";

    auto settingsMenu = juce::PopupMenu();
    if (withHeader)
    {
        settingsMenu.addSectionHeader("Settings");
        settingsMenu.addSeparator();
    }

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
    if (processor.properties)
        fontOffset = processor.properties->getIntValue("docFontSize", 0);

    for (const auto &[off, nm] : {std::make_pair(-2, std::string("Small")),
                                  {0, "Regular"},
                                  {2, "Large"},
                                  {4, "Extra-Large"}})
    {
        fsMenu.addItem(nm, true, fontOffset == off,
                       [o = off, w = juce::Component::SafePointer(this)]() {
                           if (!w)
                               return;
                           w->processor.properties->setValue("docFontSize", o);
                           w->resizeDocArea();
                       });
    }

    settingsMenu.addSubMenu("Documentation Font", fsMenu);

    settingsMenu.addItem("Show Documentation", true, isDocDisplayed(),
                         [w = juce::Component::SafePointer(this)]() {
                             if (w)
                                 w->toggleDocDisplay();
                         });

    settingsMenu.addSeparator();

    auto isRO = processor.properties->getBoolValue("editorIsReadOnly");
    settingsMenu.addItem("Use Accessible Documentation Component", true, !isRO,
                         [isRO, w = juce::Component::SafePointer(this)]() {
                             w->processor.properties->setValue("editorIsReadOnly", !isRO);
                             w->docBodyEd->setReadOnly(!isRO);
                         });

    settingsMenu.addSeparator();

    settingsMenu.addItem("Alphabetical Order Menus", true, !isChrisOrder,
                         [w = juce::Component::SafePointer(this)]() {
                             if (w)
                             {
                                 w->processor.properties->setValue("ordering", "alpha");
                             }
                         });
    settingsMenu.addItem("Chris (Quality) Order Menus", true, isChrisOrder,
                         [w = juce::Component::SafePointer(this)]() {
                             if (w)
                             {
                                 w->processor.properties->setValue("ordering", "chris");
                             }
                         });
    settingsMenu.addSeparator();
    auto rg = AirwinRegistry::registry[processor.curentProcessorIndex];
    auto n = juce::String(rg.name);
    settingsMenu.addItem("Make " + n + " your default effect",
                         [n, w = juce::Component::SafePointer(this)]() {
                             if (!w)
                                 return;
                             w->processor.properties->setValue("startupEffect", n);
                         });

    settingsMenu.addSeparator();

    settingsMenu.addItem("Open Plugin Manual...", []() {
        juce::URL("https://github.com/baconpaul/airwin2rack/blob/main/doc/manualdaw.md")
            .launchInDefaultBrowser();
    });

    settingsMenu.addSeparator();

    settingsMenu.addItem("Show Config Dir...", [this]() {
        // We cannot show a dir that doesnt exist, so we force create if needed
        getSettingsDirectory(true);
        // revealToUser() doesnt actually enter the directory given (at least on windows),
        // so we point it to Airwindows/customDocs/, to make it enter Airwindows/
        getSettingsDirectory(false).getChildFile("customDocs").revealToUser();
    });

    if (withHeader)
    {
        juce::PopupMenu about;
        settingsMenu.addSeparator();

        std::vector<std::string> versionInfo;
        about.addItem("Airwindows.com webpage...",
                      []() { juce::URL("https://airwindows.com").launchInDefaultBrowser(); });

        about.addItem("Download Latest Version...", []() {
            juce::URL("https://github.com/baconpaul/airwin2rack/releases/tag/DAWPlugin")
                .launchInDefaultBrowser();
        });
        about.addSeparator();
        about.addItem(juce::String("Built: ") + __DATE__ + " at " + __TIME__, false, false,
                      []() {});
        about.addItem(juce::String("Git Hash: ") + BUILD_HASH, false, false, []() {});

        settingsMenu.addSubMenu("About", about);
    }
    return settingsMenu;
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

        auto isAccessibleAndVisible = [](auto *r) { return r->isAccessible() && r->isVisible(); };

        switch (dir)
        {
        case 1:
        {
            auto res = std::next(iter);

            while (res != editor->accessibleOrderWeakRefs.cend() && !isAccessibleAndVisible(*res))
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
                   !isAccessibleAndVisible(*std::prev(res)))
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
        auto isRO = processor.properties->getBoolValue("editorIsReadOnly");

        processor.properties->setValue("editorIsReadOnly", !isRO);
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
        juce::Desktop::getInstance().isDarkModeActive() ? lnf->setDarkTheme()
                                                        : lnf->setLightTheme();

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
            menuPicker->typeinEd->setColour(juce::TextEditor::ColourIds::outlineColourId,
                                            findColour(ColourIds::pickerListBoxStroke));
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
        return juce::Font(firaMono).withHeight(12);

    case settingsHeader:
        return juce::Font(jakartaSansSemi).withHeight(22);

    case settingsSubHeader:
        return juce::Font(jakartaSansSemi).withHeight(18);
    case settinsLabel:
        return juce::Font(jakartaSansMedium).withHeight(15);

    case hideDoc:
        return juce::Font(jakartaSansMedium).withHeight(16);
    default:
        jassertfalse;
        return juce::Font("Comic Sans MS", 24, juce::Font::bold);
    }
}

bool AWConsolidatedAudioProcessorEditor::isDocDisplayed()
{
    if (!processor.properties)
        return true;

    return processor.properties->getBoolValue("isDocDisplayed", true);
}

void AWConsolidatedAudioProcessorEditor::toggleDocDisplay()
{
    auto res = processor.properties->getBoolValue("isDocDisplayed", true);
    processor.properties->setValue("isDocDisplayed", !res);
    sizeBasedOnDocAreaDisplay();
}
void AWConsolidatedAudioProcessorEditor::sizeBasedOnDocAreaDisplay()
{
    resizeDocArea();
    if (isDocDisplayed())
    {
        setSize(baseWidth, baseHeight);
    }
    else
    {
        setSize(baseWidth - 270, baseHeight);
    }
    repaint();
}

void AWConsolidatedAudioProcessorEditor::addCurrentAsFavorite()
{
    int idx = processor.curentProcessorIndex;
    auto &rg = AirwinRegistry::registry[idx];
    favoritesList.insert(rg.name);

    streamFavorites();
}

void AWConsolidatedAudioProcessorEditor::removeCurrentAsFavorite()
{
    int idx = processor.curentProcessorIndex;
    auto &rg = AirwinRegistry::registry[idx];
    favoritesList.erase(rg.name);

    streamFavorites();
}

void AWConsolidatedAudioProcessorEditor::streamFavorites()
{
    auto ff = getFavoritesFile(true);

    auto favx = juce::XmlElement("awfavorites");

    for (const auto &f : favoritesList)
    {
        auto el = new juce::XmlElement("favorite");
        el->setAttribute("fx", f);
        favx.addChildElement(el);
    }

    if (!favx.writeTo(ff))
    {
        // FIXME : Handle Error
    }
}

void AWConsolidatedAudioProcessorEditor::unstreamFavorites()
{
    auto ff = getFavoritesFile(false);
    favoritesList.clear();
    if (ff.exists())
    {
        auto xd = juce::XmlDocument(ff);
        auto re = xd.getDocumentElement();

        if (!re)
        {
            std::cerr << "Favorites file is corrupted: " << xd.getLastParseError() << std::endl;
            return;
        }

        for (auto *e : re->getChildIterator())
        {
            auto fn = e->getStringAttribute("fx");
            favoritesList.insert(fn.toStdString());
        }
    }
}

juce::File AWConsolidatedAudioProcessorEditor::getSettingsDirectory(bool makeDir) const
{
    juce::File res;

#if JUCE_LINUX
    res = juce::File::getSpecialLocation(juce::File::userHomeDirectory);
    res = res.getChildFile(".Airwindows");
#else
    res = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    res = res.getChildFile("Airwindows");
#endif

    if (makeDir)
    {
        res.createDirectory();
        res.getChildFile("customDocs").createDirectory();
    }

    return res;
}

juce::File AWConsolidatedAudioProcessorEditor::getFavoritesFile(bool makeDir) const
{
    juce::File res = getSettingsDirectory(makeDir);

    res = res.getChildFile("consolidatedFavorites.xml");

    return res;
}

bool AWConsolidatedAudioProcessorEditor::loadCustomDocumentation(const juce::String &fileName,
                                                                 juce::String &outContent) const
{
    static juce::String lastFileName;
    static juce::String cachedContent;

    if (fileName == lastFileName)
    {
        // Return the cached content
        outContent = cachedContent;
        return true;
    }

    juce::File customDocumentationDir = getSettingsDirectory(false).getChildFile("customDocs");
    juce::File documentationFile = customDocumentationDir.getChildFile(fileName + ".txt");

    if (!documentationFile.existsAsFile())
    {
        return false; // The file doesn't exist
    }

    juce::FileInputStream fileStream(documentationFile);

    if (!fileStream.openedOk())
    {
        return false; // The file can't be opened
    }

    cachedContent = fileStream.readEntireStreamAsString();
    lastFileName = fileName;
    outContent = cachedContent;

    return true;
}

void AWConsolidatedAudioProcessorEditor::setCurrentCollection(const std::string &s)
{
    processor.properties->setValue("collection", juce::String(s));
    menuPicker->rebuild();
}