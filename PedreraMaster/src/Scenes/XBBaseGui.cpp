//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBBaseGUI.h"
#include "ofMain.h"

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
static const ofVec4f defColorCello(11, 71, 81, 255);
static const ofVec4f defColorViolin(46, 63, 95, 255);
static const ofVec4f defColorPiano(147, 22, 36, 255);

static const int minHue             = 0;
static const int maxHue             = 360;
static const int minSaturation      = 0;
static const int maxSaturation      = 100;
static const int minBrightness      = 0;
static const int maxBrightness      = 100;
static const int minAlpha           = 0;
static const int maxAlpha           = 255;

static const float maxSavedMessageTime = 3.0f;

XBBaseGUI::XBBaseGUI()
{
}

void XBBaseGUI::setup()
{
    sceneGui.setup(strSceneGuiTitle, STR_SETTINGS_BASEPATH + sceneSettingsFilename);
    sceneGui.setPosition(scenePosX, scenePosY);
    sceneGui.add(simulateHands.setup("Simulate hands", false));
    
    sceneGui.add(useGlow.setup("Apply glow", false));
    sceneGui.add(glowAmount.setup("Glow amount", 1, 0, 5));
    sceneGui.add(glowradius.setup("Glow radius", 1, 1, 10));
    
    sceneGui.add(minAlphaWindow.setup("Min window alpha", 1, 0, 255));
    sceneGui.add(maxAlphaWindow.setup("Max window alpha", 255, 0, 255));

    colorGui.setup(strColorGuiTitle, STR_SETTINGS_BASEPATH + colorSettingsFilename);
    colorGui.setPosition(colorsPosX, colorsPosY);

    colorGui.add(lblDirector.setup(strDirector, ""));
    colorGui.add(colorDirectorH.setup("D " + strHue, int(defColorDirector.x), minHue, maxHue));
    colorGui.add(colorDirectorS.setup("D " + strSat, int(defColorDirector.y), minSaturation, maxSaturation));
    colorGui.add(colorDirectorB.setup("D " + strBright, int(defColorDirector.z), minBrightness, maxBrightness));
    colorGui.add(colorDirectorA.setup("D " + strAlpha, int(defColorDirector.w), minAlpha, maxAlpha));
    colorDirectorH.addListener(this, &XBBaseGUI::colorDirectorHChanged);
    colorDirectorS.addListener(this, &XBBaseGUI::colorDirectorSChanged);
    colorDirectorB.addListener(this, &XBBaseGUI::colorDirectorBChanged);

    colorGui.add(lblCello.setup(strCello, ""));
    colorGui.add(colorCelloH.setup("C " + strHue, int(defColorCello.x), minHue, maxHue));
    colorGui.add(colorCelloS.setup("C " + strSat, int(defColorCello.y), minSaturation, maxSaturation));
    colorGui.add(colorCelloB.setup("C " + strBright, int(defColorCello.z), minBrightness, maxBrightness));
    colorGui.add(colorCelloA.setup("C " + strAlpha, int(defColorCello.w), minAlpha, maxAlpha));
    colorCelloH.addListener(this, &XBBaseGUI::colorCelloHChanged);
    colorCelloS.addListener(this, &XBBaseGUI::colorCelloSChanged);
    colorCelloB.addListener(this, &XBBaseGUI::colorCelloBChanged);

    colorGui.add(lblViolin.setup(strViolin, ""));
    colorGui.add(colorViolinH.setup("V " + strHue, int(defColorViolin.x), minHue, maxHue));
    colorGui.add(colorViolinS.setup("V " + strSat, int(defColorViolin.y), minSaturation, maxSaturation));
    colorGui.add(colorViolinB.setup("V " + strBright, int(defColorViolin.z), minBrightness, maxBrightness));
    colorGui.add(colorViolinA.setup("V " + strAlpha, int(defColorViolin.w), minAlpha, maxAlpha));
    colorViolinH.addListener(this, &XBBaseGUI::colorViolinHChanged);
    colorViolinS.addListener(this, &XBBaseGUI::colorViolinSChanged);
    colorViolinB.addListener(this, &XBBaseGUI::colorViolinBChanged);

    colorGui.add(lblPiano.setup(strPiano, ""));
    colorGui.add(colorPianoH.setup("P " + strHue, int(defColorPiano.x), minHue, maxHue));
    colorGui.add(colorPianoS.setup("P " + strSat, int(defColorPiano.y), minSaturation, maxSaturation));
    colorGui.add(colorPianoB.setup("P " + strBright, int(defColorPiano.z), minBrightness, maxBrightness));
    colorGui.add(colorPianoA.setup("P " + strAlpha, int(defColorPiano.w), minAlpha, maxAlpha));
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

    ofAddListener(sceneGui.savePressedE, this, &XBBaseGUI::saveScenePressed);
    ofAddListener(colorGui.savePressedE, this, &XBBaseGUI::saveColorsPressed);

    showSavedMessage = false;
}

