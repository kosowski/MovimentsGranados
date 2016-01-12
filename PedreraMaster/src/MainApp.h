#pragma once

#ifndef PEDRERAMASTER_OFAPP_H
#define PEDRERAMASTER_OFAPP_H

#include "ofMain.h"

#include "XBSceneManager.h"

class MainApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

    void keyReleased(int key);

private:

    void sceneChanged(int &sceneIndex) { cout << "Scene changed to " << sceneIndex << endl; };

    XBSceneManager sceneManager;

    bool showFPS;
};

#endif // PEDRERAMASTER_OFAPP_H
