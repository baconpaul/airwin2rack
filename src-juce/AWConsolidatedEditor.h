#ifndef AWCONSOLIDATED_EDITOR_H
#define AWCONSOLIDATED_EDITOR_H

#include "AWConsolidatedProcessor.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
/**
 */
struct DocPanel;
class AWConsolidatedAudioProcessorEditor : public juce::AudioProcessorEditor, juce::AsyncUpdater
{
  public:
    AWConsolidatedAudioProcessorEditor(AWConsolidatedAudioProcessor &);
    ~AWConsolidatedAudioProcessorEditor();

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    virtual void handleAsyncUpdate() override;

    void showMenu();
    void jog(int dir);

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
    std::unique_ptr<juce::Component> menuPicker;
    std::array<std::unique_ptr<juce::Component>, AWConsolidatedAudioProcessor::nAWParams> knobs;
    std::array<std::unique_ptr<juce::Component>, AWConsolidatedAudioProcessor::nAWParams> labels;

    std::unique_ptr<juce::Drawable> clipperIcon;

    std::unique_ptr<DocPanel> docArea;
    std::unique_ptr<juce::Viewport> docView;

    juce::String docString, docHeader;

    juce::Typeface::Ptr jakartaSansMedium, jakartaSansSemi, firaMono;

    std::unique_ptr<juce::LookAndFeel_V4> lnf;
    std::unique_ptr<juce::PropertiesFile> properties;

    // debugging
    int gv{0}, bv{0};
#if 0
    struct AccSlider : public juce::Slider
    {
        AccSlider() { setWantsKeyboardFocus(true); }
        juce::String getTextFromValue(double v) override
        {
            // std::cout << "GTFV " << v << std::endl;
            // return juce::Slider::getTextFromValue(v);
            //  This is a bit of a hack to externalize this but
            return tv;
        }

        juce::String tv;
        void setTextValue(juce::String s)
        {
            tv = s;
            if (auto *handler = getAccessibilityHandler())
            {
                handler->notifyAccessibilityEvent(juce::AccessibilityEvent::valueChanged);
            }
        }
        bool keyPressed(const juce::KeyPress &key) override
        {
            float amt = 0.05;
            if (key.getModifiers().isShiftDown())
                amt = 0.01;
            if (key.getKeyCode() == juce::KeyPress::upKey)
            {
                setValue(std::clamp(getValue() + amt, 0., 1.),
                         juce::NotificationType::sendNotification);
                return true;
            }

            if (key.getKeyCode() == juce::KeyPress::downKey)
            {
                setValue(std::clamp(getValue() - amt, 0., 1.),
                         juce::NotificationType::sendNotification);
                return true;
            }

            if (key.getKeyCode() == juce::KeyPress::homeKey)
            {
                setValue(1., juce::NotificationType::sendNotification);
                return true;
            }

            if (key.getKeyCode() == juce::KeyPress::endKey)
            {
                setValue(0., juce::NotificationType::sendNotification);
                return true;
            }
            return false;
        }
    };
    AccSlider fxParamSliders[n_fx_params];

    void blastToggleState(int i);
    void resetLabels();

    std::unique_ptr<juce::Label> fxNameLabel;

    void addAndMakeVisibleRecordOrder(juce::Component *c)
    {
        accessibleOrderWeakRefs.push_back(c);
        addAndMakeVisible(c);
    }

    bool keyPressed(const juce::KeyPress &key) override;

  public:
    std::vector<juce::Component *> accessibleOrderWeakRefs;

  public:
    std::unique_ptr<juce::ComponentTraverser> createFocusTraverser() override;
#endif

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AWConsolidatedAudioProcessorEditor)
};

#endif // SURGE_SRC_SURGE_FX_SURGEFXEDITOR_H
