//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE2GUI_H
#define PEDRERAMASTER_XBSCENE2GUI_H

#include "XBBaseGUI.h"

class XBScene2;

class XBScene2GUI : public XBBaseGUI
{
    XBScene2GUI(XBScene2 *scene);
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
};


#endif //PEDRERAMASTER_XBSCENE2GUI_H
