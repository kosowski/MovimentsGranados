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

    ofAddListener(sceneManager->eventSceneChanged, this, &GUIApp::sceneChanged);
}

void GUIApp::update()
{
}

void GUIApp::draw()
{
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", 15, ofGetHeight() - 15);

    int fontSize = 8;

    string msg1 = "NOTHING TO SEE HERE (for the moment)";
    int msg1Width = int(msg1.size()) * fontSize;
    ofDrawBitmapStringHighlight(msg1, ofGetWidth()/2 - msg1Width/2, ofGetHeight()/2 - fontSize, ofColor::black, ofColor(255, 170, 170));

    string msg2 = "Expect to see SCENE GUIs in this window";
    int msg2Width = int(msg2.size()) * fontSize;
    ofDrawBitmapStringHighlight(msg2, ofGetWidth()/2 - msg2Width/2, ofGetHeight()/2 + fontSize, ofColor::black, ofColor::white);

    XBBaseScene *currentScene = sceneManager->getCurrentScene();
    if (currentScene != NULL)
    {
//        currentScene->getGUI()->draw();
    }
}

void GUIApp::exit()
{
}
