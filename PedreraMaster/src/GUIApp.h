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

class GUIApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void setSceneManager(XBSceneManager *_sceneManager) { sceneManager = _sceneManager; };

    XBBaseGUI *getGuiForSceneIndex(int sceneIndex);

private:

    vector<XBBaseGUI *> guis;
    int currentGuiIndex;

    XBSceneManager *sceneManager;

    void sceneChanged(int &sceneIndex) { currentGuiIndex = sceneIndex; };
};


#endif //PEDRERAMASTER_GUIAPP_H
