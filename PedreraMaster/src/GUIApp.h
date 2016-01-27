//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_GUIAPP_H
#define PEDRERAMASTER_GUIAPP_H

#include "ofMain.h"
#include "XBSceneManager.h"
#include "XBScene1GUI.h"
#include "XBScene2GUI.h"
#include "XBScene3GUI.h"
#include "XBScene4GUI.h"
#include "XBScene5GUI.h"
#include "XBScene6GUI.h"

class MainApp;

class GUIApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);

    void setSceneManager(XBSceneManager *_sceneManager) { sceneManager = _sceneManager; };

    XBBaseGUI *getGuiForSceneIndex(int sceneIndex);

    void setMainApp(const shared_ptr<MainApp> &mainApp) { GUIApp::mainApp = mainApp; };

private:

    vector<XBBaseGUI *> guis;
    int currentGuiIndex;
    shared_ptr<MainApp> mainApp;

    XBSceneManager *sceneManager;

    void sceneChanged(int &sceneIndex);

    void addLegendMessage(string msg);

    vector<string> legendMessages;
};


#endif //PEDRERAMASTER_GUIAPP_H
