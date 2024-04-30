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
class AWConsolidatedAudioProcessorEditor : public juce::AudioProcessorEditor, juce::AsyncUpdater
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

    std::unique_ptr<juce::LookAndFeel_V4> lnf;
    std::unique_ptr<juce::PropertiesFile> properties;

    std::vector<juce::Component *> accessibleOrderWeakRefs;
    std::unique_ptr<juce::ComponentTraverser> createKeyboardFocusTraverser() override;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AWConsolidatedAudioProcessorEditor)
};

#endif // SURGE_SRC_SURGE_FX_SURGEFXEDITOR_H
