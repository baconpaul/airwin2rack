#ifndef AWCONSOLIDATED_EDITOR_H
#define AWCONSOLIDATED_EDITOR_H

#include "AWConsolidatedProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/**
 */
struct ParamKnob;
struct ParamDisp;
struct Picker;
struct SettingsCog;
struct DocHeader;
struct BypassButton;

enum ColourIds
{
    gradientStart,
    gradientStop,
    jog,
    jogHovered,
    jogStroke,
    favoriteActive,
    help,
    helpHovered,
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
    footerBrightLabel,
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
    paramKnobLabelBelow,
    paramKnobLabelWithin,

    documentationBackground,
    documentationForeground,
    documentationStroke,
    documentationStrokeFocused,
    documentationHeader,
    footerBackground,
    footerForeground,
    footerStroke,

    settingCogOutline,
    settingCogFill,
    settingCogHover,

    bypassButtonOutline,
    bypassButtonOn,
    bypassButtonOnHover,
    bypassButtonBypassed,
    bypassButtonBypassedHover,

};

enum FontIDs
{
    pluginName,              // juce::Font(editor->jakartaSansMedium).withHeight(28)
    pluginCategory,          // juce::Font(editor->jakartaSansMedium).withHeight(18)edi
    pluginTypeaheadName,     // juce::Font(picker->editor->jakartaSansSemi).withHeight(22)
    pluginTypeaheadCategory, // jjuce::Font(picker->editor->jakartaSansMedium).withHeight(14)
    pluginTypeaheadWhat,     // juce::Font(picker->editor->jakartaSansMedium).withHeight(14)

    paramValue,     // juce::Font(editor->firaMono).withHeight(18)
    paramTitle,     // juce::Font(editor->jakartaSansMedium).withHeight(14)
    paramNoParamas, // juce::Font(editor->jakartaSansSemi).withHeight(20)

    documentationLabel, // juce::Font(jakartaSansMedium).withHeight(18)
    documentationBody,  // juce::Font(jakartaSansMedium).withHeight(15)

    airwindowsFooter, // jakartaSaneSmi at 28
    dateFooter,       // medium 12

    settingsHeader,
    settingsSubHeader,
    settinsLabel,

    hideDoc // medium 17
};
struct AWLookAndFeel : public juce::LookAndFeel_V4
{
    AWLookAndFeel();

    void setToSystemTheme();
    void setDarkTheme();
    void setLightTheme();

    juce::Typeface::Ptr jakartaSansMedium, jakartaSansSemi, firaMono;

    juce::Font getPopupMenuFont() override;
    juce::Font lookupFont(FontIDs) const;

    void drawPopupMenuBackgroundWithOptions(juce::Graphics &g, int width, int height,
                                            const juce::PopupMenu::Options &o) override;

    juce::PropertiesFile *propFileWeak{nullptr};
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

    void showEffectsMenu(bool justCurrentCategory = false);
    void jog(int dir);

    std::string getCurrentCollection()
    {
        return processor.properties->getValue("collection", "Recommended").toStdString();
    }
    void setCurrentCollection(const std::string &s);
    std::string allCollection{"All"};
    std::string favoritesCollection{"Favorites"};

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AWConsolidatedAudioProcessor &processor;

    static constexpr int baseWidth = 600, baseHeight = 600;

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

    std::atomic<bool> inActive{true}, outActive{true};
    std::unique_ptr<ParamKnob> inLevel, outLevel;
    std::unique_ptr<juce::Drawable> clipperIcon;

    juce::Rectangle<int> docAreaRect;
    std::unique_ptr<juce::TextEditor> docBodyEd;
    std::unique_ptr<DocHeader> docBodyLabel;

    void resizeDocArea();
    bool isDocDisplayed();
    void toggleDocDisplay();
    void sizeBasedOnDocAreaDisplay();

    std::unique_ptr<SettingsCog> settingsCog;
    std::unique_ptr<BypassButton> bypassButton;
    juce::PopupMenu makeSettingsMenu(bool withHeader);

    juce::String docString, docHeader;

    std::unique_ptr<AWLookAndFeel> lnf;

    std::vector<juce::Component *> accessibleOrderWeakRefs;
    std::unique_ptr<juce::ComponentTraverser> createKeyboardFocusTraverser() override;

    void darkModeSettingChanged() override;

    void addCurrentAsFavorite();
    void removeCurrentAsFavorite();
    void streamFavorites();
    void unstreamFavorites();
    juce::File getFavoritesFile(bool makeDir) const;
    std::set<std::string> favoritesList{};

  private:
    enum PostRebuidFocusTargetIs
    {
        NOTHING_SPECIAL,
        JOG_UP,
        JOG_DOWN,
        PICKER_MENU
    } postRebuildFocus{NOTHING_SPECIAL};

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
