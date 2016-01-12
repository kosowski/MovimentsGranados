//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "GUIApp.h"

static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS [GUI]";


void GUIApp::setup()
{
    ofSetWindowTitle(STR_WINDOW_TITLE);
    ofBackground(68, 84, 71);
}

void GUIApp::update()
{
}

void GUIApp::draw()
{
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", 15, ofGetHeight() - 15);

    int fontSize = 8;

    string msg1 = "NOTHING TO SEE HERE (for the moment)";
    int msg1Width = int(msg1.size()) * fontSize;
    ofDrawBitmapStringHighlight(msg1, ofGetWidth()/2 - msg1Width/2, ofGetHeight()/2 - fontSize, ofColor::black, ofColor(255, 170, 170));

    string msg2 = "Expect to see SCENE GUIs in this window";
    int msg2Width = int(msg2.size()) * fontSize;
    ofDrawBitmapStringHighlight(msg2, ofGetWidth()/2 - msg2Width/2, ofGetHeight()/2 + fontSize, ofColor::black, ofColor::white);
}

void GUIApp::exit()
{
}
