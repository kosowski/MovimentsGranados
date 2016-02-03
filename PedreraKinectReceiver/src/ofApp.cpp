#include "ofApp.h"
#include "../../Shared/OSCSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	ofBackground(30, 30, 130);
    detectionStatus = OSC_KINECT_STATE_SETUP;

}

//--------------------------------------------------------------
void ofApp::update(){

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
//        cout<<m.getAddress()<<endl;
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_STATE)){
            if (m.getArgAsString(0) != OSC_KINECT_STATE_POSITIONED)
                detectionStatus = m.getArgAsString(0);
            else
                triggerUserPositioned();
        }
            detectionStatus = m.getArgAsString(0);
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_LHAND+OSC_KINECT_ADDR_POSITION))
            handL = ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_LHAND+OSC_KINECT_ADDR_POSITION))
            handR = ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    
    if(detectionStatus=="Capturing"){
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(ofColor::red);
        ofDrawCircle(handL.x * ofGetWidth(), handL.y * ofGetHeight(), 20);
        ofDrawCircle(handR.x * ofGetWidth(), handR.y * ofGetHeight(), 20);
        ofPopStyle();
    }
    ofSetColor(ofColor::red);
    ofDrawBitmapString("State: "+detectionStatus, 5, ofGetWindowHeight()-20);
    ofSetBackgroundColor(ofColor::white);

}

//-------------------------------------------------------------
void ofApp::triggerUserPositioned(){
    ofSetBackgroundColor(ofColor::red);
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
