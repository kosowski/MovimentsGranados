//
// Created by Miquel Àngel Soler on 27/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE6GUI_H
#define PEDRERAMASTER_XBSCENE6GUI_H

#include "XBBaseGUI.h"

class XBScene6GUI : public XBBaseGUI
{
public:

    XBScene6GUI();
    virtual void setup();

    ofxGuiGroup kinectGroup;
    ofxToggle kinectMode;
    ofxGuiGroup s3Interaction;
    ofxIntSlider interactionMaxTime;
    ofxGuiGroup titlesGroup;
    ofxFloatSlider titleX;
    ofxFloatSlider titleY;
    ofxFloatSlider titleScale;
    ofxGuiGroup subtitlesGroup;
    ofxFloatSlider subtitleX;
    ofxFloatSlider subtitleY;
    ofxFloatSlider subtitleScale;
    ofxGuiGroup countdownGroup;
    ofxFloatSlider countdownX;
    ofxFloatSlider countdownY;
    ofxFloatSlider countdownScale;
    
    ofxGuiGroup directorGroup;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    ofxFloatSlider dampingWaves;
    ofxFloatSlider maskRadius;
    ofxFloatSlider maskAlpha;
    ofxFloatSlider blurAmount;
};


#endif //PEDRERAMASTER_XBSCENE6GUI_H
