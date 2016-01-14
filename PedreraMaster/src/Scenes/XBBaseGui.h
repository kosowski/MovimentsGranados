//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBBASEGUI_H
#define PEDRERAMASTER_XBBASEGUI_H

#include "ofxGui.h"
#include "ofxColorPicker.h"

static const string STR_SETTINGS_BASEPATH = "settings/";

class XBBaseGUI
{
public:

    XBBaseGUI();
    virtual void setup();
    virtual void draw();

    ofxPanel sceneGui;
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

    void loadSettings();
    void saveSettings();

protected:

    string sceneSettingsFilename;
    string colorSettingsFilename;
};


#endif //PEDRERAMASTER_XBBASEGUI_H
