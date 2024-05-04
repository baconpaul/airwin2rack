#ifndef AWCONSOLIDATED_EDITOR_H
#define AWCONSOLIDATED_EDITOR_H

#include "AWConsolidatedProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/**
 */
struct DocPanel;
struct ParamKnob;
struct ParamDisp;
struct Picker;
enum ColourIds
{
    gradientStart,
    gradientStop,
    jog,
    jogHovered,
    jogStroke,
    hamburger,
    hamburgerHovered,
    hamburgerStroke,
    pickerTypeinForeground,
    pickerTypeinBackground,
    pickerBackground,
    pickerForeground,
    pickerStroke,
    pickerListBoxBackground,
    pickerListBoxStroke,
    typeaheadCategory,
    typeaheadName,
    typeaheadStroke,
    awLink,
    awLinkHovered,
    paramDispEditorBackground,
    paramDispEditorForeground,
    paramDispEditorStroke,
    paramDispEditorStrokeFocused,
    paramDispBackground,
    paramDispForeground,
    paramDispStroke,
    paramKnob,
    paramKnobHovered,
    paramKnobValueStroke,
    paramKnobGutter,
    paramKnobStroke,
    documentationBackground,
    documentationForeground,
    documentationStroke,
    documentationStrokeFocused,
    documentationHeader,
    footerBackground,
    footerForeground,
    footerStroke,
};
struct AWLookAndFeel : public juce::LookAndFeel_V4
{
    AWLookAndFeel();

    void setToSystemTheme();
    void setDarkTheme();
    void setLightTheme();

    juce::Typeface::Ptr jakartaSansMedium;
    juce::Font getPopupMenuFont() override;

    void drawPopupMenuBackgroundWithOptions(juce::Graphics &g, int width, int height,
                                            const juce::PopupMenu::Options &o) override;
};
class AWConsolidatedAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           juce::AsyncUpdater,
                                           juce::DarkModeSettingListener
{
  public:
    AWConsolidatedAudioProcessorEditor(AWConsolidatedAudioProcessor &);
    ~AWConsolidatedAudioProcessorEditor();

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    bool keyPressed(const juce::KeyPress &) override;

    virtual void handleAsyncUpdate() override;

    void showMenu();
    void jog(int dir);

    std::string getCurrentCollection()
    {
        return properties->getValue("collection", "Recommended").toStdString();
    }
    void setCurrentCollection(const std::string &s)
    {
        properties->setValue("collection", juce::String(s));
    }
    std::string allCollection{"All"};

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AWConsolidatedAudioProcessor &processor;

    static constexpr int baseWidth = 400, baseHeight = 600;

    struct IdleTimer : juce::Timer
    {
        IdleTimer(AWConsolidatedAudioProcessorEditor *ed) : ed(ed) {}
        ~IdleTimer() = default;
        void timerCallback() override { ed->idle(); }
        AWConsolidatedAudioProcessorEditor *ed;
    };
    void idle();
    std::unique_ptr<IdleTimer> idleTimer;
    std::unique_ptr<Picker> menuPicker;
    std::array<std::unique_ptr<ParamKnob>, AWConsolidatedAudioProcessor::nAWParams> knobs;
    std::array<std::unique_ptr<ParamDisp>, AWConsolidatedAudioProcessor::nAWParams> labels;

    std::unique_ptr<juce::Drawable> clipperIcon;
    std::unique_ptr<juce::Component> awTag;

    juce::Rectangle<int> docAreaRect;
    std::unique_ptr<juce::TextEditor> docBodyEd;
    std::unique_ptr<juce::Label> docBodyLabel;

    void resizeDocArea();

    juce::String docString, docHeader;

    juce::Typeface::Ptr jakartaSansMedium, jakartaSansSemi, firaMono;

    std::unique_ptr<AWLookAndFeel> lnf;
    std::unique_ptr<juce::PropertiesFile> properties;

    std::vector<juce::Component *> accessibleOrderWeakRefs;
    std::unique_ptr<juce::ComponentTraverser> createKeyboardFocusTraverser() override;

    void darkModeSettingChanged() override;

  private:
    enum ColorStrategy
    {
        FOLLOW_SYSTEM = 1,
        ALWAYS_DARK = 2,
        ALWAYS_LIGHT = 3
    } currentColorStrategy{FOLLOW_SYSTEM};
    void updateColorStrategy(ColorStrategy s, bool writeProperties);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AWConsolidatedAudioProcessorEditor)
};

#endif // SURGE_SRC_SURGE_FX_SURGEFXEDITOR_H
