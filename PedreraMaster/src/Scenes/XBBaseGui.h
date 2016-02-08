//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBBASEGUI_H
#define PEDRERAMASTER_XBBASEGUI_H

#include "ofxGui.h"

static const string STR_SETTINGS_BASEPATH = "settings/";

class XBBaseGUI
{
public:

    XBBaseGUI();
    virtual void setup();
    virtual void draw();
    void drawSettingsSaved();

    ofxPanel sceneGui;
    ofxPanel colorGui;

    int rgbColorDirectorR;
    int rgbColorDirectorG;
    int rgbColorDirectorB;
    ofxIntSlider colorDirectorA;

    int rgbColorCelloR;
    int rgbColorCelloG;
    int rgbColorCelloB;
    ofxIntSlider colorCelloA;

    int rgbColorViolinR;
    int rgbColorViolinG;
    int rgbColorViolinB;
    ofxIntSlider colorViolinA;

    int rgbColorPianoR;
    int rgbColorPianoG;
    int rgbColorPianoB;
    ofxIntSlider colorPianoA;

    ofxToggle simulateHands;
    ofxToggle useGlow;
    ofxIntSlider glowAmount;
    ofxIntSlider glowradius;
    
    ofxIntSlider maxAlphaWindow;
    ofxIntSlider minAlphaWindow;
    ofxFloatSlider pianoSmoothFactor;
    ofxFloatSlider pianoDecay;
    
    void loadSettings();
    void saveSettings();

protected:

    string sceneSettingsFilename;
    string colorSettingsFilename;

    ofxLabel lblDirector;
    ofxIntSlider colorDirectorH;
    ofxIntSlider colorDirectorS;
    ofxIntSlider colorDirectorB;
    void colorDirectorHChanged(int &newH);
    void colorDirectorSChanged(int &newS);
    void colorDirectorBChanged(int &newB);

    ofxLabel lblCello;
    ofxIntSlider colorCelloH;
    ofxIntSlider colorCelloS;
    ofxIntSlider colorCelloB;
    void colorCelloHChanged(int &newH);
    void colorCelloSChanged(int &newS);
    void colorCelloBChanged(int &newB);

    ofxLabel lblViolin;
    ofxIntSlider colorViolinH;
    ofxIntSlider colorViolinS;
    ofxIntSlider colorViolinB;
    void colorViolinHChanged(int &newH);
    void colorViolinSChanged(int &newS);
    void colorViolinBChanged(int &newB);

    ofxLabel lblPiano;
    ofxIntSlider colorPianoH;
    ofxIntSlider colorPianoS;
    ofxIntSlider colorPianoB;
    void colorPianoHChanged(int &newH);
    void colorPianoSChanged(int &newS);
    void colorPianoBChanged(int &newB);

    bool showSavedMessage;
    float showSavedStartTime;
    string savePressedMessage;
    void saveScenePressed();
    void saveColorsPressed();

private:

    ofColor HSBtoRGB(float h, float s, float b);
};


#endif //PEDRERAMASTER_XBBASEGUI_H
