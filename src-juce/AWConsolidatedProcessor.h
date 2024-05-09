
#ifndef AWCONSOLIDATED_PROCESSOR_H
#define AWCONSOLIDATED_PROCESSOR_H

#include "AirwinRegistry.h"
#include "juce_audio_processors/juce_audio_processors.h"

#if MAC
#include <execinfo.h>
#endif

#if DEBUG_LOCK
struct LP {
    std::string st;
    int ln;
    LP(const std::string &s, int l) : st(s), ln(l) {
        std::cout << "LP LOCK " << st << ":" << ln << std::endl;
    }

    ~LP() {
        std::cout << "LP UNLOCK " << st << ":" << ln << std::endl;
    }
};

#define LOCK(x)                                                                                    \
    std::cout << __FILE__ << ":" << __LINE__ << " Locking " << #x << std::endl;                    \
    LP lpCheck(__FILE__, __LINE__);                                                                                               \
    std::lock_guard<std::mutex> g(x);                                                              \
    std::cout << __FILE__ << ":" << __LINE__ << " Lock Garnerd " << #x << std::endl;
#else
#define LOCK(x) std::lock_guard<std::mutex> g(x);
#endif

template <typename T, int Capacity = 4096> class LockFreeQueue
{
  public:
    LockFreeQueue() : fifo(Capacity) {}

    void push(const T &item)
    {
        int start1, size1, start2, size2;
        fifo.prepareToWrite(1, start1, size1, start2, size2);

        if (size1 > 0)
        {
            buffer[start1] = item;
        }

        fifo.finishedWrite(size1);
    }

    bool pop(T &item)
    {
        int start1, size1, start2, size2;
        fifo.prepareToRead(1, start1, size1, start2, size2);

        if (size1 > 0)
        {
            item = buffer[start1];
        }

        fifo.finishedRead(size1);

        return size1 > 0;
    }

  private:
    juce::AbstractFifo fifo;
    std::array<T, Capacity> buffer;
};

//==============================================================================
/**
 */
class AWConsolidatedAudioProcessor : public juce::AudioProcessor,
                                     public juce::AudioProcessorParameter::Listener,
                                     public juce::AsyncUpdater
{
  public:
    static constexpr int nAWParams{10};

    //==============================================================================
    AWConsolidatedAudioProcessor();
    ~AWConsolidatedAudioProcessor();

    //==============================================================================
    std::atomic<bool> isPlaying{false};
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    void processBlock(juce::AudioBuffer<double> &, juce::MidiBuffer &) override;
    bool supportsDoublePrecisionProcessing() const override { return true; }
    template<typename T> void processBlockT(juce::AudioBuffer<T> &);

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool isStarting) override {}
    void handleAsyncUpdate() override {}

    struct ResetTypeMsg
    {
        int type; // -1 for type, otherwise param
        int32_t toIndex;
        float value;
    };
    LockFreeQueue<ResetTypeMsg> resetType;

    // Call this from the UI thread only
    void pushResetTypeFromUI(int32_t index)
    {
        auto &rg = AirwinRegistry::registry[index];
        {
            LOCK(displayProcessorMutex);
            awDisplayProcessor = rg.generator();
            awDisplayProcessor->setSampleRate(getSampleRate());
        }
        setupParamDisplaysFromDisplayProcessor(index);

        if (isPlaying)
        {
            curentProcessorIndex = index;
            resetType.push({-1, index, 0.f});
        }
        else
        {
            setAWProcessorTo(index, false);
        }
    }
    void setupParamDisplaysFromDisplayProcessor(int index);

    std::atomic<bool> refreshUI{false},
        rebuildUI{false}; // repaint vs re-setup everything. Value vs type

    struct AWParam : public juce::AudioParameterFloat
    {
        juce::String mutableName;
        template <typename... Args>
        AWParam(Args &&...args) : juce::AudioParameterFloat(std::forward<Args>(args)...)
        {
        }

        juce::String getName(int end) const override { return mutableName.substring(0, end); }

        std::function<juce::String(float, int)> getTextHandler{nullptr};
        std::function<float(const juce::String &)> getTextToValue{nullptr};
        juce::String getText(float f, int i) const override {
            if (getTextHandler)
                return getTextHandler(f, i);
            return std::to_string(f);
        }

        float getValueForText(const juce::String &text) const override
        {
            return getTextToValue(text);
        }

        std::function<float()> getDefaultValueHandler{nullptr};
        float getDefaultValue() const override
        {
            if (getDefaultValueHandler)
                return getDefaultValueHandler();
            return 0;
        }
    };

    //==============================================================================
    typedef AWParam float_param_t;
    float_param_t *fxParams[nAWParams];
    float defaultValues[nAWParams];
    std::array<std::atomic<bool>, nAWParams> active;

    void setAWProcessorTo(int registryIndex, bool initDisplay);

    std::unique_ptr<AirwinConsolidatedBase> awProcessor, awDisplayProcessor;
    std::mutex displayProcessorMutex;
    int nProcessorParams{0};
    std::atomic<int> curentProcessorIndex{0};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AWConsolidatedAudioProcessor)
};

#endif // SURGE_SRC_SURGE_FX_SURGEFXPROCESSOR_H
