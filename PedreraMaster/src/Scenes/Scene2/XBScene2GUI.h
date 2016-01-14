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
