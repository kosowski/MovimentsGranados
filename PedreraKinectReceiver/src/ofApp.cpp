#include "ofApp.h"
#include "../../Shared/OSCSettings.h"
#include "XBSettingsManager.h"

static const string STR_APP_TITLE           = "KINECT DETECTOR";
static const string STR_GUI_TITLE           = "Kinect Status";

static const string STR_STATE               = "STATUS";
static const string STR_STATE_SETUP         = "Waiting for Sender...";
static const string STR_STATE_DETECTING     = "Detecting Person...";
static const string STR_STATE_CAPTURING     = "Capturing";
static const string STR_BUTTON_RESTART      = "RESTART";
static const string STR_TOGGLE_ACTIVATE_IP  = "Activate Kinect View (Lower FPS)";
static const string STR_TOGGLE_SHOW_HANDS   = "Draw Hand Detection";
static const string STR_SLIDER_POS_THRES    = "Positioned Threshold";
static const string STR_SLIDER_HANDS_MEAN   = "Hands Mean Size";

static const string SETTINGS_FILENAME       = "settings.xml";
static const string STR_APPSETTINGS_FILENAME    = "AppSettings.xml";

static const int GUI_POSX = 10;
static const int GUI_POSY = 10;
static const int GUI_WIDTH = 350;


//--------------------------------------------------------------
void ofApp::setup(){
    
    // GUI
    {
        gui.setup(STR_GUI_TITLE, SETTINGS_FILENAME);
        gui.setPosition(GUI_POSX, GUI_POSY);
        gui.add(activateIPVideo.setup(STR_TOGGLE_ACTIVATE_IP, false));
        gui.add(positionedThreshold.setup(STR_SLIDER_POS_THRES, 0.1, 0.01, 1));
        gui.add(handsMeanSize.setup(STR_SLIDER_HANDS_MEAN, 5, 1, 20));
        gui.loadFromFile(SETTINGS_FILENAME);
        
        gui.add(guiStatusLbl.setup(STR_STATE, STR_STATE_SETUP));
        guiStatusLbl.setBackgroundColor(ofColor::darkRed);
        guiStatusLbl.setDefaultWidth(GUI_WIDTH);
        gui.add(guiRestartBtn.setup(STR_BUTTON_RESTART));
        
        guiRestartBtn.addListener(this, &ofApp::resetKinect);
        positionedThreshold.addListener(this, &ofApp::positionedThresholdChanged);
        handsMeanSize.addListener(this, &ofApp::handsMeanChanged);
        
        gui.setSize(GUI_WIDTH, GUI_WIDTH);
        gui.setWidthElements(GUI_WIDTH);
    }
    
    // OSC
    XBSettingsManager::getInstance().loadFile(STR_APPSETTINGS_FILENAME);
    string oscHost = XBSettingsManager::getInstance().getOSCHost();
    oscSender.setup(oscHost, OSC_KINECT_SENDER_PORT_RECEIVER);
    
	// listen on the given port
	cout << "listening for osc messages on port " << OSC_KINECT_SENDER_PORT_RECEIVER << "\n";
	receiver.setup(OSC_KINECT_SENDER_PORT_RECEIVER);

	ofBackground(30, 30, 130);
    detectionStatus = "Waiting for Sender";

    
    //Video grabber
    videoGrabber.setCameraName("Kinect");
    string videograbURI = "http://" + oscHost + ":" + to_string(OSC_KINECT_SENDER_PORT_VIDEO) + "/ipvideo";
    videoGrabber.setURI(videograbURI);
    videoGrabber.connect();
}

//--------------------------------------------------------------
void ofApp::update(){

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_STATE))
            stateChanged(m.getArgAsString(0));
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_LHAND+OSC_KINECT_ADDR_POSITION))
            handL = ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_RHAND+OSC_KINECT_ADDR_POSITION))
            handR = ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
	}
    
    //video grabber update
    if(activateIPVideo)
        videoGrabber.update();
    
    
    //check changes in activeIpVideo
    if(prevActiveIpVideo != activateIPVideo)
        activeIPVideoChanged();
    
    prevActiveIpVideo = activateIPVideo;
}


//--------------------------------------------------------------
void ofApp::draw(){
    int drawBoxHeight=ofGetHeight()-(float)ofGetWidth()/1920*1080;
    videoGrabber.draw(0,drawBoxHeight, ofGetWidth(), (float)ofGetWidth()/1920*1080);
    
    if(detectionStatus=="Capturing"){
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(ofColor::red);
        ofDrawCircle(handL.x * ofGetWidth(), handL.y * drawBoxHeight, 20);
        ofDrawCircle(handR.x * ofGetWidth(), handR.y * drawBoxHeight, 20);
        ofPopStyle();
    }
    ofPushStyle();
    ofSetColor(ofColor::red);
    ofDrawBitmapString("State: "+detectionStatus, 5, drawBoxHeight-20);
    ofPopStyle();
    ofSetBackgroundColor(ofColor::white);
    
    gui.draw();
}

//-------------------------------------------------------------
void ofApp::triggerUserPositioned(){
    ofSetBackgroundColor(ofColor::red);
}

//-------------------------------------------------------------
void ofApp::resetKinect(){
    ofxOscMessage m;
//    stringstream address;
//    address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_STATE;
//    m.setAddress(address.str());
    m.setAddress("/receiver/reset");
    oscSender.sendMessage(m, false);
}

void ofApp::stateChanged(string state){
    if(state == OSC_KINECT_STATE_POSITIONED)
        triggerUserPositioned();
    else if(state == OSC_KINECT_STATE_DETECTING){
        guiStatusLbl.setup(STR_STATE, STR_STATE_DETECTING);
        guiStatusLbl.setBackgroundColor(ofColor::darkBlue);
        detectionStatus = "Detecting";
    }
    else if(state == OSC_KINECT_STATE_CAPTURING){
        guiStatusLbl.setup(STR_STATE, STR_STATE_CAPTURING);
        guiStatusLbl.setBackgroundColor(ofColor::darkGreen);
        detectionStatus = "Capturing";
    }
}

void ofApp::activeIPVideoChanged(){
    cout<<activateIPVideo<<endl;
    videoGrabber.reset();
    videoGrabber.connect();
    ofxOscMessage m;
    m.setAddress("/receiver/ipvideo");
    m.addBoolArg(activateIPVideo);
    oscSender.sendMessage(m, false);

}

void ofApp::positionedThresholdChanged(float &threshold){
    ofxOscMessage m;
    m.setAddress("/receiver/positionedThreshold");
    m.addFloatArg(threshold);
    oscSender.sendMessage(m, false);
}

void ofApp::handsMeanChanged(int &meanSize){
    ofxOscMessage m;
    m.setAddress("/receiver/meanSize");
    m.addInt32Arg(meanSize);
    oscSender.sendMessage(m, false);
}

//-------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile(SETTINGS_FILENAME);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
