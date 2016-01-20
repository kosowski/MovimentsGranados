//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE1_H
#define PEDRERAMASTER_XBSCENE1_H

#include "XBBaseScene.h"
#include "ofxTraerPhysics.h"
#include "ofxSvg.h"
#include "BlurEffect.h"
#include "ofxParticles.h"

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
    void initLines();
    void initParticles();
    void initStones();
    void updateEmitters();
    
protected:
    ofxSVG svg;
    vector<ofPolyline> verticalLines;
    vector<ofPolyline> horizontalLines;
    int violinTimeIndex = 0;
    int celloTimeIndex = 0;

    bool fakeEvent = false;
    bool fakeCelloEvent = false;
    
    vector<ofPolyline> linesToDraw;
    unsigned int violinLineIndex = 0;
    ofPolyline currentViolinNote;
    ofPolyline violinLine;
    
    unsigned int celloLineIndex = 0;
    ofPolyline currentCelloNote;
    ofPolyline celloLine;

    
    bool emitParticles;
    ofxParticleSystem particleSystem;
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    
    ParticleSystem * 	physics;
    vector<   Particle * > particles;
    vector<   Spring * > visibleSprings;
    Particle*			selectedParticle;
    Particle* p_mouse;
    
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE1_H
