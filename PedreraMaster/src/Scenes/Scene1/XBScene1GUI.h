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
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    ofxFloatSlider maskRadius;
    ofxFloatSlider maskAlpha;
    ofxFloatSlider dampingWaves;
    
    ofxGuiGroup violinGroup;
    ofxToggle celloVertical;
    ofxFloatSlider windowAttack;
    ofxFloatSlider windowFade;
    ofxToggle drawWindows;
    ofxFloatSlider timeIncrement;
    ofxToggle showTimeMarker;
    
    ofxFloatSlider particleSize;
    ofxFloatSlider particleLife;
    ofxIntSlider numParticles;
    ofxFloatSlider headSize;

    ofxVec2Slider particleVelocity;
    ofxVec2Slider particleSpread;
    
    ofxGuiGroup pianoGroup;
    ofxFloatSlider stoneGrowFactor;
    ofxFloatSlider stoneDamping;
    ofxFloatSlider stoneAlphaDecrease;
    ofxFloatSlider stoneTime;
    ofxFloatSlider stoneFrequency;

    ofxFloatSlider blurAmount;
};


#endif //PEDRERAMASTER_XBSCENE1GUI_H
