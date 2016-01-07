#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"
#include "XBMidiPortParams.h"

class ofApp : public ofBaseApp, public ofxMidiListener
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyReleased(int key);

private:

    ofxMidiIn       midiIn;
    ofxMidiMessage  midiMessage;

    ofxPanel                    gui;
    vector<XBMidiPortParams>    portParams;
    ofxLabel                    lblStatus;
    ofxButton                   btnStart;
    ofxButton                   btnStop;

    void startButtonPressed();
    void stopButtonPressed();

    void newMidiMessage(ofxMidiMessage& eventArgs);
};
