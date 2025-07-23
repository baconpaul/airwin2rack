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
                awDisplayProcessor->setParameter(i, f);
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

    bypassParam = new juce::AudioParameterBool({"bypass", 1}, "Bypass", 0);
    bypassParam->addListener(this);
    addParameter(bypassParam);

    inLev = new CubicDBParam({"inlev", 2}, "Input Level");
    inLev->addListener(this);
    outLev = new CubicDBParam({"outlev", 2}, "Output Level");
    outLev->addListener(this);
    addParameter(inLev);
    addParameter(outLev);

    monoBehaviourParameter = new MonoBehaviourParameter({"monoBehaviour", 3}, "Mono Behaviour", MonoBehaviourParameter::LeftOnly, juce::AudioParameterIntAttributes().withAutomatable(false));
    monoBehaviourParameter->addListener(this);
    addParameter(monoBehaviourParameter);

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

#if USE_JUCE_PROGRAMS
    updateHostDisplay(juce::AudioProcessor::ChangeDetails().withProgramChanged(true));
#endif
}

AWConsolidatedAudioProcessor::~AWConsolidatedAudioProcessor() {}

//==============================================================================
const juce::String AWConsolidatedAudioProcessor::getName() const { return JucePlugin_Name; }

bool AWConsolidatedAudioProcessor::acceptsMidi() const { return false; }

bool AWConsolidatedAudioProcessor::producesMidi() const { return false; }

bool AWConsolidatedAudioProcessor::isMidiEffect() const { return false; }

double AWConsolidatedAudioProcessor::getTailLengthSeconds() const { return 2.0; }

int AWConsolidatedAudioProcessor::getNumPrograms()
{
#if USE_JUCE_PROGRAMS
    return AirwinRegistry::registry.size();
#else
    return 1;
#endif
}

int AWConsolidatedAudioProcessor::getCurrentProgram()
{
#if USE_JUCE_PROGRAMS
    // not super efficient obvs
    int idx{0};
    for (auto &i : AirwinRegistry::fxAlphaOrdering)
    {
        if (i == curentProcessorIndex)
        {
            return idx;
        }
        idx++;
    }
    return 0;
#else
    return 0;
#endif
}

void AWConsolidatedAudioProcessor::setCurrentProgram(int index)
{
#if USE_JUCE_PROGRAMS
    auto rs = AirwinRegistry::fxAlphaOrdering[index];
    pushResetTypeFromUI(rs);
#endif
}

const juce::String AWConsolidatedAudioProcessor::getProgramName(int index)
{
#if USE_JUCE_PROGRAMS
    auto rs = AirwinRegistry::fxAlphaOrdering[index];
    auto &rg = AirwinRegistry::registry[rs];
    return rg.category + "/" + rg.name;
#else
    return "Airwindows Consolidated";
#endif
}

void AWConsolidatedAudioProcessor::changeProgramName(int index, const juce::String &newName) {}

//==============================================================================
void AWConsolidatedAudioProcessor::prepareToPlay(double sr, int samplesPerBlock)
{
    // Check for current AWProcessor it it supports mono. Otherwise chose something else...
    const auto isMono{ getTotalNumInputChannels()== 1 && getTotalNumOutputChannels() == 1 };
    const auto stereoPluginsInMono{ properties->getBoolValue("stereoPluginsInMono", true) };
    if (!stereoPluginsInMono && isMono && !AirwinRegistry::registry[curentProcessorIndex].isMono) {
        const auto defaultName = "Chamber"; // Mono reverb from the recommended list
        setAWProcessorTo(AirwinRegistry::nameToIndex.at(defaultName), true);
    }

    // Normally we could getProcessingPrecision() or isUsingDoublePrecision() and only
    // prepare the version we would need.
    // But clap-juce-extensions seems to have an issue here, where single precision
    // is always reported, even if later processBlock<double> is called.
    // So as a workaround we prepare both single and double precision...
    // See https://github.com/baconpaul/airwin2rack/issues/199
    const auto numCh = std::max(getTotalNumOutputChannels(), getTotalNumInputChannels());
    juce::dsp::ProcessSpec spec{ sr, static_cast<juce::uint32>(samplesPerBlock), static_cast<juce::uint32>(numCh)};
    if (isUsingDoublePrecision() || is_clap)
        getPrecisionDependantProcessing<double>().prepare(spec, inLev->getAmplitude<double>(), outLev->getAmplitude<double>());
    if (!isUsingDoublePrecision() || is_clap)
        getPrecisionDependantProcessing<float>().prepare(spec, inLev->getAmplitude<float>(), outLev->getAmplitude<float>());

    AirwinConsolidatedBase::defaultSampleRate = sr;
    if (awProcessor)
        awProcessor->setSampleRate(sr);
    isPlaying = true;
}

