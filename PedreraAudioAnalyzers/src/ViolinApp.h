#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxOsc.h"
#include "PMAudioAnalyzer.hpp"
#include "XBDeviceParams.h"

class ViolinApp : public ofBaseApp
{

public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyReleased(int key);

private:

    // GUI

    // Audio device selector
    ofxPanel                guiDevices;
    vector<XBDeviceParams>  deviceParams;
    ofxButton               btnStartAnalysis;
    ofxButton               btnStopAnalysis;
    ofxLabel                lblStatus;

    // Audio analysis (violin)
    ofxPanel                guiAnalysis;
    ofParameterGroup        paramsPitch;
    ofParameter<float>      pitchCurrentNote;
    ofParameter<float>      pitchSmoothedNote;
    ofParameter<float>      pitchSmoothAmount;
    ofParameter<float>      pitchMidiMin;
    ofParameter<float>      pitchMidiMax;
    ofParameterGroup        paramsEnergy;
    ofParameter<float>      energyEnergy;
    ofParameter<float>      energySmoothed;
    ofParameter<float>      energySmoothAmount;
    ofParameter<float>      energyMin;
    ofParameter<float>      energyMax;
    ofParameter<float>      digitalGain;
    ofParameterGroup        paramsSilence;
    ofParameter<float>      silenceThreshold;
    ofParameter<float>      silenceLength;
    ofParameter<bool>       silenceOn;
    ofParameterGroup        paramsOnsets;
    ofParameter<float>      onsetsThreshold;
    ofParameter<bool>       onsetsOn;

    void buildDevicesPanel();
    void buildCelloAnalysisPanel();

    // GUI EVENTS

    void startButtonPressed();
    void stopButtonPressed();

    void guiPitchSmoothAmountChanged(float &smoothAmount);
    void guiPitchMinChanged(float &pitch);
    void getPitchMaxChanged(float &pitch);
    void guiEnergySmoothAmountChanged(float &smoothAmount);
    void guiEnergyMinChanged(float &energy);
    void guiEnergyMaxChanged(float &energy);
    void guiDigitalGainChanged(float &digitalGain);
    void guiSilenceThresholdChanged(float &threshold);
    void guiSilenceLengthChanged(float &length);
    void guiOnsetsThresholdChanged(float &threshold);

    // AUDIO ANALYZER

    vector<PMDeviceAudioAnalyzer *> *audioAnalyzers;

    void analyzerPitchChanged(pitchParams &pitchParams);
    void analyzerEnergyChanged(energyParams &energyParams);
    void analyzerSilenceStateChanged(silenceParams &silenceParams);
    void analyzerOnsetDetected(onsetParams &onsetParams);

    // OSC

    ofxOscSender oscSender;
};
