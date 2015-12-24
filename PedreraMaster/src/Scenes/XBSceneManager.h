//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENEMANAGER_H
#define PEDRERAMASTER_XBSCENEMANAGER_H

#include "XBBaseScene.h"

typedef enum
{
    STM_Direct = 0,
    STM_Fade = 1
} SceneTransitionMode;

typedef enum
{
    SMS_None = 0,
    SMS_OnScene = 1,
    SMS_Transitioning = 2
} SceneManagerState;

class XBSceneManager
{
public:

    XBSceneManager();

    void addScene(XBBaseScene *scene);

    void update();
    void draw();
    void drawWhileTransitioning();

    void goToScene(unsigned int sceneIndex, SceneTransitionMode transitionMode = STM_Direct, unsigned int timeInMillis = 0);
    void goToNextScene(SceneTransitionMode transitionMode = STM_Direct, unsigned int timeInMillis = 0);
    void goToPrevScene(SceneTransitionMode transitionMode = STM_Direct, unsigned int timeInMillis = 0);
    int getCurrentSceneIndex() const { return currentSceneIndex; };

private:

    vector<XBBaseScene *> scenes;
    int currentSceneIndex, nextSceneIndex;
    SceneManagerState state;
};


#endif //PEDRERAMASTER_XBSCENEMANAGER_H
