//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBBaseGUI.h"

static const string strSceneGuiTitle = "SCENE SETTINGS";
static const string strColorGuiTitle = "COLOR SETTINGS";

static const string strDirector     = "DIRECTOR";
static const string strCello        = "CELLO";
static const string strViolin       = "VIOLIN";
static const string strPiano        = "PIANO";

static const string strHue          = "Hue";
static const string strSat          = "Saturation";
static const string strBright       = "Brightness";
static const string strAlpha        = "Alpha";

static const float scenePosX        = 10;
static const float scenePosY        = 10;
static const float colorsPosX       = 240;
static const float colorsPosY       = scenePosY;

static const int defColorValue      = 127;
static const int defAlpha           = 255;
static const int minColor           = 0;
static const int maxColor           = 255;

XBBaseGUI::XBBaseGUI()
{
}

void XBBaseGUI::setup()
{
    sceneGui.setup(strSceneGuiTitle, STR_SETTINGS_BASEPATH + sceneSettingsFilename);
    sceneGui.setPosition(scenePosX, scenePosY);

    colorGui.setup(strColorGuiTitle, STR_SETTINGS_BASEPATH + colorSettingsFilename);
    colorGui.setPosition(colorsPosX, colorsPosY);

    colorGui.add(lblDirector.setup(strDirector, ""));
    colorGui.add(colorDirectorH.setup("D " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorDirectorS.setup("D " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorDirectorB.setup("D " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorDirectorA.setup("D " + strAlpha, defAlpha, minColor, maxColor));
    colorDirectorH.addListener(this, &XBBaseGUI::colorDirectorHChanged);
    colorDirectorS.addListener(this, &XBBaseGUI::colorDirectorSChanged);
    colorDirectorB.addListener(this, &XBBaseGUI::colorDirectorBChanged);

    colorGui.add(lblCello.setup(strCello, ""));
    colorGui.add(colorCelloH.setup("C " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloS.setup("C " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloB.setup("C " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloA.setup("C " + strAlpha, defAlpha, minColor, maxColor));
    colorCelloH.addListener(this, &XBBaseGUI::colorCelloHChanged);
    colorCelloS.addListener(this, &XBBaseGUI::colorCelloSChanged);
    colorCelloB.addListener(this, &XBBaseGUI::colorCelloBChanged);

    colorGui.add(lblViolin.setup(strViolin, ""));
    colorGui.add(colorViolinH.setup("V " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinS.setup("V " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinB.setup("V " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinA.setup("V " + strAlpha, defAlpha, minColor, maxColor));
    colorViolinH.addListener(this, &XBBaseGUI::colorViolinHChanged);
    colorViolinS.addListener(this, &XBBaseGUI::colorViolinSChanged);
    colorViolinB.addListener(this, &XBBaseGUI::colorViolinBChanged);

    colorGui.add(lblPiano.setup(strPiano, ""));
    colorGui.add(colorPianoH.setup("P " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoS.setup("P " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoB.setup("P " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoA.setup("P " + strAlpha, defAlpha, minColor, maxColor));
    colorPianoH.addListener(this, &XBBaseGUI::colorPianoHChanged);
    colorPianoS.addListener(this, &XBBaseGUI::colorPianoSChanged);
    colorPianoB.addListener(this, &XBBaseGUI::colorPianoBChanged);

    lblDirector.setBackgroundColor(ofColor::darkGoldenRod);
    lblCello.setBackgroundColor(ofColor(40, 100, 40));
    lblViolin.setBackgroundColor(ofColor(50, 110, 50));
    lblPiano.setBackgroundColor(ofColor(60, 120, 60));
}

void XBBaseGUI::draw()
{
    sceneGui.draw();
    colorGui.draw();
}

void XBBaseGUI::loadSettings()
{
    sceneGui.loadFromFile(STR_SETTINGS_BASEPATH + sceneSettingsFilename);
    colorGui.loadFromFile(STR_SETTINGS_BASEPATH + colorSettingsFilename);
}

void XBBaseGUI::saveSettings()
{
    sceneGui.saveToFile(STR_SETTINGS_BASEPATH + sceneSettingsFilename);
    colorGui.saveToFile(STR_SETTINGS_BASEPATH + colorSettingsFilename);
}

void XBBaseGUI::colorDirectorHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorDirectorS, colorDirectorB);
    rgbColorDirectorR = int(c.r);
}

void XBBaseGUI::colorDirectorSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorDirectorH, newS, colorDirectorB);
    rgbColorDirectorG = int(c.g);
}

void XBBaseGUI::colorDirectorBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorDirectorH, colorDirectorS, newB);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorCelloHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorCelloS, colorCelloB);
    rgbColorCelloR = int(c.r);
}

void XBBaseGUI::colorCelloSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorCelloH, newS, colorCelloB);
    rgbColorCelloG = int(c.g);
}

void XBBaseGUI::colorCelloBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorCelloH, colorCelloS, newB);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorViolinHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorViolinS, colorViolinB);
    rgbColorViolinR = int(c.r);
}

void XBBaseGUI::colorViolinSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorViolinH, newS, colorViolinB);
    rgbColorViolinG = int(c.g);
}

void XBBaseGUI::colorViolinBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorViolinH, colorViolinS, newB);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorPianoHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorPianoS, colorPianoB);
    rgbColorPianoR = int(c.r);
}

void XBBaseGUI::colorPianoSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorPianoH, newS, colorPianoB);
    rgbColorPianoG = int(c.g);
}

void XBBaseGUI::colorPianoBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorPianoH, colorPianoS, newB);
    rgbColorPianoB = int(c.b);
}
