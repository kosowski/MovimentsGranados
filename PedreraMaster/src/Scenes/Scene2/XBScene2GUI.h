//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE2GUI_H
#define PEDRERAMASTER_XBSCENE2GUI_H

#include "XBBaseGUI.h"

class XBScene2GUI : public XBBaseGUI
{
public:

    XBScene2GUI();
    virtual void setup();

    ofxGuiGroup directorGroup;
    ofxFloatSlider minAttractorStrength;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    ofxFloatSlider dampingWaves;
    
    ofxGuiGroup pianoGroup;
    ofxFloatSlider stoneGrowFactor;
    ofxFloatSlider stoneDamping;
    ofxFloatSlider stoneAlphaDecrease;
    ofxFloatSlider stoneTime;
    ofxFloatSlider stoneFrequency;
    
    ofxGuiGroup windowGroup;
    ofxFloatSlider barHeight;
    ofxFloatSlider alphaStart;
    ofxFloatSlider alphaFactor;
    ofxFloatSlider growFactor;
    ofxIntSlider windowFrequency;
    
    ofxFloatSlider blurAmount;
};


#endif //PEDRERAMASTER_XBSCENE2GUI_H
