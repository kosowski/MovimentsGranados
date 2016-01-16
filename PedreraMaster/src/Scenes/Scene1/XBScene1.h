//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE1_H
#define PEDRERAMASTER_XBSCENE1_H

#include "XBBaseScene.h"
#include "ofxTraerPhysics.h"
#include "ofxSvg.h"
#include "BlurEffect.h"

using namespace  traer::physics ;

class XBScene1 : public XBBaseScene
{
public:
    
    XBScene1(const string &name) : XBBaseScene(name) {}
    
    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyPressed(int key);
    void keyReleased(int key);
    
    void initSystem();
    void initSVG();
    
protected:
    ofxSVG svg;
    vector<ofPolyline> verticalLines;
    int indices[24] = {2,5,6,12,17,19,20,23,24,25,29,30,36,41,43,44,47,52,53,54,56,63,65,70};
    int timeIndex = 0;
    bool fakeEvent = false;
    vector<ofPolyline> linesToDraw;
    unsigned int violinLineIndex = 0;
    ofPolyline currentNote;
    ofPolyline violinLine;
    
    ParticleSystem * 	physics;
    vector<   Particle * > particles;
    vector<   Spring * > visibleSprings;
    Particle*			selectedParticle;
    Particle* p_mouse;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE1_H
