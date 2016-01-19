//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE1GUI_H
#define PEDRERAMASTER_XBSCENE1GUI_H

#include "XBBaseGUI.h"

class XBScene1GUI : public XBBaseGUI
{
public:

    XBScene1GUI();
    virtual void setup();

    ofxGuiGroup directorGroup;
    ofxFloatSlider springStrength;
    ofxFloatSlider springDamping;
    ofxFloatSlider drag;
    ofxFloatSlider gravity;
    ofxFloatSlider particleMass;
    ofxFloatSlider mouseSlope;
    ofxFloatSlider mouseStrength;
    ofxFloatSlider fixedStrength;
    ofxFloatSlider fixedDamping;
    ofxFloatSlider fixedRestLength;
    ofxFloatSlider restLength;
    ofxFloatSlider xDamping;
    
    ofxGuiGroup violinGroup;
    ofxFloatSlider violinBorderSize;
    ofxFloatSlider ViolinBorderFade;
    ofxToggle showTimeMarker;
    
    ofxGuiGroup particlesGroup;
    ofxFloatSlider particleSize;
    ofxFloatSlider particleLife;
    ofxVec3Slider particleVelocity;
    ofxVec3Slider particleSpread;

    ofxFloatSlider blurAmount;
};


#endif //PEDRERAMASTER_XBSCENE1GUI_H
