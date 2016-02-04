#include "ofApp.h"
#include "../../Shared/OSCSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	ofBackground(30, 30, 130);
    detectionStatus = "Waiting for Sender";
    
    //Video grabber
    videoGrabber.setCameraName("Kinect");
    videoGrabber.setURI("http://192.168.1.112:7890/ipvideo");
    videoGrabber.connect();
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
            auto receivedMessage = m.getArgAsString(0);
            if (receivedMessage != OSC_KINECT_STATE_POSITIONED)
                detectionStatus =receivedMessage;
            else
                triggerUserPositioned();
        }
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_LHAND+OSC_KINECT_ADDR_POSITION))
            handL = ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
        if(m.getAddress() == (OSC_KINECT_ADDR_BASE+OSC_KINECT_ADDR_RHAND+OSC_KINECT_ADDR_POSITION))
            handR = ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
	}
    
    //video grabber update
    videoGrabber.update();
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
