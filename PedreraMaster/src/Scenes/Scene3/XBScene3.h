//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE3_H
#define PEDRERAMASTER_XBSCENE3_H
#define NUM_WAVES 8

#include "XBBaseScene.h"
#include "Vehicle.h"
#include "ofxParticles.h"
#include "BlurEffect.h"
#include "ofxSvg.h"
#include "Wave.h"

struct expandingPolyLine{
    ofPolyline line;
    ofPath path;
    float life;
    ofPoint centroid;
};

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

    Vehicle v, x;
    
    bool emitParticles;
    ofxParticleSystem particleSystem;
    int pmouseX, pmouseY;
    ofVec2f pmouseVel;
    
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    int displayMode;
    
    ofxSVG svg;
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    
    
    void initWaves();
    
    vector<Wave> waves;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE3_H
