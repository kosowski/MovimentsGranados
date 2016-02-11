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
    ofxIntSlider minCelloSpeed;
    ofxIntSlider maxCelloSpeed;
    ofxFloatSlider violinK;
    ofxFloatSlider violinF;
    ofxToggle linkAudio;
    ofxIntSlider presetIndex;
    
    ofxGuiGroup pianoGroup;
    ofxFloatSlider stoneGrowFactor;
    ofxFloatSlider stoneDamping;
    ofxFloatSlider stoneAlphaDecrease;
    ofxFloatSlider stoneTime;
    ofxFloatSlider stoneFrequency;
    ofxIntSlider stonePhase;
    
    ofxGuiGroup directorGroup;
    ofxFloatSlider minAttractorStrength;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    ofxFloatSlider dampingWaves;
    
    ofxGuiGroup windowGroup;
    ofxFloatSlider barHeight;
    ofxFloatSlider alphaStart;
    ofxFloatSlider alphaFactor;
    ofxFloatSlider growFactor;
    ofxIntSlider windowFrequency;
    
    ofxIntSlider blurAmount;
    ofxToggle enableSmooth;
    
    PresetsRD  presets;
};


#endif //PEDRERAMASTER_XBSCENE4GUI_H
