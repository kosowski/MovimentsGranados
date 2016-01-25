//
// Created by nacho cossio on 11/01/16.
//

#ifndef PEDRERAMASTER_XBSCENE4_H
#define PEDRERAMASTER_XBSCENE4_H

#include "XBBaseScene.h"
#include "ofxGui.h"
#include "Wave.h"
#include "BlurEffect.h"
#include "ofxMaskedGrayScott.h"
#include "ofxSvg.h"

class XBScene4 : public XBBaseScene
{
public:

    XBScene4(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyReleased(int key);
    
    void initReactionDiffusion();
    void initWaves();
    void initSVG();
    
    ofxSVG svg;
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    
    vector<Wave> waves;
    
    ofxMaskedGrayScott gray;
    ofShader convertToBW;

    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE4_H
