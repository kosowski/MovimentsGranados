//
// Created by nacho cossio on 11/01/16.
//

#ifndef PEDRERAMASTER_XBSCENE4_H
#define PEDRERAMASTER_XBSCENE4_H

#include "XBBaseScene.h"
#include "ofxGui.h"
#include "Wave.h"


#define NUM_WAVES 10

class XBScene4 : public XBBaseScene
{
public:

    XBScene4(const string &name) : XBBaseScene(name) {}

    virtual void setup();
    virtual void update();
    virtual void drawIntoFBO();
    void keyReleased(int key);
    
    void initSystem();
    
    vector<Wave> waves;
    ofColor directorColor;
    
    bool showGui;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxPanel gui;

};


#endif //PEDRERAMASTER_XBSCENE4_H
