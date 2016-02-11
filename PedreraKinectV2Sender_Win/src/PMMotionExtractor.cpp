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
	positionDetectedCounter = 0;
	positionThreshold = 0.1;
	handsMeanSize = 5;

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	//TODO: Implement kinect detection
    //hasKinect = kinect.isOpen();
	hasKinect = true;
    return hasKinect;

	
}

///--------------------------------------------------------------
void PMMotionExtractor::update()
{
    if(hasKinect){
        kinect.update();
		// Count number of tracked bodies
		int numBodiesTracked = 0;
		auto& bodies = kinect.getBodySource()->getBodies();
		for (auto& body : bodies) {
			if (body.tracked) {
				numBodiesTracked++;
			}
		}
		if (numBodiesTracked != 0) {
			if (!hasUser) {
				hasUser = true;
				ofNotifyEvent(eventUserDetection, hasUser, this);
			}
			//--
			//Getting joint positions (skeleton tracking)
			//--
			//
			{
				auto body = findClosestBody();
				auto referenceBodyZ = body.joints[JointType_SpineBase].getPosition().z;


				/*for (auto body : bodies) {
					if (body.trackingId != closestBody.trackingId) {
						cout << body.trackingId << "  " << closestBody.trackingId << endl;
						break;
					}*/

					for (auto joint : body.joints) {
						if (joint.first == JointType_HandLeft) {
							handsInfo.leftHand.pos = joint.second.getProjected(kinect.getBodySource()->getCoordinateMapper(), ofxKFW2::ProjectionCoordinates::DepthCamera);
							handsInfo.leftHand.pos.x /= 512;
							handsInfo.leftHand.pos.y /= 424;
							handsInfo.leftHand.pos.z = referenceBodyZ - joint.second.getPosition().z;
						}
						else if (joint.first == JointType_HandRight) {
							handsInfo.rightHand.pos = joint.second.getProjected(kinect.getBodySource()->getCoordinateMapper(), ofxKFW2::ProjectionCoordinates::DepthCamera);
							handsInfo.rightHand.pos.x /= 512;
							handsInfo.rightHand.pos.y /= 424;
							handsInfo.rightHand.pos.z = referenceBodyZ - joint.second.getPosition().z;
						}
						else if (joint.first == JointType_Head) {
							auto headPos = joint.second.getProjected(kinect.getBodySource()->getCoordinateMapper(), ofxKFW2::ProjectionCoordinates::DepthCamera);
							headPos.y /= 424;
							if (abs(headPos.y - handsInfo.leftHand.pos.y) < 0.05 && abs(headPos.y - handsInfo.rightHand.pos.y) < positionThreshold) {
								positionDetectedCounter++;
							}
							else {
								positionDetectedCounter = 0;
							}

							if (positionDetectedCounter >= 60) {
								auto userPositioned = true;
								ofNotifyEvent(eventUserPositioned, userPositioned, this);
								positionDetectedCounter = 0;
							}
						}
					}
				//}
				computeVelocity(handsMeanSize);
			}
		}
		else {
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
        computeVelocity(handsMeanSize);
		//cout << ofGetMouseX() << endl;
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
	if (hasKinect && drawImage) {
		auto infraredImage = kinect.getInfraredSource();
		auto infraredPixels = infraredImage->getPixels();
		for (auto & pixel : infraredPixels) {
			pixel += pixel * 2;
		}
		ofTexture drawTexture;
		drawTexture.allocate(512, 424, GL_LUMINANCE);
		drawTexture.loadData(infraredPixels);
		drawTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
    if(drawHands){
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(ofColor::red);
        ofDrawEllipse(handsInfo.rightHand.pos.x * ofGetWidth(), handsInfo.rightHand.pos.y * ofGetHeight(), 20+20*(handsInfo.rightHand.v.x), 20+20*(handsInfo.rightHand.v.y));
        ofDrawEllipse(handsInfo.leftHand.pos.x * ofGetWidth(), handsInfo.leftHand.pos.y * ofGetHeight(), 20+20*(handsInfo.leftHand.v.x),20+20*(handsInfo.leftHand.v.y));
		//ofDrawBitmapString(handsInfo.rightHand.pos.z, handsInfo.rightHand.pos.x * ofGetWidth() + 10, handsInfo.rightHand.pos.y * ofGetHeight() + 10);
		//ofDrawBitmapString(handsInfo.leftHand.pos.z, handsInfo.leftHand.pos.x * ofGetWidth() + 10, handsInfo.leftHand.pos.y * ofGetHeight() + 10);
        ofPopStyle();
    }
	ofDrawBitmapString(positionDetectedCounter, 0, 0);
}

///--------------------------------------------------------------
void PMMotionExtractor::exit()
{
	kinect.close();
}

KinectInfo PMMotionExtractor::getHandsInfo() {
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

ofxKFW2::Data::Body PMMotionExtractor::findClosestBody()
{
	double closestBodyDistance = 10000000.0;

	auto& bodies = kinect.getBodySource()->getBodies();
	auto result = bodies[0];

	for (auto body : bodies)
	{
		if (body.tracked)
		{
			auto joints = body.joints;

			auto currentLocation = joints[JointType_SpineBase].getPosition();

			auto currentDistance = sqrt(pow(currentLocation.x, 2) + pow(currentLocation.y, 2) + pow(currentLocation.z, 2));

			if (currentDistance < closestBodyDistance)
			{
				result = body;
				closestBodyDistance = currentDistance;
			}
		}
	}

	return result;
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
	kinect.close();
	kinect.open();

	return true;
}


ofPixels PMMotionExtractor::getColorPixels() {
	auto ColoredImage = kinect.getColorSource();
	return ColoredImage->getPixels();
}