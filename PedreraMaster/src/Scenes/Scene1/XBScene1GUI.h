//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE1GUI_H
#define PEDRERAMASTER_XBSCENE1GUI_H

#include "XBBaseGUI.h"

class XBScene1;

class XBScene1GUI : public XBBaseGUI
{
public:

    XBScene1GUI(XBScene1 *scene);
    void draw();

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
};


#endif //PEDRERAMASTER_XBSCENE1GUI_H
