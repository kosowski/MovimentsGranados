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

    ofxPanel                guiAudioDevices;
    vector<XBDeviceParams>  deviceParams;
    ofxButton               btnStartAnalysis;
    ofxButton               btnStopAnalysis;
    ofxLabel                lblStatus;

    void startButtonPressed();
    void stopButtonPressed();
};
