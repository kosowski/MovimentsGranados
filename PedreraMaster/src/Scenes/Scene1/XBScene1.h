//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE1_H
#define PEDRERAMASTER_XBSCENE1_H

#include "XBBaseScene.h"
#include "ofxTraerPhysics.h"

using namespace  traer::physics ;

class XBScene1 : public XBBaseScene
{
public:

    XBScene1(const string &name) : XBBaseScene(name) {}

    virtual void setup();
    virtual void update();
    virtual void drawIntoFBO();
    void keyReleased(int key);
    
    void initSystem();
    
    ParticleSystem * 	physics;
    vector<   Particle * > particles;
    vector<   Spring * > visibleSprings;
    Particle*			selectedParticle;
    Particle* p_mouse;
    
    ofColor directorColor;
    
    ofxFloatSlider springStrength;
    ofxFloatSlider springDamping;
    ofxFloatSlider drag;
    ofxFloatSlider gravity;
    ofxFloatSlider particleMass;
    ofxFloatSlider mouseSlope;
    ofxFloatSlider mouseStrength;
    ofxFloatSlider fixedStrength;
    ofxFloatSlider fixedDamping;
    ofxFloatSlider fixedRestLength;
    ofxFloatSlider restLength;
    ofxFloatSlider xDamping;
};


#endif //PEDRERAMASTER_XBSCENE1_H
