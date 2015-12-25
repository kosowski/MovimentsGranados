//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBSceneManager.h"
#include "ofxTweenzor.h"

#ifdef OF_DEBUG
const string GUI_FILENAME = "Settings/guiSettings.xml";
#endif

XBSceneManager::XBSceneManager()
{
    state = SCENESTATE_OnScene;
    currentSceneIndex = 0;
    nextSceneIndex = 0;
    showAll = false;

    Tweenzor::init();
}

void XBSceneManager::addScene(XBBaseScene *scene)
{
    scenes.push_back(scene);
}

void XBSceneManager::setup()
{
}

void XBSceneManager::update()
{
    if (scenes.size() == 0) return;

    if (!showAll)
    {
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
                Tweenzor::update(ofGetElapsedTimeMillis());
                break;
            }
            default: break;
        }
//        cout << "State: " << state << endl;
    }
    else
    {
        for (int i=0; i<scenes.size(); ++i)
            scenes[i]->update();
    }
}

void XBSceneManager::draw()
{
    if (scenes.size() == 0) return;

    if (!showAll)
    {
        switch(state)
        {
            // RECORDATORI: afegir ofxGUI per a controlar alphes de les dues escenes

            case SCENESTATE_OnScene:
            {
                ofEnableBlendMode(OF_BLENDMODE_ALPHA);
                drawSceneAtIndex(currentSceneIndex);
                ofDisableBlendMode();
                break;
            }
            case SCENESTATE_Transitioning:
            {
                cout << "Draw scenes " << currentSceneIndex << " and " << nextSceneIndex << endl;
                drawSceneAtIndex(currentSceneIndex);
                drawSceneAtIndex(nextSceneIndex);
                break;
            }
            default: break;
        }
    }
    else
    {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        for (int i=0; i<scenes.size(); ++i)
            drawSceneAtIndex(i);
        ofDisableBlendMode();
    }
}

void XBSceneManager::exit()
{
#ifdef OF_DEBUG
    if (!showAll) return;

    gui->saveSettings(GUI_FILENAME);
#endif
}

void XBSceneManager::goToScene(unsigned int sceneIndex, SceneTransitionMode transitionMode, float timeInSeconds)
{
    if (sceneIndex >= scenes.size()) return;
    if (sceneIndex == currentSceneIndex) return;

    showAll = false;

//    if (currentSceneIndex == -1) transitionMode = STM_Direct;

    switch(transitionMode)
    {
        case SCENETRANSITION_Direct:
        {
            state = SCENESTATE_OnScene;
            currentSceneIndex = sceneIndex;
            break;
        }
        case SCENETRANSITION_Fade:
        {
            state = SCENESTATE_Transitioning;
            nextSceneIndex = sceneIndex;

            scenes[currentSceneIndex]->setFBOAlpha(0.0f);
            scenes[nextSceneIndex]->setFBOAlpha(255.0f);

            Tweenzor::add(scenes[currentSceneIndex]->getFBOAlpha(), 0.0f, 255.0f, 0.0f, timeInSeconds, EASE_IN_OUT_SINE);
            Tweenzor::add(scenes[nextSceneIndex]->getFBOAlpha(), 255.0f, 0.0f, 0.0f, timeInSeconds, EASE_IN_OUT_SINE);
            Tweenzor::addCompleteListener(Tweenzor::getTween(scenes[nextSceneIndex]->getFBOAlpha()), this, &XBSceneManager::onFadeComplete);

            break;
        }
        default: break;
    }
}

void XBSceneManager::goToNextScene(SceneTransitionMode transitionMode, float timeInSeconds)
{
    unsigned int sceneIndex = (unsigned int)((currentSceneIndex + 1) % scenes.size());
    goToScene(sceneIndex, transitionMode, timeInSeconds);
}

void XBSceneManager::goToPrevScene(SceneTransitionMode transitionMode, float timeInSeconds)
{
    unsigned int sceneIndex = (currentSceneIndex == 0) ? ((unsigned int)(scenes.size() - 1)) : ((unsigned int)(currentSceneIndex - 1));
    goToScene(sceneIndex, transitionMode, timeInSeconds);
}

void XBSceneManager::drawSceneAtIndex(int sceneIndex)
{
    if (sceneIndex >= scenes.size()) return;

    scenes[sceneIndex]->drawIntoFBO();
    const ofFbo &sceneFBO = scenes[sceneIndex]->getMainFBO();
    sceneFBO.draw(0, 0);
}

void XBSceneManager::onFadeComplete(float *arg)
{
    state = SCENESTATE_OnScene;
    currentSceneIndex = nextSceneIndex;
    Tweenzor::removeAllTweens();
}

void XBSceneManager::showAllScenes()
{
    showAll = true;
#ifndef OF_DEBUG
    for (int i=0; i<scenes.size(); ++i)
        scenes[i]->setFBOAlpha(127.0f);
#else
    gui = new ofxUISuperCanvas("SCENES", OFX_UI_FONT_MEDIUM);

    gui->setColorBack(ofColor(0, 100, 127, 127));

    gui->addSpacer();
    for (int i=0; i<scenes.size(); ++i)
    {
        ofxUISlider *alphaSlider = gui->addSlider(scenes[i]->getName() + " alpha", 0.0, 255.0, scenes[i]->getFBOAlpha());
        alphaSlider->setID(i);
    }

    gui->autoSizeToFitWidgets();

    ofAddListener(gui->newGUIEvent, this, &XBSceneManager::handleGUIEvents);
    gui->loadSettings(GUI_FILENAME);

    gui->setVisible(true);
#endif
}

#ifdef OF_DEBUG
void XBSceneManager::handleGUIEvents(ofxUIEventArgs &e)
{
}
#endif
