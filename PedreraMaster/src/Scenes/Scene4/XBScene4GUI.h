//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE4GUI_H
#define PEDRERAMASTER_XBSCENE4GUI_H

#include "XBBaseGUI.h"

class XBScene4GUI : public XBBaseGUI
{
public:

    XBScene4GUI();
    virtual void setup();

    ofxGuiGroup violinGroup;
    ofxIntSlider minViolinSpeed;
    ofxIntSlider maxViolinSpeed;
    ofxFloatSlider violinK;
    ofxFloatSlider violinF;
    
    ofxGuiGroup pianoGroup;
    ofxFloatSlider stoneGrowFactor;
    ofxFloatSlider stoneDamping;
    ofxFloatSlider stoneAlphaDecrease;
    ofxFloatSlider stoneTime;
    ofxFloatSlider stoneFrequency;
    
    ofxGuiGroup directorGroup;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    
    ofxGuiGroup windowGroup;
    ofxFloatSlider barHeight;
    ofxFloatSlider alphaFactor;
    ofxFloatSlider growFactor;
    ofxIntSlider windowFrequency;
    
    ofxIntSlider blurAmount;
    ofxToggle enableSmooth;
    
};


#endif //PEDRERAMASTER_XBSCENE4GUI_H
