//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE4GUI_H
#define PEDRERAMASTER_XBSCENE4GUI_H

#include "XBBaseGUI.h"

class XBScene4GUI : public XBBaseGUI
{
public:

    XBScene4GUI();
    void draw();

    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
};


#endif //PEDRERAMASTER_XBSCENE4GUI_H
