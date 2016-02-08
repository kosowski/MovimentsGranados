//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "GUIApp.h"
#include "MainApp.h"
#include "XBSettingsManager.h"

static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS [GUI]";
static const string STR_APPSETTINGS_FILENAME = "settings/AppSettings.xml";


void GUIApp::setup()
{
    ofSetWindowTitle(STR_WINDOW_TITLE);
    ofSetVerticalSync(false);

    ofBackground(68, 84, 71);

    XBSettingsManager::getInstance().loadFile(STR_APPSETTINGS_FILENAME);
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    int winX, winY;
    if (MAIN_WINDOW_MODE == OF_WINDOW)
    {
        int windowWidths = int(MAIN_WINDOW_WIDTH * windowScale) + GUI_WINDOW_WIDTH;
        winX = ofGetScreenWidth()/2 - windowWidths/2 + int(MAIN_WINDOW_WIDTH * windowScale);
        winY = ofGetScreenHeight()/2 - int(MAIN_WINDOW_HEIGHT * windowScale)/2;
    }
    else
    {
        winX = ofGetScreenWidth()/2 - GUI_WINDOW_WIDTH/2;
        winY = ofGetScreenHeight()/2 - GUI_WINDOW_HEIGHT/2;
    }

    ofSetWindowShape(GUI_WINDOW_WIDTH, GUI_WINDOW_HEIGHT);
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
    XBScene5GUI *s5GUI = new XBScene5GUI();
    s5GUI->setup();
    guis.push_back(s5GUI);
    XBScene6GUI *s6GUI = new XBScene6GUI();
    s6GUI->setup();
    guis.push_back(s6GUI);

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

    drawLegend();
    drawSceneName();

    ofSetColor(ofColor::white);
    ofDrawBitmapStringHighlight(ofToString(roundf(ofGetFrameRate())) + "fps", ofGetWidth() - 55, ofGetHeight()/2 - 70);
}

void GUIApp::drawLegend()
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

void GUIApp::drawSceneName()
{
    string sceneName = sceneManager->getCurrentSceneName();
    int strLength = int(sceneName.size() * 8.0f);
    ofDrawBitmapStringHighlight(sceneManager->getCurrentSceneName(), ofGetWidth() - strLength - 10, ofGetHeight() - 10);
}

void GUIApp::exit()
{
//    for (int i=0; i<guis.size(); ++i)
//        guis[i]->saveSettings();
}

void GUIApp::keyPressed(int key)
{
    mainApp->keyPressed(key);
}

void GUIApp::keyReleased(int key)
{
    mainApp->keyReleased(key);
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
