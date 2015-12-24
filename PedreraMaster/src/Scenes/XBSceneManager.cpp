//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBSceneManager.h"

XBSceneManager::XBSceneManager()
{
    state = SMS_None;
    currentSceneIndex = -1;
    nextSceneIndex = -1;
}

void XBSceneManager::addScene(XBBaseScene *scene)
{
    scenes.push_back(scene);
}

void XBSceneManager::update()
{
    if (scenes.size() == 0) return;

    switch(state)
    {
        case SMS_OnScene:
        {
            scenes[currentSceneIndex]->update();
            break;
        }
        case SMS_Transitioning:
        {
            scenes[currentSceneIndex]->update();
            scenes[nextSceneIndex]->update();
            break;
        }
        default: break;
    }
}

void XBSceneManager::draw()
{
    if (scenes.size() == 0) return;

    switch(state)
    {
        case SMS_OnScene:
        {
            scenes[currentSceneIndex]->drawIntoFBO();
            const ofFbo &sceneFBO = scenes[currentSceneIndex]->getMainFBO();
            sceneFBO.draw(0, 0);
            break;
        }
        case SMS_Transitioning:
        {
            drawWhileTransitioning();
            break;
        }
        default: break;
    }
}

void XBSceneManager::drawWhileTransitioning()
{
    // TODO: Should manage fades and timing
    scenes[currentSceneIndex]->drawIntoFBO();
    const ofFbo &srcSceneFBO = scenes[currentSceneIndex]->getMainFBO();
    srcSceneFBO.draw(0, 0);
    scenes[nextSceneIndex]->drawIntoFBO();
    const ofFbo &dstSceneFBO = scenes[nextSceneIndex]->getMainFBO();
    dstSceneFBO.draw(0, 0);
}

void XBSceneManager::goToScene(unsigned int sceneIndex, SceneTransitionMode transitionMode, unsigned int timeInMillis)
{
    if (sceneIndex >= scenes.size()) return;
    if (sceneIndex == currentSceneIndex) return;

    if (currentSceneIndex == -1) transitionMode = STM_Direct;

    switch(transitionMode)
    {
        case STM_Direct:
        {
            state = SMS_OnScene;
            currentSceneIndex = sceneIndex;
            break;
        }
        case STM_Fade:
        {
            state = SMS_Transitioning;
            nextSceneIndex = sceneIndex;
            break;
        }
        default: break;
    }
}

void XBSceneManager::goToNextScene(SceneTransitionMode transitionMode, unsigned int timeInMillis)
{
    unsigned int sceneIndex = (unsigned int)((currentSceneIndex + 1) % scenes.size());
    goToScene(sceneIndex, transitionMode, timeInMillis);
}

void XBSceneManager::goToPrevScene(SceneTransitionMode transitionMode, unsigned int timeInMillis)
{
    unsigned int sceneIndex = (currentSceneIndex == 0) ? ((unsigned int)(scenes.size() - 1)) : ((unsigned int)(currentSceneIndex - 1));
    goToScene(sceneIndex, transitionMode, timeInMillis);
}
