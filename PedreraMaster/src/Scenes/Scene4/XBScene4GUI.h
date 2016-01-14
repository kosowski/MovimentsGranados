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

    ofxFloatSlider attractorStrength;
    ofxFloatSlider attractorRadius;
    ofxFloatSlider minPeriod;
    ofxFloatSlider maxPeriod;
    ofxFloatSlider lineWidth;
    ofxFloatSlider alpha;
    ofxToggle enableSmooth;
};


#endif //PEDRERAMASTER_XBSCENE4GUI_H
