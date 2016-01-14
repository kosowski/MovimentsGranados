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
    virtual void draw();

    ofxPanel gui;
    ofxPanel colorGui;

    ofxLabel lblDirector;
    ofxIntSlider colorDirectorH;
    ofxIntSlider colorDirectorS;
    ofxIntSlider colorDirectorB;
    ofxIntSlider colorDirectorA;

    ofxLabel lblCello;
    ofxIntSlider colorCelloH;
    ofxIntSlider colorCelloS;
    ofxIntSlider colorCelloB;
    ofxIntSlider colorCelloA;

    ofxLabel lblViolin;
    ofxIntSlider colorViolinH;
    ofxIntSlider colorViolinS;
    ofxIntSlider colorViolinB;
    ofxIntSlider colorViolinA;

    ofxLabel lblPiano;
    ofxIntSlider colorPianoH;
    ofxIntSlider colorPianoS;
    ofxIntSlider colorPianoB;
    ofxIntSlider colorPianoA;
};


#endif //PEDRERAMASTER_XBBASEGUI_H
