//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE3_H
#define PEDRERAMASTER_XBSCENE3_H

#include "XBBaseScene.h"
#include "Vehicle.h"
#include "ofxParticles.h"


class XBScene3 : public XBBaseScene
{
public:

    XBScene3(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    virtual void drawGUI();
    void keyReleased(int key);

    Vehicle v, x;
    
    bool emitParticles;
    ofxParticleSystem particleSystem;
    int pmouseX, pmouseY;
    ofVec2f pmouseVel;
    
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    int displayMode;
};


#endif //PEDRERAMASTER_XBSCENE3_H
