//
//  PMAudioAnalyzer.cpp
//  PMConcertParaules
//
//  Created by Miquel Àngel Soler on 21/10/15.
//
//

#include "PMAudioAnalyzer.hpp"

void PMAudioAnalyzer::init(
        float _silenceThreshold, unsigned int _silenceQueueLength,
        float _onsetsThreshold, int _ascDescAnalysisSize) {

    silenceThreshold = _silenceThreshold;
    silenceQueueLength = _silenceQueueLength;
    onsetsThreshold = _onsetsThreshold;
    ascDescAnalysisSize = _ascDescAnalysisSize;
}

PMDeviceAudioAnalyzer *PMAudioAnalyzer::addDeviceAnalyzer(unsigned int audioInputIndex, int deviceID, int inChannels, int outChannels,
        int sampleRate, int bufferSize,vector<unsigned int> channelNumbers)
{
    PMDeviceAudioAnalyzer *deviceAudioAnalyzer = new PMDeviceAudioAnalyzer(deviceID, inChannels, outChannels, sampleRate, bufferSize);
    deviceAudioAnalyzer->setup(audioInputIndex,
            channelNumbers,
            silenceThreshold,
            silenceQueueLength,
            onsetsThreshold,
            ascDescAnalysisSize);

    deviceAudioAnalyzers.push_back(deviceAudioAnalyzer);

    return deviceAudioAnalyzer;
}

vector<PMDeviceAudioAnalyzer *> *PMAudioAnalyzer::getAudioAnalyzers() {
    return &deviceAudioAnalyzers;
}

void PMAudioAnalyzer::start() {
    for (int i = 0; i < deviceAudioAnalyzers.size(); i++)
        deviceAudioAnalyzers[i]->start();
}

void PMAudioAnalyzer::stop() {
    for (int i = 0; i < deviceAudioAnalyzers.size(); i++)
        deviceAudioAnalyzers[i]->stop();
}

void PMAudioAnalyzer::clear() {
    stop();

    // Delete all device audio analyzers
    for (int i = 0; i < deviceAudioAnalyzers.size(); i++)
        deviceAudioAnalyzers[i]->clear();

    // Erase all device audio analyzers from vector
    deviceAudioAnalyzers.clear();
}

vector<ofSoundDevice> PMAudioAnalyzer::getInputDevices()
{
    ofSoundStream soundStream;
    vector<ofSoundDevice> allDevices = soundStream.getDeviceList();

#ifdef OF_DEBUG
    soundStream.printDeviceList();
#endif

    vector<ofSoundDevice> inputDevices;
    for (int i = 0; i < allDevices.size(); ++i)
        if (allDevices[i].inputChannels > 0)
            inputDevices.push_back(allDevices[i]);

    return inputDevices;
}
