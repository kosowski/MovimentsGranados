//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBBASEGUI_H
#define PEDRERAMASTER_XBBASEGUI_H

#include "ofxGui.h"
#include "ofxColorPicker.h"

class XBBaseScene;

class XBBaseGUI
{
public:

    XBBaseGUI(XBBaseScene *scene);
    virtual void update();
    virtual void draw();

protected:

    ofxPanel gui;
    vector<ofxColorPicker> colorPickers;
    vector<ofColor> colors;

    XBBaseScene *scene;

private:

};


#endif //PEDRERAMASTER_XBBASEGUI_H
