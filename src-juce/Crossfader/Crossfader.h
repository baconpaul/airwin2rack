
#ifndef AWCONSOLIDATED_CROSSFADER_H
#define AWCONSOLIDATED_CROSSFADER_H

#include "juce_dsp/juce_dsp.h"

/**
    Crossfader class that simply crossfades between two inputs.
    The fading is smoothed over 10ms to avoid clicks
 */
template <typename SampleType>
class Crossfader
{
public:
    //==============================================================================
    Crossfader() {};
    ~Crossfader() = default;

    //==============================================================================
    enum ActiveBuffer
    {
        PrimaryBuffer,
        SecondaryBuffer
    };

    //==============================================================================
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        primaryBufferGain.reset(spec.sampleRate, 0.01);
        primaryBufferGain.setCurrentAndTargetValue(1.0);
        secondaryBufferGain.reset(spec.sampleRate, 0.01);
        secondaryBufferGain.setCurrentAndTargetValue(0.0);
        localSecondaryBuffer.setSize(static_cast<int>(spec.numChannels), static_cast<int>(spec.maximumBlockSize), false, false, true);
    }

    void setActiveBuffer(ActiveBuffer activeBuffer)
    {
        primaryBufferGain.setTargetValue(activeBuffer == ActiveBuffer::PrimaryBuffer ? 1.0 : 0.0);
        secondaryBufferGain.setTargetValue(activeBuffer == ActiveBuffer::SecondaryBuffer ? 1.0 : 0.0);
    }

    ActiveBuffer getActiveBuffer() const
    {
        return primaryBufferGain.getTargetValue() == 1.0 ? ActiveBuffer::PrimaryBuffer : ActiveBuffer::SecondaryBuffer;
    }

    bool fading() const
    {
        return primaryBufferGain.isSmoothing() || secondaryBufferGain.isSmoothing();
    }

    void pushSecondaryBuffer(const juce::dsp::AudioBlock<const SampleType> secondaryBuffer)
    {
        localSecondaryBuffer.setSize(secondaryBuffer.getNumChannels(), secondaryBuffer.getNumSamples(), false, false, true);
        juce::dsp::AudioBlock<SampleType> block(localSecondaryBuffer);
        block.copyFrom(secondaryBuffer);
    }

    /**
        Perform the crossfade between primary and and secondary according to the active buffer.
     */
    void process(juce::dsp::AudioBlock<SampleType> primaryBuffer)
    {
        juce::dsp::AudioBlock<SampleType> secondaryBlock(localSecondaryBuffer);
        if (fading())
        {
            primaryBuffer.multiplyBy(primaryBufferGain);
            secondaryBlock.multiplyBy(secondaryBufferGain);
            primaryBuffer.add(secondaryBlock);
        } else {
            if (getActiveBuffer() == ActiveBuffer::SecondaryBuffer)
            {
                // Simply copy secondary buffer to primary
                primaryBuffer.copyFrom(secondaryBlock);
            }
        }
    }

private:
    juce::SmoothedValue<double, juce::ValueSmoothingTypes::Linear> primaryBufferGain;
    juce::SmoothedValue<double, juce::ValueSmoothingTypes::Linear> secondaryBufferGain;
    juce::AudioBuffer<SampleType> localSecondaryBuffer;
};

#endif // AWCONSOLIDATED_CROSSFADER_H
