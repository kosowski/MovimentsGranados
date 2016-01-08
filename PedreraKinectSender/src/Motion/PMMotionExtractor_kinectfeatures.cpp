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
    kinectNI= new           ofxOpenNI;
    kinectNI->setup();
    kinectNI->addDepthGenerator();
    kinectNI->addImageGenerator();
    kinectNI->setRegister(false);
    kinectNI->setMirror(true);
    kinectNI->addUserGenerator();
//    kinectNI->setMaxNumUsers(1);
//    kinectNI->setSafeThreading(true);
    kinectNI->setSkeletonProfile(XN_SKEL_PROFILE_UPPER);
    kinectNI->start();
    kinectNI->setPaused(true);
//    kinectNI->removeUserGenerator();
//    kinectNI->stop();
//    kinectNI->start();
    isSomeoneTracked=30;
    return kinectNI->getNumDevices();
}

///--------------------------------------------------------------
void PMMotionExtractor::update()
{
    kinectNI->update();
    for (int i = 0; i < kinectNI->getNumTrackedUsers(); i++) {
        ofxOpenNIUser user = kinectNI->getTrackedUser(i);
        //The following "if" statement is a hard-coded alternative for if(kinectNI->getUserGenerator().IsNewDataAvailable()), which doesn't work properly in ofxOpenNI
        if (user.getJoint((Joint)0).getWorldPosition() != ofPoint(0,0,0) &&
            user.getJoint((Joint)0).getWorldPosition() != kinectFeatures.getPosition(0) ) {
            isSomeoneTracked=0;
            map<int, ofPoint> joints;
            for (int j = 0; j < user.getNumJoints(); j++) {
                joints[j] = user.getJoint((Joint)j).getWorldPosition();
            }
            kinectFeatures.update(joints);
        }else{isSomeoneTracked++;}
    }
    
    //This is a trick to reset the user generator if all users are lost
    if (kinectNI->getNumTrackedUsers()) {
        hadUsers = true;
    } else if (!kinectNI->getNumTrackedUsers() && hadUsers){
        hadUsers = false;
        kinectNI->setPaused(true);
        kinectNI->removeUserGenerator();
        kinectNI->addUserGenerator();
        kinectNI->setPaused(false);
//        resetUsers();
    }
    
    //wraper to kinectInfo struct
    int mean_values=5;
    int mean_values2=2; //for torso joint that acelerates low time
    
    if(false){
        //LEFT HAND
        cout<<"new Values!"<<endl;
        kinectOut.leftHand_joint.x=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_LEFT_HAND)).x/kinectNI->getWidth();
        kinectOut.leftHand_joint.y=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_LEFT_HAND)).y/kinectNI->getHeight();
        kinectOut.leftHand_joint.z=ofMap(kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_LEFT_HAND)).z, 1000, 3000, 0, 1);
        kinectOut.leftHand_joint.a=kinectFeatures.getAccelerationMagnitudeMean(JOINT_LEFT_HAND, mean_values);
//        kinectOut.leftHand_joint.v=(ofPoint(kinectOut.leftHand_joint.x, kinectOut.leftHand_joint.y)-ofPoint(oldKinectOut.leftHand_joint.x, oldKinectOut.leftHand_joint.y)).normalize();
        kinectOut.leftHand_joint.v=kinectFeatures.getVelocityMean(JOINT_LEFT_HAND, mean_values);
        kinectOut.leftHand_joint.v.y*=-1;
        
        //RIGHT HAND
        kinectOut.rightHand_joint.x=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_RIGHT_HAND)).x/kinectNI->getWidth();
        kinectOut.rightHand_joint.y=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_RIGHT_HAND)).y/kinectNI->getHeight();
        kinectOut.rightHand_joint.z=ofMap(kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_RIGHT_HAND)).z, 1000, 3000, 0, 1);
        kinectOut.rightHand_joint.a=kinectFeatures.getAccelerationMagnitudeMean(JOINT_RIGHT_HAND, mean_values);
//        kinectOut.rightHand_joint.v=(ofPoint(kinectOut.rightHand_joint.x, kinectOut.rightHand_joint.y)-ofPoint(oldKinectOut.rightHand_joint.x, oldKinectOut.rightHand_joint.y)).normalize();
        kinectOut.rightHand_joint.v=kinectFeatures.getVelocityMean(JOINT_RIGHT_HAND, mean_values);
        kinectOut.rightHand_joint.v.y*=-1;
        
        //HEAD
        kinectOut.head_joint.x=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_HEAD)).x/kinectNI->getWidth();
        kinectOut.head_joint.y=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_HEAD)).y/kinectNI->getHeight();
        kinectOut.head_joint.z=ofMap(kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_HEAD)).z, 1000, 3000, 0, 1);
//        kinectOut.head_joint.a=kinectFeatures.getAccelerationMagnitudeMean(JOINT_HEAD, mean_values2);
        kinectOut.head_joint.a=kinectFeatures.getAccelerationMagnitude(JOINT_HEAD);
//        kinectOut.head_joint.v=(ofPoint(oldKinectOut.head_joint.x, oldKinectOut.head_joint.y)-ofPoint(oldKinectOut.head_joint.x, oldKinectOut.head_joint.y)).normalize();
        kinectOut.head_joint.v=kinectFeatures.getVelocityMean(JOINT_HEAD, mean_values);
        kinectOut.head_joint.v.y*=-1;
        
        //TORSO
        kinectOut.torso_joint.x=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_TORSO)).x/kinectNI->getWidth();
        kinectOut.torso_joint.y=kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_TORSO)).y/kinectNI->getHeight();
        kinectOut.torso_joint.z=ofMap(kinectNI->worldToProjective(kinectFeatures.getPosition(JOINT_TORSO)).z, 1000, 3000, 0, 1);
        kinectOut.torso_joint.a=kinectFeatures.getAccelerationMagnitudeMean(JOINT_TORSO, mean_values2);
//        kinectOut.torso_joint.v=(ofPoint(kinectOut.torso_joint.x, kinectOut.torso_joint.y)-ofPoint(oldKinectOut.torso_joint.x, oldKinectOut.torso_joint.y)).normalize();
        kinectOut.torso_joint.v=kinectFeatures.getVelocityMean(JOINT_TORSO, mean_values);
        kinectOut.torso_joint.v.y*=-1;
        
//        oldKinectOut=kinectOut;
    }
}

///--------------------------------------------------------------
void PMMotionExtractor::draw()
{
    ofSetRectMode(OF_RECTMODE_CENTER);
        kinectNI->drawImage(ofGetWidth()/2,ofGetHeight()/2, ofGetWidth(), ofGetHeight()*(kinectNI->getHeight()/kinectNI->getWidth()));
    //kinectNI->drawImage();
    ofSetRectMode(OF_RECTMODE_CORNER);
}

///--------------------------------------------------------------
void PMMotionExtractor::exit()
{
    kinectNI->removeUserGenerator();
    kinectNI->removeDepthGenerator();
    kinectNI->removeImageGenerator();
    kinectNI->stop();
    if (kinectNI){
        kinectNI->waitForThread(true);
//        kinectNI->unlock();
        delete kinectNI;
    }
}

KinectInfo* PMMotionExtractor::getKinectInfo()
{
    return &kinectOut;
}

void PMMotionExtractor::resetUsers()
{
    hadUsers = false;
    kinectNI->setPaused(true);
    kinectNI->removeUserGenerator();
    kinectNI->addUserGenerator();
    kinectNI->setPaused(false);
    isSomeoneTracked=0;
}


