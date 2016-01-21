//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE3_H
#define PEDRERAMASTER_XBSCENE3_H
#define NUM_WAVES 1

#include "XBBaseScene.h"
#include "Vehicle.h"
#include "ofxParticles.h"
#include "BlurEffect.h"
#include "ofxSvg.h"
#include "Wave.h"


class XBScene3 : public XBBaseScene
{
public:

    XBScene3(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    virtual void drawGUI();
    void keyReleased(int key);
    void initSVG();
    void initParticles();
    void initPaths();
    void initWaves();

    Vehicle v, x;
    ofPolyline vPath;
    float vPathIndex = 0;
    ofPolyline xPath;
    float xPathIndex = 0;
    
    bool emitParticles;
    ofxParticleSystem particleSystem;
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    
    ofxSVG svg;
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
 
    vector<Wave> waves;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE3_H