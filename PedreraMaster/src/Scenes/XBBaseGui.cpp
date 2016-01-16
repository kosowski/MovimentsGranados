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
static const float colorsPosX       = 220;
static const float colorsPosY       = scenePosY;
static const float colorGuiWidth    = 500;

static const ofVec4f defColorDirector(195, 45, 55, 255);
static const ofVec4f defColorCello(147, 22, 36, 255);
static const ofVec4f defColorViolin(46, 63, 95, 255);
static const ofVec4f defColorPiano(11, 71, 81, 255);
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
    colorGui.add(colorDirectorH.setup("D " + strHue, int(defColorDirector.x), minColor, maxColor));
    colorGui.add(colorDirectorS.setup("D " + strSat, int(defColorDirector.y), minColor, maxColor));
    colorGui.add(colorDirectorB.setup("D " + strBright, int(defColorDirector.z), minColor, maxColor));
    colorGui.add(colorDirectorA.setup("D " + strAlpha, int(defColorDirector.w), minColor, maxColor));
    colorDirectorH.addListener(this, &XBBaseGUI::colorDirectorHChanged);
    colorDirectorS.addListener(this, &XBBaseGUI::colorDirectorSChanged);
    colorDirectorB.addListener(this, &XBBaseGUI::colorDirectorBChanged);

    colorGui.add(lblCello.setup(strCello, ""));
    colorGui.add(colorCelloH.setup("C " + strHue, int(defColorCello.x), minColor, maxColor));
    colorGui.add(colorCelloS.setup("C " + strSat, int(defColorCello.y), minColor, maxColor));
    colorGui.add(colorCelloB.setup("C " + strBright, int(defColorCello.z), minColor, maxColor));
    colorGui.add(colorCelloA.setup("C " + strAlpha, int(defColorCello.w), minColor, maxColor));
    colorCelloH.addListener(this, &XBBaseGUI::colorCelloHChanged);
    colorCelloS.addListener(this, &XBBaseGUI::colorCelloSChanged);
    colorCelloB.addListener(this, &XBBaseGUI::colorCelloBChanged);

    colorGui.add(lblViolin.setup(strViolin, ""));
    colorGui.add(colorViolinH.setup("V " + strHue, int(defColorViolin.x), minColor, maxColor));
    colorGui.add(colorViolinS.setup("V " + strSat, int(defColorViolin.y), minColor, maxColor));
    colorGui.add(colorViolinB.setup("V " + strBright, int(defColorViolin.z), minColor, maxColor));
    colorGui.add(colorViolinA.setup("V " + strAlpha, int(defColorViolin.w), minColor, maxColor));
    colorViolinH.addListener(this, &XBBaseGUI::colorViolinHChanged);
    colorViolinS.addListener(this, &XBBaseGUI::colorViolinSChanged);
    colorViolinB.addListener(this, &XBBaseGUI::colorViolinBChanged);

    colorGui.add(lblPiano.setup(strPiano, ""));
    colorGui.add(colorPianoH.setup("P " + strHue, int(defColorPiano.x), minColor, maxColor));
    colorGui.add(colorPianoS.setup("P " + strSat, int(defColorPiano.y), minColor, maxColor));
    colorGui.add(colorPianoB.setup("P " + strBright, int(defColorPiano.z), minColor, maxColor));
    colorGui.add(colorPianoA.setup("P " + strAlpha, int(defColorPiano.w), minColor, maxColor));
    colorPianoH.addListener(this, &XBBaseGUI::colorPianoHChanged);
    colorPianoS.addListener(this, &XBBaseGUI::colorPianoSChanged);
    colorPianoB.addListener(this, &XBBaseGUI::colorPianoBChanged);

    lblDirector.setBackgroundColor(ofColor::darkGoldenRod);
    lblCello.setBackgroundColor(ofColor(40, 100, 40));
    lblViolin.setBackgroundColor(ofColor(50, 110, 50));
    lblPiano.setBackgroundColor(ofColor(60, 120, 60));

    // Write XML settings files if they do not exist.
    if (!(ofFile::doesFileExist(STR_SETTINGS_BASEPATH + sceneSettingsFilename)))
        sceneGui.saveToFile(STR_SETTINGS_BASEPATH + sceneSettingsFilename);
    if (!(ofFile::doesFileExist(STR_SETTINGS_BASEPATH + colorSettingsFilename)))
        colorGui.saveToFile(STR_SETTINGS_BASEPATH + colorSettingsFilename);

    colorGui.setSize(colorGuiWidth, colorGuiWidth);
    colorGui.setWidthElements(colorGuiWidth);
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
    rgbColorDirectorG = int(c.g);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorDirectorSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorDirectorH, newS, colorDirectorB);
    rgbColorDirectorR = int(c.r);
    rgbColorDirectorG = int(c.g);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorDirectorBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorDirectorH, colorDirectorS, newB);
    rgbColorDirectorR = int(c.r);
    rgbColorDirectorG = int(c.g);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorCelloHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorCelloS, colorCelloB);
    rgbColorCelloR = int(c.r);
    rgbColorCelloG = int(c.g);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorCelloSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorCelloH, newS, colorCelloB);
    rgbColorCelloR = int(c.r);
    rgbColorCelloG = int(c.g);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorCelloBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorCelloH, colorCelloS, newB);
    rgbColorCelloR = int(c.r);
    rgbColorCelloG = int(c.g);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorViolinHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorViolinS, colorViolinB);
    rgbColorViolinR = int(c.r);
    rgbColorViolinG = int(c.g);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorViolinSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorViolinH, newS, colorViolinB);
    rgbColorViolinR = int(c.r);
    rgbColorViolinG = int(c.g);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorViolinBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorViolinH, colorViolinS, newB);
    rgbColorViolinR = int(c.r);
    rgbColorViolinG = int(c.g);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorPianoHChanged(int &newH) {
    ofColor c = ofColor::fromHsb(newH, colorPianoS, colorPianoB);
    rgbColorPianoR = int(c.r);
    rgbColorPianoG = int(c.g);
    rgbColorPianoB = int(c.b);
}

void XBBaseGUI::colorPianoSChanged(int &newS) {
    ofColor c = ofColor::fromHsb(colorPianoH, newS, colorPianoB);
    rgbColorPianoR = int(c.r);
    rgbColorPianoG = int(c.g);
    rgbColorPianoB = int(c.b);
}

void XBBaseGUI::colorPianoBChanged(int &newB) {
    ofColor c = ofColor::fromHsb(colorPianoH, colorPianoS, newB);
    rgbColorPianoR = int(c.r);
    rgbColorPianoG = int(c.g);
    rgbColorPianoB = int(c.b);
}
