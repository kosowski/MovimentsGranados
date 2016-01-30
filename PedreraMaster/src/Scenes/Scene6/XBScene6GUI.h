//
// Created by Miquel Àngel Soler on 27/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE6GUI_H
#define PEDRERAMASTER_XBSCENE6GUI_H

#include "XBBaseGUI.h"

class XBScene6GUI : public XBBaseGUI
{
public:

    XBScene6GUI();
    virtual void setup();

    ofxGuiGroup messagesGroup;
    ofxFloatSlider mainMsgX;
    ofxFloatSlider mainMsgY;
    ofxFloatSlider mainMsgScale;
    ofxFloatSlider countdownMsgX;
    ofxFloatSlider countdownMsgY;
    ofxFloatSlider countdownMsgScale;
};


#endif //PEDRERAMASTER_XBSCENE6GUI_H
