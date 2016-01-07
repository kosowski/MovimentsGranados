#include "ofApp.h"

static const string STR_APP_TITLE = "PIANO [MIDI RECEIVER]";

void ofApp::setup()
{
    ofSetWindowTitle(STR_APP_TITLE);
    ofBackground(81, 111, 88);

#ifdef OF_DEBUG
    midiIn.listPorts();
#endif
}

void ofApp::update()
{
}

void ofApp::draw()
{
}

void ofApp::keyReleased(int key)
{
}
