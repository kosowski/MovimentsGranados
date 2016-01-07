#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();

    void keyReleased(int key);

private:

    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
};
