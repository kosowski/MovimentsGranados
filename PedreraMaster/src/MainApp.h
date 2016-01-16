#pragma once

#ifndef PEDRERAMASTER_OFAPP_H
#define PEDRERAMASTER_OFAPP_H

#include "ofMain.h"
#include "GUIApp.h"
#include "XBSceneManager.h"

class MainApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);

    XBSceneManager *getSceneManager() { return &sceneManager; };

    void setGuiApp(const shared_ptr<GUIApp> &guiApp) { MainApp::guiApp = guiApp; };

private:

    XBSceneManager sceneManager;
    shared_ptr<GUIApp> guiApp;

    bool showFPS;
};

#endif // PEDRERAMASTER_OFAPP_H
