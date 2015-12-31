#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "PMAudioAnalyzer.hpp"
#include "XBDeviceParams.h"

class ofApp : public ofBaseApp
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

    // Audio analysis
    ofxPanel                guiAnalysis;
    ofParameterGroup        paramsPitch;
    ofParameter<float>      pitchMidiNote;
    ofParameterGroup        paramsEnergy;
    ofParameter<float>      energyEnergy;
    ofParameter<float>      energyGain;
    ofParameterGroup        paramsSilence;
    ofParameter<float>      silenceThreshold;
    ofParameter<float>      silenceLength;
//    ofxLabel                silenceOn;
    ofParameter<bool>       silenceOn;
    ofParameterGroup        paramsPause;
    ofParameter<float>      pauseLength;
//    ofxLabel                pauseOn;
    ofParameter<bool>       pauseOn;
    ofParameterGroup        paramsOnsets;
    ofParameter<float>      onsetsThreshold;
//    ofxLabel                onsetsOn;
    ofParameter<bool>       onsetsOn;

    void buildDevicesPanel();
    void buildAnalysisPanel();

    // GUI EVENTS

    void startButtonPressed();
    void stopButtonPressed();
    void silenceThresholdChanged(float &threshold);
    void silenceLengthChanged(float &length);
    void pauseLengthChanged(float &length);
    void onsetsThresholdChanged(float &threshold);

    // AUDIO ANALYZER

    vector<PMDeviceAudioAnalyzer *> *audioAnalyzers;

    void pitchChanged(pitchParams &pitchParams);
    void energyChanged(energyParams &energyParams);
    void silenceStateChanged(silenceParams &silenceParams);
    void pauseStateChanged(pauseParams &pauseParams);
    void onsetDetected(onsetParams &onsetParams);
};
