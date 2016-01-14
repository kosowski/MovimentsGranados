//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBBaseGUI.h"

static const string strSceneName    = "SCENE SETTINGS";
static const string strColorName    = "COLOR SETTINGS";

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
    gui.setup(strSceneName);
    gui.setPosition(scenePosX, scenePosY);

    colorGui.setup(strColorName);
    colorGui.setPosition(colorsPosX, colorsPosY);

    colorGui.add(lblDirector.setup(strDirector, ""));
    colorGui.add(colorDirectorH.setup(strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorDirectorS.setup(strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorDirectorB.setup(strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorDirectorA.setup(strAlpha, defAlpha, minColor, maxColor));

    colorGui.add(lblCello.setup(strCello, ""));
    colorGui.add(colorCelloH.setup(strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloS.setup(strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloB.setup(strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorCelloA.setup(strAlpha, defAlpha, minColor, maxColor));

    colorGui.add(lblViolin.setup(strViolin, ""));
    colorGui.add(colorViolinH.setup(strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinS.setup(strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinB.setup(strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorViolinA.setup(strAlpha, defAlpha, minColor, maxColor));

    colorGui.add(lblPiano.setup(strPiano, ""));
    colorGui.add(colorPianoH.setup(strHue, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoS.setup(strSat, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoB.setup(strBright, defColorValue, minColor, maxColor));
    colorGui.add(colorPianoA.setup(strAlpha, defAlpha, minColor, maxColor));

    lblDirector.setBackgroundColor(ofColor::darkGoldenRod);
    lblCello.setBackgroundColor(ofColor(40, 100, 40));
    lblViolin.setBackgroundColor(ofColor(50, 110, 50));
    lblPiano.setBackgroundColor(ofColor(60, 120, 60));
}

void XBBaseGUI::draw()
{
    gui.draw();
    colorGui.draw();
}
