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
#include "ofxKinectFeatures.h"
#include "ofxOpenNI.h"

#define MAX_DEVICES 1

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

    KinectInfo getKinectInfo();
    KinectInfo gethandsInfo(){return handsInfo;};
    
    ofEvent<bool> eventUserDetection;

private:
    ofxOpenNI openNIDevice;
    KinectInfo kinectOut;
    KinectInfo handsInfo;
    bool hasUser, hasKinect;
    
    //for computing velocity
    deque<ofPoint> rHandPosHist;
    deque<ofPoint> lHandPosHist;
};

#endif /* PMMotionExtractor_hpp */