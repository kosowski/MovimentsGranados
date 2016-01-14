//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "GUIApp.h"
#include "Defaults.h"

static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS [GUI]";


void GUIApp::setup()
{
    ofSetWindowTitle(STR_WINDOW_TITLE);
    ofSetVerticalSync(false);

    ofBackground(68, 84, 71);

    int winX, winY;
    if (MAIN_WINDOW_MODE == OF_WINDOW)
    {
        int windowWidths = MAIN_WINDOW_WIDTH + GUI_WINDOW_WIDTH;
        winX = ofGetScreenWidth()/2 - windowWidths/2 + MAIN_WINDOW_WIDTH;
        winY = ofGetScreenHeight()/2 - MAIN_WINDOW_HEIGHT/2;
    }
    else
    {
        winX = ofGetScreenWidth()/2 - GUI_WINDOW_WIDTH/2;
        winY = ofGetScreenHeight()/2 - GUI_WINDOW_HEIGHT/2;
    }

    ofSetWindowPosition(winX, winY);

    currentGuiIndex = -1;

    XBScene1GUI *s1GUI = new XBScene1GUI();
    guis.push_back(s1GUI);
    XBScene2GUI *s2GUI = new XBScene2GUI();
    guis.push_back(s2GUI);
    XBScene3GUI *s3GUI = new XBScene3GUI();
    guis.push_back(s3GUI);
    XBScene4GUI *s4GUI = new XBScene4GUI();
    guis.push_back(s4GUI);

    ofAddListener(sceneManager->eventSceneChanged, this, &GUIApp::sceneChanged);
}

void GUIApp::update()
{
}

void GUIApp::draw()
{
    if (currentGuiIndex >= 0)
        guis[currentGuiIndex]->draw();
}

void GUIApp::exit()
{
}

XBBaseGUI *GUIApp::getGuiForSceneIndex(int sceneIndex)
{
    if (sceneIndex >= guis.size()) return nullptr;

    return guis[sceneIndex];
}

void GUIApp::sceneChanged(int &sceneIndex)
{
    currentGuiIndex = sceneIndex;
}