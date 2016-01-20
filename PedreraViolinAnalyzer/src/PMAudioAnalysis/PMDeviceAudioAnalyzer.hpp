//
//  PMAudioAnalyzer.hpp
//  ConcertParaules
//
//  Created by Miquel Àngel Soler on 25/9/15.
//
//

#pragma once

#ifndef PMDeviceAudioAnalyzer_hpp
#define PMDeviceAudioAnalyzer_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxAubio.h"
#include "PMAudioInParams.h"



class PMDeviceAudioAnalyzer : public ofBaseSoundInput
{
public:

    PMDeviceAudioAnalyzer(int deviceID, int channelID, int outChannels, int sampleRate, int bufferSize, int inChannels);
    PMDeviceAudioAnalyzer() {};
    ~PMDeviceAudioAnalyzer();

    void setup(unsigned int audioInputIndex, vector<unsigned int> channelNumbers,
            float silenceThreshold, unsigned int silenceQueueLength,
            float onsetsThreshold,
            int ascDescAnalysisSize);

    void start();
    void stop();
    void clear();

    void audioIn(float *input, int bufferSize, int nChannels);

    unsigned int            getInputIndex()     { return audioInputIndex; };
    int                     getDeviceID()       { return deviceID; };
    int                     getSampleRate()     { return sampleRate; };
    int                     getNumChannels()    { return inChannels; };
    vector<unsigned int>    getChannelNumbers() { return channelNumbers; };

    void                    setSilenceThreshold(float value)   { silenceThreshold = value; };
    void                    setSilenceQueueLength(float value) { silenceTimeTreshold = value; };
    void                    setPauseTimeTreshold(float value)  { pauseTimeTreshold = value; };
    void                    setDigitalGain(float value) { digitalGain = value; };
    void                    setOnsetsThreshold(float value);
    void                    setMinPitch(float value)    {pParams.min=value;};
    void                    setMaxPitch(float value)    {pParams.max=value;};
    void                    setMinEnergy(float value)   {eParams.min=value;};
    void                    setMaxEnergy(float value)   {eParams.max=value;};
    void                    setDeltaEnergy(float value)  {eParams.deltaEnergy=value;};
    void                    setDeltaPitch(float value)   {pParams.deltaPitch=value;};
    void                    setSmoothedPitch(float value) {pParams.smoothedPitch=value;};
    void                    setSmoothedEnergy(float value) {eParams.smoothedEnergy=value;};

    // Events for listeners
    ofEvent<pitchParams> eventPitchChanged;
    ofEvent<silenceParams> eventSilenceStateChanged;
    ofEvent<energyParams> eventEnergyChanged;
    ofEvent<onsetParams> eventOnsetStateChanged;
    ofEvent<freqBandsParams> eventFreqBandsParams;
    ofEvent<shtParams> eventShtStateChanged;
    ofEvent<pauseParams> eventPauseStateChanged;
    ofEvent<melodyDirectionParams> eventMelodyDirection;

private:

    /**/
    float *channelInput;

    // Setup
    unsigned int audioInputIndex;
    int deviceID;
    int channelID;
    int inChannels;
    int outChannels;
    int sampleRate;
    int bufferSize;
    int numBuffers;

    vector<unsigned int> channelNumbers;

    // Onsets
    float onsetsThreshold;
    bool oldOnsetState;

    // Smoothing
    deque<float> midiNoteHistory;

    // Sound analysis

    ofSoundStream soundStream;

    ofxAubioPitch *aubioPitch;
    ofxAubioOnset *aubioOnset;
    ofxAubioMelBands *aubioMelBands;

    bool isSetup;

    // Silence
    bool wasSilent;
    float silenceThreshold;
    float oldTimeOfSilence;

    bool alreadyInSilence;
    bool isInPause;
    float notSilenceBeginTime;
    float silenceTimeTreshold;
    float pauseTimeTreshold;
    float digitalGain;

    float getEnergy();
    float getRms(float *input, int bufferSize);
    float getAbsMean(float *input, int bufferSize);
    void silenceStarted();
    void notifySilence(bool b);
    void silenceStopped();
    void checkMelodyDirection();

    int ascDescAnalysisSize;

    // sshht
    bool isShtSounding;
    float shtBeginTime;
    float shtTimeTreshold;
    bool isShtTrueSent;
    bool isShtFalseSent;

    void checkShtSound();

    pitchParams pParams;
    energyParams eParams;
    float      oldPitch;
    float      oldEnergy;
    float      deltaConfidence;
    float      oldPitchForConfidence;
};

#endif /* PMDeviceAudioAnalyzer_h */
