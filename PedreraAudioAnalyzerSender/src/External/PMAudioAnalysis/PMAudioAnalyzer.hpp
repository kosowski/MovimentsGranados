//
//  PMAudioAnalyzer.hpp
//  PMConcertParaules
//
//  Created by Miquel Àngel Soler on 21/10/15.
//
//

#ifndef PMAudioAnalyzer_hpp
#define PMAudioAnalyzer_hpp

#include <stdio.h>
#include "PMDeviceAudioAnalyzer.hpp"


class PMAudioAnalyzer
{
public:

    static PMAudioAnalyzer &getInstance()
    {
        static PMAudioAnalyzer instance;
        return instance;
    }

    void init(float silenceThreshold, unsigned int silenceQueueLength,
            float onsetsThreshold, int ascDescAnalysisSize);

    PMDeviceAudioAnalyzer *addDeviceAnalyzer(unsigned int audioInputIndex, int deviceID, int inChannels, int outChannels,
            int sampleRate, int bufferSize, vector<unsigned int> channelNumbers);

    vector<PMDeviceAudioAnalyzer *> *getAudioAnalyzers();

    void start();
    void stop();

    void clear();

    static vector<ofSoundDevice> getInputDevices();

private:

    // Silence
    float           silenceThreshold;
    unsigned int    silenceQueueLength;

    // Onsets
    float           onsetsThreshold;
    float           onsetsAlpha;

    // Ascendent Descendent Melody analysis
    int             ascDescAnalysisSize;

    vector<PMDeviceAudioAnalyzer *> deviceAudioAnalyzers;
};


#endif /* PMAudioAnalyzer_hpp */
