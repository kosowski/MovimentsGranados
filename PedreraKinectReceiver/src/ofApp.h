#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "IPVideoGrabber.h"

// listen on port 12345
#define PORT 30304
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
    void exit();
    
        void triggerUserPositioned();
        void stateChanged(string state);
    void activeIPVideoChanged();
    void positionedThresholdChanged(float &threshold);
    void handsMeanChanged(int &meanSize);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void resetKinect();

		ofTrueTypeFont font;
		ofxOscReceiver receiver;


        ofPoint handR, handL;
        string detectionStatus;
        bool userPositioned;
    
    //GUI
    ofxPanel gui;
    ofxLabel guiStatusLbl;
    ofxButton guiRestartBtn;
    ofxToggle activateIPVideo;
    ofxFloatSlider positionedThreshold;
    ofxIntSlider handsMeanSize;
    
    bool prevActiveIpVideo;
    
    
    //OSC
    ofxOscSender oscSender;
    
    
    //Video Grabbing
    ofx::Video::IPVideoGrabber videoGrabber;
};
