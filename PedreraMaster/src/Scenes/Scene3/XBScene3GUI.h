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

    ofxFloatSlider particleSize;
    ofxFloatSlider particleLife;
    ofxVec3Slider particleVelocity;
    ofxVec3Slider particleSpread;
};


#endif //PEDRERAMASTER_XBSCENE3GUI_H
