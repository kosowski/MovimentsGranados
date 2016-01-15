//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE3GUI_H
#define PEDRERAMASTER_XBSCENE3GUI_H

#include "XBBaseGUI.h"

class XBScene3GUI : public XBBaseGUI
{
public:

    XBScene3GUI();
    virtual void setup();

    ofxGuiGroup particlesGroup;
    ofxFloatSlider particleSize;
    ofxFloatSlider particleLife;
    ofxVec3Slider particleVelocity;
    ofxVec3Slider particleSpread;
    ofxFloatSlider blurAmount;
    
    ofxGuiGroup pianoGroup;
    ofxFloatSlider stoneGrowFactor;
    ofxFloatSlider stoneAlphaDecrease;
    ofxFloatSlider stoneTime;
    
    ofxGuiGroup directorGroup;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    
};


#endif //PEDRERAMASTER_XBSCENE3GUI_H
