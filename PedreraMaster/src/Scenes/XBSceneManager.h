//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENEMANAGER_H
#define PEDRERAMASTER_XBSCENEMANAGER_H

#include "XBBaseScene.h"

typedef enum
{
    SCENESTATE_OnScene = 0,
    SCENESTATE_Transitioning = 1
} SceneTransitionState;

class XBSceneManager
{
public:

    XBSceneManager();

    void addScene(XBBaseScene *scene);

    void setup(int initialScene);
    void update();
    void draw();

    void exit();

    void goToScene(unsigned int sceneIndex, float fadeOutTime = 0.0f, float fadeInTime = 0.0f);
    void goToNextScene(float fadeOutTime = 0.0f, float fadeInTime = 0.0f);
    void goToPrevScene(float fadeOutTime = 0.0f, float fadeInTime = 0.0f);
    void keyReleased(int key);
    void keyPressed(int key);
    XBBaseScene *getCurrentScene();
    string getCurrentSceneName();

    ofEvent<int> eventSceneChanged;

private:

    void drawSceneAtIndex(int sceneIndex);
    void onFadeComplete(float* arg);

    vector<XBBaseScene *> scenes;
    int currentSceneIndex, nextSceneIndex;
    SceneTransitionState state;
};


#endif //PEDRERAMASTER_XBSCENEMANAGER_H