void AWConsolidatedAudioProcessor::releaseResources()
{
    getPrecisionDependantProcessing<float>().reset();
    getPrecisionDependantProcessing<double>().reset();
    isPlaying = false;
}

bool AWConsolidatedAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    bool inputValid = (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() ||
                       layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono());

    bool outputValid = (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() ||
                        layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono());

    bool testValid = layouts.getMainInputChannelSet().size() <= layouts.getMainOutputChannelSet().size();

    return inputValid && outputValid && testValid;
}

template <typename T> void AWConsolidatedAudioProcessor::processBlockT(juce::AudioBuffer<T> &buffer)
{
    juce::ScopedNoDenormals noDenormals;

    auto inBus = getBus(true, 0);
    auto outBus = getBus(false, 0);
    const auto numberOfInputChannels = inBus->getNumberOfChannels();
    const auto numberOfOutputChannels = outBus->getNumberOfChannels();

    if (numberOfInputChannels == 0 || numberOfOutputChannels == 0 ||
        buffer.getNumChannels() < std::max(numberOfInputChannels, numberOfInputChannels) )
    {
        isPlaying = false;
        return;
    }

    if (numberOfInputChannels == 1 && numberOfOutputChannels == 2)
    {
        // special case - mono to stereo. Copy buffer 1 to buffer 2 to emulated stereo to stereo
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
    }

    auto& precisionProcessing{ getPrecisionDependantProcessing<T>()};
    if (!precisionProcessing.isValid()) {
        isPlaying = false;
        return;
    }

    // TODO: Should we use resetType to communicate this change instead of storing member?
    if (currentBypass != bypassParam->get())
    {
        currentBypass = bypassParam->get();
        precisionProcessing.bypassCrossfader->setActiveBuffer(currentBypass ? Crossfader<T>::SecondaryBuffer : Crossfader<T>::PrimaryBuffer);
        if (!currentBypass) {
            setAWProcessorTo(curentProcessorIndex, false); // TODO: Very crude way of reseting AWProcessor!!
        }
    }

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

    for (int i = 0; i < nProcessorParams; ++i)
    {
        awProcessor->setParameter(i, fxParams[i]->get());
    }

    juce::dsp::AudioBlock<T> block(buffer);
    precisionProcessing.bypassCrossfader->pushSecondaryBuffer(block);

    // Save CPU cycles by not running the processing in bypass mode
    if (precisionProcessing.bypassCrossfader->getActiveBuffer() == Crossfader<T>::PrimaryBuffer
        || precisionProcessing.bypassCrossfader->fading())
    {
        precisionProcessing.inputGain->setGainLinear(inLev->getAmplitude<T>());
        precisionProcessing.inputGain->process(juce::dsp::ProcessContextReplacing<T>(block));

        // NOTE: Most Airwindows plugins take a copy of the L/R input sample before writing the output sample.
        // But some, like BitShiftPan, doesn't so giving the same buffer as both L and R causes some issues,
        // as the input buffer is overridden before the R channel is typically processed.
        // In mono input mode, we therefor take a copy of the input and use that.
        bool useMonoBuffer = (numberOfInputChannels == 1 && numberOfOutputChannels == 1);
        if (useMonoBuffer)
        {
            precisionProcessing.monoBuffer->copyFrom(0, 0, buffer, 0, 0, precisionProcessing.monoBuffer->getNumSamples());
        }

        const T *inputs[2];
        T *outputs[2];
        inputs[0] = buffer.getReadPointer(0);
        inputs[1] = !useMonoBuffer ? buffer.getReadPointer(1) : precisionProcessing.monoBuffer->getReadPointer(0);
        outputs[0] = buffer.getWritePointer(0);
        outputs[1] = !useMonoBuffer ? buffer.getWritePointer(1) : precisionProcessing.monoBuffer->getWritePointer(0);

        if (!(inputs[0] && inputs[1] && outputs[0] && outputs[1]))
        {
            isPlaying = false;
            return;
        }
        isPlaying = true;

        if constexpr (std::is_same_v<T, float>)
        {
            awProcessor->processReplacing((float **)inputs, (float **)outputs, buffer.getNumSamples());
        }
        else
        {
            awProcessor->processDoubleReplacing((double **)inputs, (double **)outputs, buffer.getNumSamples());
        }
        if (numberOfOutputChannels == 1 && *monoBehaviourParameter == MonoBehaviourParameter::LeftRightSum)
        {
            // Output = L+R / 2
            buffer.addFrom(0, 0, *precisionProcessing.monoBuffer, 0, 0, buffer.getNumSamples());
            buffer.applyGain(0.5);
        }
        // In LeftOnly mode, we don't need to do anything as the right monoBuffer is automatically discarded

        precisionProcessing.outputGain->setGainLinear(outLev->getAmplitude<T>());
        precisionProcessing.outputGain->process(juce::dsp::ProcessContextReplacing<T>(block));
    }

    precisionProcessing.bypassCrossfader->process(block);
}

void AWConsolidatedAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                                juce::MidiBuffer &midiMessages)
{
    processBlockT(buffer);
}

void AWConsolidatedAudioProcessor::processBlock(juce::AudioBuffer<double> &buffer,
                                                juce::MidiBuffer &)
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
    // See
    // https://forum.renoise.com/t/macos-crash-with-airwindows-vst-when-changing-presets/72288/11
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

    for (int i = 0; i < nAWParams; ++i)
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
    xml->setAttribute("inlev", inLev->get());
    xml->setAttribute("outlev", outLev->get());

    xml->setAttribute("monoBehaviour", monoBehaviourParameter->get());

    copyXmlToBinary(*xml, destData);
}

void AWConsolidatedAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("awconsolidated"))
        {
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

            auto il = xmlState->getDoubleAttribute("inlev", CubicDBParam::defaultVal);
            inLev->setValueNotifyingHost(il);
            auto ol = xmlState->getDoubleAttribute("outlev", CubicDBParam::defaultVal);
            outLev->setValueNotifyingHost(ol);

            auto mono = xmlState->getIntAttribute("monoBehaviour");
            *monoBehaviourParameter = static_cast<MonoBehaviourParameter::MonoBehaviour>(mono);
        }

#if USE_JUCE_PROGRAMS
        updateHostDisplay(juce::AudioProcessor::ChangeDetails().withProgramChanged(true));
#endif
        refreshUI = true;

        /*
         * What the heck is this? We called updateHostDisplay above already so we shouldn't
         * need this. But LIVE11 has some internal cache which it seems sticks the values
         * and ignores a call to update in setState. But if you just asunc re-call updateHostDisplay
         * then its internal cache says "all ok" but the display doesn't update.
         *
         * So the trick is, just in live, set the last param name to a random then back a
         * millisecond later, and things seem to work. For more information, see
         * https://github.com/baconpaul/airwin2rack/issues/112
         */
        if (juce::PluginHostType().isAbletonLive())
        {
            juce::Timer::callAfterDelay(1, [this]() {
                // grab the name
                auto mnCopy = fxParams[nAWParams - 1]->mutableName;
                // randomize and update
                fxParams[nAWParams - 1]->mutableName =
                    std::string("live_fix_") + std::to_string(rand());
                updateHostDisplay(
                    juce::AudioProcessor::ChangeDetails().withParameterInfoChanged(true));
                // and then later set it back
                juce::Timer::callAfterDelay(1, [this, mnCopy]() {
                    fxParams[nAWParams - 1]->mutableName = mnCopy;
                    updateHostDisplay(
                        juce::AudioProcessor::ChangeDetails().withParameterInfoChanged(true));
                });
            });
        }
    }
}

template<typename T>
void AWConsolidatedAudioProcessor::PrecisionDependantProcessing<T>::prepare(const juce::dsp::ProcessSpec& spec, T inputGainLinear, T outputGainLinear)
{
    monoBuffer.reset(new juce::AudioBuffer<T>(1, spec.maximumBlockSize));
    bypassCrossfader.reset(new Crossfader<T>());
    bypassCrossfader->prepare(spec);
    inputGain.reset(new juce::dsp::Gain<T>());
    inputGain->prepare(spec);
    inputGain->setGainLinear(inputGainLinear);
    inputGain->setRampDurationSeconds(0.01);
    inputGain->reset();
    outputGain.reset(new juce::dsp::Gain<T>());
    outputGain->prepare(spec);
    outputGain->setGainLinear(outputGainLinear);
    outputGain->setRampDurationSeconds(0.01);
    outputGain->reset();
}

template<typename T>
void AWConsolidatedAudioProcessor::PrecisionDependantProcessing<T>::reset()
{
    monoBuffer.reset();
    bypassCrossfader.reset();
    inputGain.reset();
    outputGain.reset();
}

template<typename T>
bool AWConsolidatedAudioProcessor::PrecisionDependantProcessing<T>::isValid() const
{
    return static_cast<bool>(monoBuffer) && static_cast<bool>(bypassCrossfader) && static_cast<bool>(inputGain) && static_cast<bool>(outputGain);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AWConsolidatedAudioProcessor();
}
