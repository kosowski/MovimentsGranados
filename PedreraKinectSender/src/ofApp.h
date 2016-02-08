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
    
    void keyPressed(int key);

    void handleStateChanges();
    void userDetection(bool &hasUser);
    void sendHandInfo();
    void resetKinect();

private:

    KinectSenderState currState, prevState;

    ofxPanel gui;
    ofxLabel guiStatusLbl;
    ofxButton guiRestartBtn;
    ofxToggle showImage;
    ofxToggle showHands;

    ofxOscSender oscSender, oscSender_Max;
    
    PMMotionExtractor* motionExtractor;
    
    bool kinectAvailable;
    
    KinectInfo  handsInfo;
};
