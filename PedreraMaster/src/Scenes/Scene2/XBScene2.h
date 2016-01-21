//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE2_H
#define PEDRERAMASTER_XBSCENE2_H

#include "XBBaseScene.h"
#include "ofxTraerPhysics.h"
#include "ofxGui.h"
#include "ofxSvg.h"

using namespace  traer::physics ;

class XBScene2 : public XBBaseScene
{
public:

    XBScene2(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyReleased(int key);
    void keyPressed(int key);
    
    void initSystem();
    void initWindows(string name,  vector<ofRectangle>& vectorWindows, int starIndex, int floor);
    void arrangeWindows(int index, vector<ofRectangle>& elements);
    
    void drawWindow(float note, vector<ofRectangle>& windows);
    
    ofxSVG svg;
    vector<ofRectangle> celloWindows;
    vector<ofRectangle> violinWindows;
    vector<ofRectangle> pianoWindows;
    vector<ofPolyline> celloLines;
    ofImage windowMask;
    
    ParticleSystem * 	physics;
    vector<   Particle * > particles;
    vector<   Spring * > visibleSprings;
    Particle*			selectedParticle;
    Particle* p_mouse;
    
    ofColor directorColor;
};


#endif //PEDRERAMASTER_XBSCENE2_H
