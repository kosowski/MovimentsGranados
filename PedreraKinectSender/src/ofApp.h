#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxOsc.h"

typedef enum
{
    STATE_SETUP = 0,
    STATE_DETECTING = 1,
    STATE_CAPTURING = 2
} KinectSenderState;

class ofApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void handleStateChanges();

    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

private:

    KinectSenderState currState, prevState;

    ofxPanel gui;
    ofxLabel guiStatusLbl;
    ofxButton guiRestartBtn;

    ofxOscSender oscSender;
};
