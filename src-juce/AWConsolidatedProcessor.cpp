/*
 * Surge XT - a free and open source hybrid synthesizer,
 * built by Surge Synth Team
 *
 * Learn more at https://surge-synthesizer.github.io/
 *
 * Copyright 2018-2024, various authors, as described in the GitHub
 * transaction log.
 *
 * Surge XT is released under the GNU General Public Licence v3
 * or later (GPL-3.0-or-later). The license is found in the "LICENSE"
 * file in the root of this repository, or at
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 * Surge was a commercial product from 2004-2018, copyright and ownership
 * held by Claes Johanson at Vember Audio during that period.
 * Claes made Surge open source in September 2018.
 *
 * All source for Surge XT is available at
 * https://github.com/surge-synthesizer/surge
 */

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
    Airwin2RackBase::defaultSampleRate = 48000;

    // Multiple calls to addParameter here
    for (int i = 0; i < nAWParams; ++i)
    {
        fxParams[i] = new AWParam(juce::ParameterID(std::string("ctrl_") + std::to_string(i), 1), "Name",
                                  juce::NormalisableRange<float>(0.f, 1.f), 0.f);
        fxParams[i]->getTextHandler = [i, this](auto f, auto iv) {
            std::lock_guard<std::mutex> g(this->displayProcessorMutex);
            if (this->awDisplayProcessor && i < this->nProcessorParams)
            {
                for (int id = 0; id < this->nProcessorParams; ++id)
                {
                    awDisplayProcessor->setParameter(id, fxParams[id]->get());
                }
                char tl[kVstMaxParamStrLen], td[kVstMaxParamStrLen];
                this->awDisplayProcessor->getParameterDisplay(i, td);
                this->awDisplayProcessor->getParameterLabel(i, tl);
                return std::string(td) + (tl[0] == 0 ? "" : " " ) + std::string(tl);
            }
            else
            {
                return std::string("-");
            }
        };
        fxParams[i]->getTextToValue = [i, this](auto s) {
            std::lock_guard<std::mutex> g(this->displayProcessorMutex);
            if (this->awDisplayProcessor && i < this->nProcessorParams)
            {
                float rv = 0.f;
                auto res = this->awDisplayProcessor->parameterTextToValue(i, s.toStdString().c_str(), rv);
                if (res)
                    return rv;
            }
            return this->fxParams[i]->get();
        };
        fxParams[i]->addListener(this);
        addParameter(fxParams[i]);
    }

    setAWProcessorTo(AirwinRegistry::nameToIndex.at("Galactic"), true);
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
    return 2; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int AWConsolidatedAudioProcessor::getCurrentProgram() { return 0; }

void AWConsolidatedAudioProcessor::setCurrentProgram(int index) {}

const juce::String AWConsolidatedAudioProcessor::getProgramName(int index)
{
    return "Default " + std::to_string(index);
}

void AWConsolidatedAudioProcessor::changeProgramName(int index, const juce::String &newName) {}

//==============================================================================
void AWConsolidatedAudioProcessor::prepareToPlay(double sr, int samplesPerBlock)
{
    Airwin2RackBase::defaultSampleRate = sr;
    if (awProcessor)
        awProcessor->setSampleRate(sr);
    isPlaying = true;
}

void AWConsolidatedAudioProcessor::releaseResources()
{
    isPlaying = false;
}

bool AWConsolidatedAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    bool inputValid = (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() ||
                       layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono());

    bool outputValid = layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();

    return inputValid && outputValid;
}

void AWConsolidatedAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                                juce::MidiBuffer &midiMessages)
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

    if (inBus->getNumberOfChannels() == 0 ||
        outBus->getNumberOfChannels() != 2 ||
        buffer.getNumChannels() < 2)
    {
        isPlaying = false;
        return;
    }


    const float *inputs[2];
    float *outputs[2];
    inputs[0] = buffer.getReadPointer(0);
    inputs[1] = inBus->getNumberOfChannels() == 2 ? buffer.getReadPointer(1) : buffer.getReadPointer(0);
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

    awProcessor->processReplacing((float **)inputs,
                                  (float **)outputs,
                                  buffer.getNumSamples());
}

//==============================================================================
bool AWConsolidatedAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *AWConsolidatedAudioProcessor::createEditor()
{
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
        nProcessorParams = rg.nParams;
        for (int i = 0; i < rg.nParams; ++i)
        {
            char txt[kVstMaxParamStrLen];
            awProcessor->getParameterName(i, txt);
            fxParams[i]->mutableName = txt;
            fxParams[i]->setValueNotifyingHost(awProcessor->getParameter(i));
            defaultValues[i] = awProcessor->getParameter(i);
            active[i] = true;
        }
        for (int i = rg.nParams; i < nAWParams; ++i)
        {
            fxParams[i]->mutableName = "-";
            fxParams[i]->setValueNotifyingHost(0.f);
            active[i] = false;
        }
    }

    if (initDisplay)
    {
        std::lock_guard<std::mutex> g(displayProcessorMutex);
        awDisplayProcessor = rg.generator();
        awDisplayProcessor->setSampleRate(getSampleRate());
    }

    updateHostDisplay(juce::AudioProcessor::ChangeDetails().withParameterInfoChanged(true));
    refreshUI = true;
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

            if (AirwinRegistry::nameToIndex.find(effectName.toStdString())
                != AirwinRegistry::nameToIndex.end())
            {
                setAWProcessorTo(AirwinRegistry::nameToIndex.at(effectName.toStdString()), true);
            }

            for (int i=0; i<nAWParams; ++i)
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
