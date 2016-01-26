#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	mouseButtonState = "";

	ofBackground(30, 30, 130);

}

//--------------------------------------------------------------
void ofApp::update(){

	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
//        cout<<m.getAddress()<<endl;
        if(m.getAddress() == "/motion/detectionStatus")
            detectionStatus=m.getArgAsString(0); cout<<detectionStatus<<endl;
        if(m.getAddress() == "/motion/leftHand/position")
            handL=ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
        if(m.getAddress() == "/motion/rightHand/position")
            handR=ofPoint(m.getArgAsFloat(0), m.getArgAsFloat(1));
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

    
//	string buf;
//	buf = "listening for osc messages on port" + ofToString(PORT);
//	ofDrawBitmapString(buf, 10, 20);
//    
//    if(receivedImage.getWidth() > 0){
//        ofDrawBitmapString("Image:", 10, 160);
//        receivedImage.draw(10, 180);
//    }
//
//	// draw mouse state
//	buf = "mouse: " + ofToString(mouseX, 4) +  " " + ofToString(mouseY, 4);
//	ofDrawBitmapString(buf, 430, 20);
//	ofDrawBitmapString(mouseButtonState, 580, 20);
//
//	for(int i = 0; i < NUM_MSG_STRINGS; i++){
//		ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
//	}
//
    

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
