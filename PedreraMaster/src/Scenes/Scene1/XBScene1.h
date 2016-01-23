//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE1_H
#define PEDRERAMASTER_XBSCENE1_H

#include "XBBaseScene.h"
#include "ofxSvg.h"
#include "BlurEffect.h"
#include "ofxParticles.h"
#include "Wave.h"


class XBScene1 : public XBBaseScene
{
public:
    
    XBScene1(const string &name) : XBBaseScene(name) {}
    
    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyPressed(int key);
    void keyReleased(int key);
    
    void initWaves();
    void initLines();
    void initParticles();
    void initStones();
    void initWindows();
    void updateEmitters();
    
protected:
    int findIntersectionVertical(ofPolyline &line, int posY);
    int findIntersectionHorizontal(ofPolyline &line, int posX);
        
    ofxSVG svg;
    vector<ofPolyline> verticalLines;
    vector<ofPolyline> horizontalLines;
    int violinTimeIndex = 0;
    int celloTimeIndex = 0;

    bool fakeEvent = false;
    bool fakeCelloEvent = false;
    
    vector<ofPolyline> windowsOutlines;
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
    
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    
    vector<Wave> waves;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE1_H
