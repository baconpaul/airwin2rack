#include <type_traits>

#include "AWConsolidatedProcessor.h"
#include "AWConsolidatedEditor.h"
#include "AirwinRegistry.h"

#if LINUX
// getCurrentPosition is deprecated in J7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

//==============================================================================
AWConsolidatedAudioProcessor::AWConsolidatedAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    AirwinConsolidatedBase::defaultSampleRate = 48000;

    // Multiple calls to addParameter here
    for (int i = 0; i < nAWParams; ++i)
    {
        fxParams[i] = new AWParam(juce::ParameterID(std::string("ctrl_") + std::to_string(i), 1),
                                  "Name", juce::NormalisableRange<float>(0.f, 1.f), 0.f);
        fxParams[i]->getTextHandler = [i, this](auto f, auto iv) {
            LOCK(this->displayProcessorMutex);
            if (this->awDisplayProcessor && i < this->nProcessorParams)
            {
                for (int id = 0; id < this->nProcessorParams; ++id)
                {
                    awDisplayProcessor->setParameter(id, fxParams[id]->get());
                }
                char tl[kVstMaxParamStrLen], td[kVstMaxParamStrLen];
                this->awDisplayProcessor->getParameterDisplay(i, td);
                this->awDisplayProcessor->getParameterLabel(i, tl);
                return std::string(td) + (tl[0] == 0 ? "" : " ") + std::string(tl);
            }
            else
            {
                return std::string("-");
            }
        };
        fxParams[i]->getTextToValue = [i, this](auto s) {
            LOCK(this->displayProcessorMutex);
            if (this->awDisplayProcessor && i < this->nProcessorParams)
            {
                float rv = 0.f;
                auto res =
                    this->awDisplayProcessor->parameterTextToValue(i, s.toStdString().c_str(), rv);
                if (res)
                    return rv;
            }
            return this->fxParams[i]->get();
        };
        fxParams[i]->getDefaultValueHandler = [i, this]() { return this->defaultValues[i]; };
        fxParams[i]->addListener(this);
        addParameter(fxParams[i]);
    }

    juce::PropertiesFile::Options options;
    options.applicationName = "AirwindowsConsolidated";
#if JUCE_LINUX
    options.folderName = ".config/AirwindowsConsolidated";
#else
    options.folderName = "AirwindowsConsolidated";
#endif

    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Application Support";
    properties = std::make_unique<juce::PropertiesFile>(options);

    auto defaultName = properties->getValue("startupEffect", "Galactic").toStdString();
    if (AirwinRegistry::nameToIndex.find(defaultName) == AirwinRegistry::nameToIndex.end())
        defaultName = "Galactic";

    setAWProcessorTo(AirwinRegistry::nameToIndex.at(defaultName), true);
}

AWConsolidatedAudioProcessor::~AWConsolidatedAudioProcessor() {}

//==============================================================================
const juce::String AWConsolidatedAudioProcessor::getName() const { return JucePlugin_Name; }

bool AWConsolidatedAudioProcessor::acceptsMidi() const { return false; }

bool AWConsolidatedAudioProcessor::producesMidi() const { return false; }

bool AWConsolidatedAudioProcessor::isMidiEffect() const { return false; }

double AWConsolidatedAudioProcessor::getTailLengthSeconds() const { return 2.0; }

int AWConsolidatedAudioProcessor::getNumPrograms() { return AirwinRegistry::registry.size(); }

int AWConsolidatedAudioProcessor::getCurrentProgram()
{
    // not super efficient obvs
    int idx{0};
    for (auto &i : AirwinRegistry::fxAlphaOrdering)
    {
        if (i == curentProcessorIndex)
            return idx;
        idx++;
    }
    return 0;
}

void AWConsolidatedAudioProcessor::setCurrentProgram(int index)
{
    auto rs = AirwinRegistry::fxAlphaOrdering[index];
    pushResetTypeFromUI(rs);
}

const juce::String AWConsolidatedAudioProcessor::getProgramName(int index)
{
    auto rs = AirwinRegistry::fxAlphaOrdering[index];
    auto &rg = AirwinRegistry::registry[rs];
    return rg.category + "/" + rg.name;
}

void AWConsolidatedAudioProcessor::changeProgramName(int index, const juce::String &newName) {}

//==============================================================================
void AWConsolidatedAudioProcessor::prepareToPlay(double sr, int samplesPerBlock)
{
    AirwinConsolidatedBase::defaultSampleRate = sr;
    if (awProcessor)
        awProcessor->setSampleRate(sr);
    isPlaying = true;
}

void AWConsolidatedAudioProcessor::releaseResources() { isPlaying = false; }

bool AWConsolidatedAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    bool inputValid = (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() ||
                       layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono());

    bool outputValid = layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();

    return inputValid && outputValid;
}

