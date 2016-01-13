#pragma once

#include "ofMain.h"
#include "ofxGUI.h"
#include "ofxOsc.h"
#include "PMMotionExtractor.hpp"

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
    void userDetection(bool &hasUser);
    void sendHandInfo();

    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

private:

    KinectSenderState currState, prevState;

    ofxPanel gui;
    ofxLabel guiStatusLbl;
    ofxButton guiRestartBtn;
    ofxToggle showImage;
    ofxToggle showHands;

    ofxOscSender oscSender;
    
    PMMotionExtractor* motionExtractor;
    
    bool kinectAvailable;
    
    KinectInfo  handsInfo;
};
