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
    float x;
    float y;
    float z;
    float a;
    ofPoint v;
};
struct KinectInfo
{
    KinectElement leftHand_joint;
    KinectElement rightHand_joint;
    KinectElement head_joint;
    KinectElement torso_joint;
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

    void draw(); //noexistirà?
    void exit();

    KinectInfo *getKinectInfo();

    bool isReady() { return (isSomeoneTracked<30 && hadUsers); };

    bool isTracking()
    {
        return hadUsers;
    }
    
    void resetUsers();
    void stop(){
        hadUsers = false;
        kinectNI->setPaused(true);
        kinectNI->removeUserGenerator();
        isSomeoneTracked=0;
    };
    
    void hardStart(){kinectNI->start();};
    void start(){
        kinectNI->addUserGenerator();
//        kinectNI->setMaxNumUsers(1);
        kinectNI->setPaused(false);
        isSomeoneTracked=0;
    };
    
    int getNumUsers(){
        return kinectNI->getNumTrackedUsers();
    }

private:
    ofTrueTypeFont font;
    ofxKinectFeatures kinectFeatures;
    ofxOpenNI* kinectNI;
    bool hadUsers;
    KinectInfo kinectOut;

    int isSomeoneTracked;
};

#endif /* PMMotionExtractor_hpp */