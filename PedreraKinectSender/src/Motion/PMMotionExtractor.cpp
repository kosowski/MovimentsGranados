//
//  PMMotionExtractor.cpp
//  PMCancons_Visuals
//
//  Created by Eduard Frigola on 16/10/15.
//
//

#include "PMMotionExtractor.hpp"


///--------------------------------------------------------------
bool PMMotionExtractor::setup()
{
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    //    openNIDevice.setFrame(60);
    
    // setup the hand generator
    openNIDevice.addHandsGenerator();
    
    // add all focus gestures (ie., wave, click, raise arm)
    openNIDevice.addAllHandFocusGestures();
    
    // or you can add them one at a time
    //vector<string> gestureNames = openNIDevice.getAvailableGestures(); // you can use this to get a list of gestures
    // prints to console and/or you can use the returned vector
    //openNIDevice.addHandFocusGesture("Wave");
    
    openNIDevice.setMaxNumHands(2);
    
    for(int i = 0; i < openNIDevice.getMaxNumHands(); i++){
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
//        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
//        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true,
//        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false,
//        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }
    
    openNIDevice.start();
    
//    handsPosition.push_back(ofPoint(0,0,0));
//    handsPosition.push_back(ofPoint(0,0,0));
    
    return openNIDevice.getNumDevices();
}

///--------------------------------------------------------------
void PMMotionExtractor::update()
{
    openNIDevice.update();
    // iterate through users
    // only processes if two hands are found, it means a user is found
    if(openNIDevice.getNumTrackedHands()==2){
        if(!hasUser){
            hasUser=true;
            ofNotifyEvent(eventUserDetection, hasUser, this);
        }
        for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++){
            // get a reference to this user
            ofxOpenNIHand & hand = openNIDevice.getTrackedHand(i);
            
            // get hand position
            ofPoint handPos = hand.getPosition();
            handPos.x/=openNIDevice.getWidth(); //mapped to 0-1
            handPos.y/=openNIDevice.getHeight(); //mapped to 0-1
            handPos.z/=1000; //mapped to give distance to kinect in meters
            //Random choser left and right hand
            //TODO: implement right/left detection (IF NEED)
            if(i)
                handsPosition.leftHand.pos = handPos;
            else
                handsPosition.rightHand.pos = handPos;
        }
    }else{
        if(hasUser){
            hasUser=false;
            ofNotifyEvent(eventUserDetection, hasUser, this);
        }
    }
    
}

///--------------------------------------------------------------
void PMMotionExtractor::draw(bool drawDebug)
{
    openNIDevice.drawImage();
    if(drawDebug && hasUser){
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(ofColor::red);
        ofDrawCircle(handsPosition.rightHand.pos.x*openNIDevice.getWidth(), handsPosition.rightHand.pos.y*openNIDevice.getHeight(), 20);
        ofDrawCircle(handsPosition.leftHand.pos.x*openNIDevice.getWidth(), handsPosition.leftHand.pos.y*openNIDevice.getHeight(), 20);
        ofPopStyle();
    }
//    ofSetRectMode(OF_RECTMODE_CENTER);
//        kinectNI->drawImage(ofGetWidth()/2,ofGetHeight()/2, ofGetWidth(), ofGetHeight()*(kinectNI->getHeight()/kinectNI->getWidth()));
//    //kinectNI->drawImage();
//    ofSetRectMode(OF_RECTMODE_CORNER);
}

///--------------------------------------------------------------
void PMMotionExtractor::exit()
{
    openNIDevice.stop();
}

KinectInfo PMMotionExtractor::getKinectInfo()
{
    return kinectOut;
}


