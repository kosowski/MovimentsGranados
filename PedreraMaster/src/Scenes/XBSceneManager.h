//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENEMANAGER_H
#define PEDRERAMASTER_XBSCENEMANAGER_H

#include "XBBaseScene.h"

typedef enum
{
    SCENETRANSITION_Direct = 0,
    SCENETRANSITION_Fade = 1
} SceneTransitionMode;

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

    void goToScene(unsigned int sceneIndex, SceneTransitionMode transitionMode = SCENETRANSITION_Direct, float timeInSeconds = 0.0f);
    void goToNextScene(SceneTransitionMode transitionMode = SCENETRANSITION_Direct, float timeInSeconds = 0.0f);
    void goToPrevScene(SceneTransitionMode transitionMode = SCENETRANSITION_Direct, float timeInSeconds = 0.0f);
    void keyReleased(int key);
    void keyPressed(int key);
    XBBaseScene *getCurrentScene();

    ofEvent<int> eventSceneChanged;

private:

    void drawSceneAtIndex(int sceneIndex);
    void onFadeComplete(float* arg);

    vector<XBBaseScene *> scenes;
    int currentSceneIndex, nextSceneIndex;
    SceneTransitionState state;
};


#endif //PEDRERAMASTER_XBSCENEMANAGER_H