template<typename T>
void AWConsolidatedAudioProcessor::processBlockT(juce::AudioBuffer<T> &buffer)
{
    juce::ScopedNoDenormals noDenormals;

    ResetTypeMsg item;
    while (resetType.pop(item))
    {
        if (item.type == -1)
        {
            setAWProcessorTo(item.toIndex, false);
        }
    }

    if (!awProcessor)
    {
        isPlaying = false;
        return;
    }

    auto inBus = getBus(true, 0);
    auto outBus = getBus(false, 0);

    if (inBus->getNumberOfChannels() == 0 || outBus->getNumberOfChannels() != 2 ||
        buffer.getNumChannels() < 2)
    {
        isPlaying = false;
        return;
    }

    const T *inputs[2];
    T *outputs[2];
    inputs[0] = buffer.getReadPointer(0);
    inputs[1] =
        inBus->getNumberOfChannels() == 2 ? buffer.getReadPointer(1) : buffer.getReadPointer(0);
    outputs[0] = buffer.getWritePointer(0);
    outputs[1] = buffer.getWritePointer(1);

    if (!(inputs[0] && inputs[1] && outputs[0] && outputs[1]))
    {
        isPlaying = false;
        return;
    }

    isPlaying = true;

    for (int i = 0; i < nProcessorParams; ++i)
    {
        awProcessor->setParameter(i, fxParams[i]->get());
    }

    if constexpr (std::is_same_v<T, float>)
    {
        awProcessor->processReplacing((float **)inputs, (float **)outputs, buffer.getNumSamples());
    }
    else
    {
        awProcessor->processDoubleReplacing((double **)inputs, (double **)outputs, buffer.getNumSamples());
    }
}

void AWConsolidatedAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                                juce::MidiBuffer &midiMessages)
{
    processBlockT(buffer);
}

void AWConsolidatedAudioProcessor::processBlock(juce::AudioBuffer<double> &buffer, juce::MidiBuffer &)
{
    processBlockT(buffer);
}


//==============================================================================
bool AWConsolidatedAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AWConsolidatedAudioProcessor::createEditor()
{
    rebuildUI = true;
    return new AWConsolidatedAudioProcessorEditor(*this);
}

void AWConsolidatedAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    refreshUI = true;
}

void AWConsolidatedAudioProcessor::setAWProcessorTo(int registryIndex, bool initDisplay)
{
    curentProcessorIndex = registryIndex;
    auto rg = AirwinRegistry::registry[registryIndex];

    awProcessor = rg.generator();
    if (awProcessor)
    {
        awProcessor->setSampleRate(getSampleRate());
    }

    if (initDisplay)
    {
        LOCK(displayProcessorMutex);
        awDisplayProcessor = rg.generator();
        awDisplayProcessor->setSampleRate(getSampleRate());
    }

    if (initDisplay)
    {
        setupParamDisplaysFromDisplayProcessor(registryIndex);
    }
}

void AWConsolidatedAudioProcessor::setupParamDisplaysFromDisplayProcessor(int index)
{
    // Renoise re-enters to get text when you set value notifying host
    // so don't setvalue notifying host under the lock.
    // See https://forum.renoise.com/t/macos-crash-with-airwindows-vst-when-changing-presets/72288/11
    std::array<float, nAWParams> setParamsTo{};

    {
        LOCK(displayProcessorMutex);

        auto rg = AirwinRegistry::registry[index];

        nProcessorParams = rg.nParams;
        for (int i = 0; i < rg.nParams; ++i)
        {
            char txt[kVstMaxParamStrLen];
            awDisplayProcessor->getParameterName(i, txt);
            fxParams[i]->mutableName = txt;
            setParamsTo[i] = awDisplayProcessor->getParameter(i);
            defaultValues[i] = awDisplayProcessor->getParameter(i);
            active[i] = true;
        }
        for (int i = rg.nParams; i < nAWParams; ++i)
        {
            fxParams[i]->mutableName = "-";
            setParamsTo[i] = 0.f;
            active[i] = false;
        }
    }

    for (int i=0; i<nAWParams; ++i)
    {
        fxParams[i]->setValueNotifyingHost(setParamsTo[i]);
    }

    updateHostDisplay(juce::AudioProcessor::ChangeDetails().withParameterInfoChanged(true));
    rebuildUI = true;
}

//==============================================================================
void AWConsolidatedAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("awconsolidated"));
    xml->setAttribute("streamingVersion", (int)8524);

    xml->setAttribute("currentProcessorName", AirwinRegistry::registry[curentProcessorIndex].name);
    for (int i = 0; i < nAWParams; ++i)
    {
        juce::String nm = juce::String("awp_") + std::to_string(i);
        float val{0.f};
        if (i < nProcessorParams)
            val = *(fxParams[i]);

        xml->setAttribute(nm, val);
    }

    copyXmlToBinary(*xml, destData);
}

void AWConsolidatedAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("awconsolidated"))
        {
            auto streamingVersion = xmlState->getIntAttribute("streamingVersion", (int)2);
            auto effectName = xmlState->getStringAttribute("currentProcessorName");

            if (AirwinRegistry::nameToIndex.find(effectName.toStdString()) !=
                AirwinRegistry::nameToIndex.end())
            {
                setAWProcessorTo(AirwinRegistry::nameToIndex.at(effectName.toStdString()), true);
            }

            for (int i = 0; i < nAWParams; ++i)
            {
                juce::String nm = juce::String("awp_") + std::to_string(i);
                auto f = xmlState->getDoubleAttribute(nm);
                fxParams[i]->setValueNotifyingHost(f);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AWConsolidatedAudioProcessor();
}
