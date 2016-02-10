#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"
#include "ofxOsc.h"
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

    // MIDI

    ofxMidiIn       midiIn;

    void newMidiMessage(ofxMidiMessage& midiMessage);

    // GUI

    ofxPanel                    gui;
    vector<XBMidiPortParams>    portParams;
    ofxLabel                    lblStatus;
    ofxButton                   btnStart;
    ofxButton                   btnStop;

    stringstream                strMessage;
    unsigned int                numStrMessages;

    void startButtonPressed();
    void stopButtonPressed();

    // OSC

    ofxOscSender oscSender;
};
