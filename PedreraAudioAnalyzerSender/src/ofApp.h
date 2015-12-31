#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
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
    ofParameter<bool>       silenceOn;
    ofParameterGroup        paramsPause;
    ofParameter<float>      pauseLength;
    ofParameter<bool>       pauseOn;
    ofParameterGroup        paramsOnsets;
    ofParameter<float>      onsetsThreshold;
    ofParameter<bool>       onsetsOn;

    void buildDevicesPanel();
    void buildAnalysisPanel();

    void startButtonPressed();
    void stopButtonPressed();
};
