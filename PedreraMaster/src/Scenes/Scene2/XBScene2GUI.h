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
    virtual void setup();

    ofxGuiGroup directorGroup;
    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
};


#endif //PEDRERAMASTER_XBSCENE2GUI_H
