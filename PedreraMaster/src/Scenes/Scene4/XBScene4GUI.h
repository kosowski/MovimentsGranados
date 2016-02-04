//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE4GUI_H
#define PEDRERAMASTER_XBSCENE4GUI_H

#include "XBBaseGUI.h"
#include "PresetsRD.h"

class XBScene4GUI : public XBBaseGUI
{
public:

    XBScene4GUI();
    virtual void setup();
    void onPresetChange(int &index);

    ofxGuiGroup violinGroup;
    ofxIntSlider minViolinSpeed;
    ofxIntSlider maxViolinSpeed;
    ofxFloatSlider violinK;
    ofxFloatSlider violinF;
    ofxIntSlider presetIndex;
    
    ofxGuiGroup pianoGroup;
    ofxFloatSlider stoneGrowFactor;
    ofxFloatSlider stoneDamping;
    ofxFloatSlider stoneAlphaDecrease;
    ofxFloatSlider stoneTime;
    ofxFloatSlider stoneFrequency;
    ofxIntSlider stonePhase;
    
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
    
    PresetsRD  presets;
};


#endif //PEDRERAMASTER_XBSCENE4GUI_H
