//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_GUIAPP_H
#define PEDRERAMASTER_GUIAPP_H

#include "ofMain.h"
#include "XBSceneManager.h"

class GUIApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void setSceneManager(XBSceneManager *_sceneManager) { sceneManager = _sceneManager; };

private:

    XBSceneManager *sceneManager;

    void sceneChanged(int &sceneIndex) { cout << "Scene changed to " << sceneIndex << endl; };
};


#endif //PEDRERAMASTER_GUIAPP_H
