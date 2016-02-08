//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBSceneManager.h"
#include "ofxTweenzor.h"

XBSceneManager::XBSceneManager()
{
    state = SCENESTATE_OnScene;
    currentSceneIndex = -1;
    nextSceneIndex = -1;
}

void XBSceneManager::addScene(XBBaseScene *scene)
{
    scenes.push_back(scene);
}

void XBSceneManager::setup(int initialScene)
{
    currentSceneIndex = nextSceneIndex = initialScene;

    fadingOut = fadingIn = false;
    Tweenzor::init();

    scenes[currentSceneIndex]->enteredScene();
    ofNotifyEvent(eventSceneChanged, currentSceneIndex);
}

void XBSceneManager::update()
{
    if (scenes.size() == 0) return;
    if (currentSceneIndex < 0) return;

    Tweenzor::update(ofGetElapsedTimeMillis());

    switch(state)
    {
        case SCENESTATE_OnScene:
        {
            scenes[currentSceneIndex]->update();
            break;
        }
        case SCENESTATE_Transitioning:
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
    if (currentSceneIndex < 0) return;

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    switch(state)
    {
        case SCENESTATE_OnScene:
        {
            drawSceneAtIndex(currentSceneIndex);
            break;
        }
        case SCENESTATE_Transitioning:
        {
            drawSceneAtIndex(currentSceneIndex);
            drawSceneAtIndex(nextSceneIndex);
            break;
        }
        default: break;
    }
}

void XBSceneManager::exit()
{
}

void XBSceneManager::goToScene(unsigned int sceneIndex, float fadeOutTime, float fadeInTime)
{
    if (sceneIndex >= scenes.size()) return;
    if (sceneIndex == currentSceneIndex) return;
    if (state == SCENESTATE_Transitioning) return;

    state = SCENESTATE_Transitioning;
    nextSceneIndex = sceneIndex;

    scenes[currentSceneIndex]->setFBOAlpha(255.0f);
    scenes[nextSceneIndex]->setFBOAlpha(0.0f);

    scenes[nextSceneIndex]->enteredScene();

    float tweenDelay = 0.0f;

    fadingOut = true;
    Tweenzor::add(scenes[currentSceneIndex]->getFBOAlpha(), 255.0f, 0.0f, tweenDelay, fadeOutTime, EASE_OUT_SINE);
    Tween *srcTween = Tweenzor::getTween(scenes[currentSceneIndex]->getFBOAlpha());
    Tweenzor::addCompleteListener(srcTween, this, &XBSceneManager::onFadeOutComplete);

    fadingIn = true;
    Tweenzor::add(scenes[nextSceneIndex]->getFBOAlpha(), 0.0f, 255.0f, tweenDelay, fadeInTime, EASE_IN_SINE);
    Tween *dstTween = Tweenzor::getTween(scenes[nextSceneIndex]->getFBOAlpha());
    Tweenzor::addCompleteListener(dstTween, this, &XBSceneManager::onFadeInComplete);
}

void XBSceneManager::goToNextScene(float fadeOutTime, float fadeInTime)
{
    unsigned int sceneIndex = (unsigned int)((currentSceneIndex + 1) % scenes.size());
    goToScene(sceneIndex, fadeOutTime, fadeInTime);
}

void XBSceneManager::goToPrevScene(float fadeOutTime, float fadeInTime)
{
    unsigned int sceneIndex = (currentSceneIndex == 0) ? ((unsigned int)(scenes.size() - 1)) : ((unsigned int)(currentSceneIndex - 1));
    goToScene(sceneIndex, fadeOutTime, fadeInTime);
}

void XBSceneManager::drawSceneAtIndex(int sceneIndex)
{
    if (sceneIndex >= scenes.size()) return;

    scenes[sceneIndex]->drawIntoFBO();
    const ofFbo &sceneFBO = scenes[sceneIndex]->getMainFBO();
    sceneFBO.draw(0, 0);
}

XBBaseScene *XBSceneManager::getCurrentScene()
{
    if (currentSceneIndex == -1) return nullptr;
    return scenes[currentSceneIndex];
}

string XBSceneManager::getCurrentSceneName()
{
    switch (state)
    {
        case SCENESTATE_OnScene:
            return getCurrentScene()->getName();
        case SCENESTATE_Transitioning:
        {
            string sceneName = scenes[currentSceneIndex]->getName() + " > " + scenes[nextSceneIndex]->getName();
            return sceneName;
        }
    }
}

void XBSceneManager::onFadeOutComplete(float *arg)
{
    Tween *tween = Tweenzor::getTween(scenes[currentSceneIndex]->getFBOAlpha());
    Tweenzor::removeTween(scenes[currentSceneIndex]->getFBOAlpha());
    Tweenzor::removeCompleteListener(tween);

    fadingOut = false;

    scenes[currentSceneIndex]->leftScene();
    if (!fadingIn)
    {
        currentSceneIndex = nextSceneIndex;
        state = SCENESTATE_OnScene;
        ofNotifyEvent(eventSceneChanged, currentSceneIndex);
    }
}

void XBSceneManager::onFadeInComplete(float *arg)
{
    Tween *tween = Tweenzor::getTween(scenes[nextSceneIndex]->getFBOAlpha());
    Tweenzor::removeTween(scenes[nextSceneIndex]->getFBOAlpha());
    Tweenzor::removeCompleteListener(tween);

    fadingIn = false;

    if (!fadingOut)
    {
//        scenes[currentSceneIndex]->leftScene();
        currentSceneIndex = nextSceneIndex;
        state = SCENESTATE_OnScene;
        ofNotifyEvent(eventSceneChanged, currentSceneIndex);
    }
}

void XBSceneManager::keyReleased(int key)
{
    scenes[currentSceneIndex]->keyReleased(key);
}

void XBSceneManager::keyPressed(int key)
{
    scenes[currentSceneIndex]->keyPressed(key);
}
