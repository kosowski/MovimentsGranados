//
//  PMMotionExtractor.hpp
//  PMCancons_Visuals
//
//  Created by Eduard Frigola on 16/10/15.
//
//

#ifndef PMMotionExtractor_hpp
#define PMMotionExtractor_hpp

#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "ofxKinectForWindows2.h"

struct KinectElement
{
    ofPoint pos;
    ofPoint v;
    float a;
};
struct KinectInfo
{
    KinectElement leftHand;
    KinectElement rightHand;
//    KinectElement head_joint;
//    KinectElement torso_joint;
};

class PMMotionExtractor
{
public:
    /**
     * getInstance()
     * Returns singleton instance
     */
    static PMMotionExtractor &getInstance()
    {
        static PMMotionExtractor instance;
        return instance;
    }

    PMMotionExtractor() {};
    ~PMMotionExtractor() {};

    bool setup();
    void update();
    void draw(bool drawImage, bool drawHands);
    void exit();
    
    bool reset(bool kinectActivated);
    
    void computeVelocity(int meanSize);
	ofPixels getColorPixels();

	KinectInfo getHandsInfo();

	ofxKFW2::Data::Body findClosestBody();

	void setPositionedThreshold(float threshold) { positionThreshold = threshold; };
	void setMeanSize(int meanSize) { handsMeanSize = meanSize; };
    
    ofEvent<bool> eventUserDetection;
	ofEvent<bool> eventUserPositioned;

private:
	ofxKFW2::Device kinect;
    KinectInfo kinectOut;
    KinectInfo handsInfo;
    bool hasUser, hasKinect;
    
    //for computing velocity
    deque<ofPoint> rHandPosHist;
    deque<ofPoint> lHandPosHist;
	int handsMeanSize;

	//positioning variable
	int positionDetectedCounter;
	float positionThreshold;
};

#endif /* PMMotionExtractor_hpp */