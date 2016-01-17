//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "GUIApp.h"
#include "Defaults.h"
#include "MainApp.h"

static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS [GUI]";


void GUIApp::setup()
{
    ofSetWindowTitle(STR_WINDOW_TITLE);
    ofSetVerticalSync(false);

    ofBackground(68, 84, 71);

    int winX, winY;
    if (MAIN_WINDOW_MODE == OF_WINDOW)
    {
        int windowWidths = int(MAIN_WINDOW_WIDTH * MAIN_WINDOW_SCALE) + GUI_WINDOW_WIDTH;
        winX = ofGetScreenWidth()/2 - windowWidths/2 + int(MAIN_WINDOW_WIDTH * MAIN_WINDOW_SCALE);
        winY = ofGetScreenHeight()/2 - int(MAIN_WINDOW_HEIGHT * MAIN_WINDOW_SCALE)/2;
    }
    else
    {
        winX = ofGetScreenWidth()/2 - GUI_WINDOW_WIDTH/2;
        winY = ofGetScreenHeight()/2 - GUI_WINDOW_HEIGHT/2;
    }

    ofSetWindowPosition(winX, winY);

    currentGuiIndex = -1;

    XBScene1GUI *s1GUI = new XBScene1GUI();
    s1GUI->setup();
    guis.push_back(s1GUI);
    XBScene2GUI *s2GUI = new XBScene2GUI();
    s2GUI->setup();
    guis.push_back(s2GUI);
    XBScene3GUI *s3GUI = new XBScene3GUI();
    s3GUI->setup();
    guis.push_back(s3GUI);
    XBScene4GUI *s4GUI = new XBScene4GUI();
    s4GUI->setup();
    guis.push_back(s4GUI);

    ofAddListener(sceneManager->eventSceneChanged, this, &GUIApp::sceneChanged);

    addLegendMessage("KEYS:");
    addLegendMessage("- [LEFT] Go to Previous Scene");
    addLegendMessage("- [RIGHT] Go to Next Scene");
    addLegendMessage("- [T] Show facade template");
    addLegendMessage("- [F] Show facade bitmap");
}

void GUIApp::update()
{
}

void GUIApp::draw()
{
    if (currentGuiIndex >= 0)
        guis[currentGuiIndex]->draw();

    // Show keys info
    {
        int msgHeight = 16;
        float xMargin = 10;
        float yMargin = 10;

        float posX = xMargin;
        float posY = ofGetHeight() - (legendMessages.size() * msgHeight) - yMargin;

        for (int i=0; i<legendMessages.size(); ++i) {
            ofDrawBitmapString(legendMessages[i], posX, posY);
            posY += msgHeight;
        }
    }
}

void GUIApp::exit()
{
//    for (int i=0; i<guis.size(); ++i)
//        guis[i]->saveSettings();
}

void GUIApp::keyReleased(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
        case OF_KEY_DOWN:
        case OF_KEY_RIGHT:
        case OF_KEY_UP:
        case 'f':
        case 'F':
        case '1':
        case '2':
        case '3':
        case '4':
            mainApp->keyReleased(key);
        default: break;
    }
}

XBBaseGUI *GUIApp::getGuiForSceneIndex(int sceneIndex)
{
    if (sceneIndex >= guis.size()) return nullptr;

    return guis[sceneIndex];
}

void GUIApp::sceneChanged(int &sceneIndex)
{
//    if (currentGuiIndex >= 0)
//        guis[currentGuiIndex]->saveSettings();

    currentGuiIndex = sceneIndex;
}

void GUIApp::addLegendMessage(string msg)
{
    legendMessages.push_back(msg);
}
