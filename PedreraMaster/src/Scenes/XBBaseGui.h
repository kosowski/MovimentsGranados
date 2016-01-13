//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBBASEGUI_H
#define PEDRERAMASTER_XBBASEGUI_H

#include "ofxGui.h"
#include "ofxColorPicker.h"


class XBBaseGUI
{
public:

    XBBaseGUI();
    virtual void update();
    virtual void draw();

protected:

    ofxPanel gui;
    vector<ofxColorPicker> colorPickers;
    vector<ofColor> colors;
};


#endif //PEDRERAMASTER_XBBASEGUI_H