void XBBaseGUI::draw()
{
    sceneGui.draw();
    colorGui.draw();

    if (showSavedMessage) drawSettingsSaved();
}

void XBBaseGUI::drawSettingsSaved()
{
    if (showSavedMessage)
    {
        float t = ofGetElapsedTimef();
        if (t - showSavedStartTime < maxSavedMessageTime)
        {
            ofSetColor(ofColor::white);
            ofDrawBitmapString(savePressedMessage, ofGetWidth()-180, ofGetHeight() - 35);
            ofDrawBitmapString(savePressedMessage, ofGetWidth()-179, ofGetHeight() - 35);
        }
        else
        {
            showSavedMessage = false;
        }
    }
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
    ofColor c = HSBtoRGB(newH, colorDirectorS, colorDirectorB);
    rgbColorDirectorR = int(c.r);
    rgbColorDirectorG = int(c.g);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorDirectorSChanged(int &newS) {
    ofColor c = HSBtoRGB(colorDirectorH, newS, colorDirectorB);
    rgbColorDirectorR = int(c.r);
    rgbColorDirectorG = int(c.g);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorDirectorBChanged(int &newB) {
    ofColor c = HSBtoRGB(colorDirectorH, colorDirectorS, newB);
    rgbColorDirectorR = int(c.r);
    rgbColorDirectorG = int(c.g);
    rgbColorDirectorB = int(c.b);
}

void XBBaseGUI::colorCelloHChanged(int &newH) {
    ofColor c = HSBtoRGB(newH, colorCelloS, colorCelloB);
    rgbColorCelloR = int(c.r);
    rgbColorCelloG = int(c.g);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorCelloSChanged(int &newS) {
    ofColor c = HSBtoRGB(colorCelloH, newS, colorCelloB);
    rgbColorCelloR = int(c.r);
    rgbColorCelloG = int(c.g);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorCelloBChanged(int &newB) {
    ofColor c = HSBtoRGB(colorCelloH, colorCelloS, newB);
    rgbColorCelloR = int(c.r);
    rgbColorCelloG = int(c.g);
    rgbColorCelloB = int(c.b);
}

void XBBaseGUI::colorViolinHChanged(int &newH) {
    ofColor c = HSBtoRGB(newH, colorViolinS, colorViolinB);
    rgbColorViolinR = int(c.r);
    rgbColorViolinG = int(c.g);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorViolinSChanged(int &newS) {
    ofColor c = HSBtoRGB(colorViolinH, newS, colorViolinB);
    rgbColorViolinR = int(c.r);
    rgbColorViolinG = int(c.g);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorViolinBChanged(int &newB) {
    ofColor c = HSBtoRGB(colorViolinH, colorViolinS, newB);
    rgbColorViolinR = int(c.r);
    rgbColorViolinG = int(c.g);
    rgbColorViolinB = int(c.b);
}

void XBBaseGUI::colorPianoHChanged(int &newH) {
    ofColor c = HSBtoRGB(newH, colorPianoS, colorPianoB);
    rgbColorPianoR = int(c.r);
    rgbColorPianoG = int(c.g);
    rgbColorPianoB = int(c.b);
}

void XBBaseGUI::colorPianoSChanged(int &newS) {
    ofColor c = HSBtoRGB(colorPianoH, newS, colorPianoB);
    rgbColorPianoR = int(c.r);
    rgbColorPianoG = int(c.g);
    rgbColorPianoB = int(c.b);
}

void XBBaseGUI::colorPianoBChanged(int &newB) {
    ofColor c = HSBtoRGB(colorPianoH, colorPianoS, newB);
    rgbColorPianoR = int(c.r);
    rgbColorPianoG = int(c.g);
    rgbColorPianoB = int(c.b);
}

ofColor XBBaseGUI::HSBtoRGB(float h, float s, float b)
{
    float myH = ofMap(h, minHue, maxHue, 0, 255);
    float myS = ofMap(s, minSaturation, maxSaturation, 0, 255);
    float myB = ofMap(b, minBrightness, maxBrightness, 0, 255);

    return ofColor::fromHsb(myH, myS, myB);
}

void XBBaseGUI::saveScenePressed()
{
    cout << "Save Scene pressed" << endl;

    showSavedMessage = true;
    savePressedMessage = "Scene Settings Saved!";
    showSavedStartTime = ofGetElapsedTimef();
}

void XBBaseGUI::saveColorsPressed()
{
    cout << "Save colors pressed" << endl;

    showSavedMessage = true;
    savePressedMessage = "Color Settings Saved!";
    showSavedStartTime = ofGetElapsedTimef();
}
