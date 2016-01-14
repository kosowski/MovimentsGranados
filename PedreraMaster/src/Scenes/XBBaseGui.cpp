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

    colorGui.add(lblCello.setup(strCello, ""));
    colorGui.add(colorCelloH.setup("C " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloS.setup("C " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloB.setup("C " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloA.setup("C " + strAlpha, defAlpha, minColor, maxColor));

    colorGui.add(lblViolin.setup(strViolin, ""));
    colorGui.add(colorViolinH.setup("V " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinS.setup("V " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinB.setup("V " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinA.setup("V " + strAlpha, defAlpha, minColor, maxColor));

    colorGui.add(lblPiano.setup(strPiano, ""));
    colorGui.add(colorPianoH.setup("P " + strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoS.setup("P " + strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoB.setup("P " + strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoA.setup("P " + strAlpha, defAlpha, minColor, maxColor));

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
