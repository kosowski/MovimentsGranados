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

    ofxGuiGroup creaturesGroup;
    ofxFloatSlider size;
    ofxFloatSlider maxSpeed;
    ofxFloatSlider maxForce;
    ofxFloatSlider pathSpeed;
    ofxFloatSlider stiffness;
    ofxFloatSlider damping;
    ofxFloatSlider mass;
    ofxFloatSlider glowAmount;
    ofxFloatSlider glowRadius;
    ofxToggle linkAudio;
    ofxToggle showPath;
    
    ofxGuiGroup particlesGroup;
    ofxFloatSlider particleSize;
    ofxFloatSlider particleLife;
    ofxFloatSlider maxParticles;
    ofxFloatSlider maxDistance;
    ofxToggle collisionOn;
    ofxVec3Slider particleVelocity;
    ofxVec3Slider particleSpread;
    ofxFloatSlider blurAmount;
    
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
    
//    ofxToggle showTemplate;
};


#endif //PEDRERAMASTER_XBSCENE3GUI_H
