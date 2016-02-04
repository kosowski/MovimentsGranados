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

    for (int i = 0; i < openNIDevice.getMaxNumHands(); i++) {
        ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
//        // ofxOpenNIDepthThreshold is overloaded, has defaults and can take a lot of different parameters, eg:
//        // (ofxOpenNIROI OR) int _nearThreshold, int _farThreshold, bool _bUsePointCloud = false, bool _bUseMaskPixels = true,
//        // bool _bUseMaskTexture = true, bool _bUseDepthPixels = false, bool _bUseDepthTexture = false,
//        // int _pointCloudDrawSize = 2, int _pointCloudResolution = 2
        openNIDevice.addDepthThreshold(depthThreshold);
    }

    openNIDevice.start();

    hasKinect = (openNIDevice.getNumDevices() >= 1);
    return hasKinect;
}

///--------------------------------------------------------------
void PMMotionExtractor::update()
{
    if(hasKinect){
        openNIDevice.update();
        // iterate through users
        // only processes if two hands are found, it means a user is found
        if (openNIDevice.getNumTrackedHands() == 2) {
            if (!hasUser) {
                hasUser = true;
                ofNotifyEvent(eventUserDetection, hasUser, this);
            }
            for (int i = 0; i < openNIDevice.getNumTrackedHands(); i++) {
                // get a reference to this user
                ofxOpenNIHand &hand = openNIDevice.getTrackedHand(i);
                
                // get hand position
                ofPoint handPos = hand.getPosition();
                handPos.x /= openNIDevice.getWidth(); //mapped to 0-1
                handPos.y /= openNIDevice.getHeight(); //mapped to 0-1
                handPos.z /= 1000; //mapped to give distance to kinect in meters
                //Random choser left and right hand
                //TODO: implement right/left detection (IF NEED)
                if (i)
                    handsInfo.leftHand.pos = handPos;
                else
                    handsInfo.rightHand.pos = handPos;
            }
            computeVelocity(5);
        } else {
            if (hasUser) {
                hasUser = false;
                ofNotifyEvent(eventUserDetection, hasUser, this);
                rHandPosHist.clear();
                lHandPosHist.clear();
            }
        }
    }else if(ofGetMousePressed()){
        hasUser = true;
        ofNotifyEvent(eventUserDetection, hasUser, this);
        handsInfo.leftHand.pos.x = (float)ofGetMouseX() / (float)ofGetWidth();
        handsInfo.leftHand.pos.y = (float)ofGetMouseY() / (float)ofGetHeight();
        handsInfo.leftHand.pos.z = 1;
        handsInfo.rightHand.pos.x = ofMap(handsInfo.leftHand.pos.x, 0, 1, 1, 0);
        handsInfo.rightHand.pos.y = handsInfo.leftHand.pos.y;
        handsInfo.leftHand.pos.z = 1;
        computeVelocity(5);
    }else{
        hasUser = false;
        ofNotifyEvent(eventUserDetection, hasUser, this);
        rHandPosHist.clear();
        lHandPosHist.clear();
    }
    
}

///--------------------------------------------------------------
void PMMotionExtractor::draw(bool drawImage, bool drawHands)
{
    if(hasKinect && drawImage)
        openNIDevice.drawImage(0, 0, ofGetWidth(), ofGetHeight());
    if(drawHands){
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(ofColor::red);
        ofDrawEllipse(handsInfo.rightHand.pos.x * ofGetWidth(), handsInfo.rightHand.pos.y * ofGetHeight(), 20+20*(handsInfo.rightHand.v.x), 20+20*(handsInfo.rightHand.v.y));
        ofDrawEllipse(handsInfo.leftHand.pos.x * ofGetWidth(), handsInfo.leftHand.pos.y * ofGetHeight(), 20+20*(handsInfo.leftHand.v.x),20+20*(handsInfo.leftHand.v.y));
        ofPopStyle();
    }
}

///--------------------------------------------------------------
void PMMotionExtractor::exit()
{
    openNIDevice.stop();
}

KinectInfo PMMotionExtractor::getHandsInfo()
{
    KinectInfo tempInfo = handsInfo;
    
    //Compute hand Mean value;
    ofPoint lHandPosMean = ofPoint(0);
    for (auto & tempPos : lHandPosHist) {
        lHandPosMean += tempPos;
    }
    lHandPosMean /= lHandPosHist.size();
    
    ofPoint rHandPosMean = ofPoint(0);
    for (auto & tempPos : rHandPosHist) {
        rHandPosMean += tempPos;
    }
    rHandPosMean /= rHandPosHist.size();
    
    //Assign mean value to output value;
    tempInfo.leftHand.pos = lHandPosMean;
    tempInfo.rightHand.pos = rHandPosMean;
    
    return tempInfo;
}

void PMMotionExtractor::computeVelocity(int meanSize)
{
    while(rHandPosHist.size() > meanSize)
        rHandPosHist.pop_back();
    while(lHandPosHist.size() > meanSize)
        lHandPosHist.pop_back();
    
    rHandPosHist.push_front(handsInfo.rightHand.pos);
    lHandPosHist.push_front(handsInfo.leftHand.pos);
    
    ofPoint rHandPosMean = ofPoint(0);
    for(auto & tempPos : rHandPosHist){
        rHandPosMean += tempPos;
    }
    rHandPosMean/=rHandPosHist.size();
    
    ofPoint lHandPosMean = ofPoint(0);
    for(auto & tempPos : lHandPosHist){
        lHandPosMean += tempPos;
    }
    lHandPosMean/=lHandPosHist.size();
    
    ofPoint rHandVel = ofPoint(0);
    for(auto & tempPos : rHandPosHist){
        rHandVel = (tempPos-rHandPosMean)*(tempPos-rHandPosMean);
    }
    rHandVel/=rHandPosHist.size();
    rHandVel*=1000;
    handsInfo.rightHand.v=rHandVel;
    
    ofPoint lHandVel = ofPoint(0);
    for(auto & tempPos : lHandPosHist){
        lHandVel = (tempPos-lHandPosMean)*(tempPos-lHandPosMean);
    }
    lHandVel/=lHandPosHist.size();
    lHandVel*=1000;
    handsInfo.leftHand.v=lHandVel;
    
}

bool PMMotionExtractor::reset(bool kinectActivated){
    if(!kinectActivated)
        return setup();
    else{
        hasUser=false;
        ofNotifyEvent(eventUserDetection, hasUser, this);
        openNIDevice.setPaused(true);
        openNIDevice.removeHandsGenerator();
        openNIDevice.removeAllHandFocusGestures();
        openNIDevice.addHandsGenerator();
        openNIDevice.addAllHandFocusGestures();
        openNIDevice.setMaxNumHands(2);
        
        for (int i = 0; i < openNIDevice.getMaxNumHands(); i++) {
            ofxOpenNIDepthThreshold depthThreshold = ofxOpenNIDepthThreshold(0, 0, false, true, true, true, true);
            openNIDevice.addDepthThreshold(depthThreshold);
        }
        openNIDevice.setPaused(false);
        return true;
    }
}
