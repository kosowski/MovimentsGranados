//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBBaseScene.h"

static const int FBO_NUM_SAMPLES = 0;

XBBaseScene::XBBaseScene(string _name)
{
    name = _name;

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB, FBO_NUM_SAMPLES);
}

void XBBaseScene::setup()
{
    fbo.begin();
    {
        // Often the FBO will contain artifacts from the memory that the graphics card has just allocated for it,
        // so it's good to clear it before starting to draw it
        ofClear(0, 0, 0, 0);
    }
    fbo.end();

    fboAlpha = 255.0f;

    subscribeToCelloEvents();
    subscribeToViolinEvents();
    subscribeToPianoEvents();
    subscribeToKinectEvents();
}

void XBBaseScene::drawGUI()
{
    if (showGUI) guiOld.draw();
}

void XBBaseScene::drawFadeRectangle()
{
    if (int(fboAlpha) == 255) return;

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    ofClear(0, 0, 0, (int)fboAlpha);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void XBBaseScene::keyReleased(int key)
{
    switch(key)
    {
        case 'g':
        case 'G':
            showGUI = !showGUI;
            break;
        default:
            break;
    }
}

void XBBaseScene::subscribeToCelloEvents()
{
    XBOSCManager &oscManager = XBOSCManager::getInstance();
    ofAddListener(oscManager.eventCelloStarted, this, &XBBaseScene::onCelloStarted);
    ofAddListener(oscManager.eventCelloStopped, this, &XBBaseScene::onCelloStopped);
    ofAddListener(oscManager.eventCelloPitchChanged, this, &XBBaseScene::onCelloPitchChanged);
    ofAddListener(oscManager.eventCelloEnergyChanged, this, &XBBaseScene::onCelloEnergyChanged);
    ofAddListener(oscManager.eventCelloSilenceChanged, this, &XBBaseScene::onCelloSilenceChanged);
    ofAddListener(oscManager.eventCelloPauseChanged, this, &XBBaseScene::onCelloPauseChanged);
    ofAddListener(oscManager.eventCelloOnsetDetected, this, &XBBaseScene::onCelloOnsetDetected);
}

void XBBaseScene::subscribeToViolinEvents()
{
    XBOSCManager &oscManager = XBOSCManager::getInstance();
    ofAddListener(oscManager.eventViolinStarted, this, &XBBaseScene::onViolinStarted);
    ofAddListener(oscManager.eventViolinStopped, this, &XBBaseScene::onViolinStopped);
    ofAddListener(oscManager.eventViolinPitchChanged, this, &XBBaseScene::onViolinPitchChanged);
    ofAddListener(oscManager.eventViolinEnergyChanged, this, &XBBaseScene::onViolinEnergyChanged);
    ofAddListener(oscManager.eventViolinSilenceChanged, this, &XBBaseScene::onViolinSilenceChanged);
    ofAddListener(oscManager.eventViolinPauseChanged, this, &XBBaseScene::onViolinPauseChanged);
    ofAddListener(oscManager.eventViolinOnsetDetected, this, &XBBaseScene::onViolinOnsetDetected);
}

void XBBaseScene::subscribeToPianoEvents()
{
    XBOSCManager &oscManager = XBOSCManager::getInstance();
    ofAddListener(oscManager.eventPianoNoteOn, this, &XBBaseScene::onPianoNoteOn);
    ofAddListener(oscManager.eventPianoNoteOff, this, &XBBaseScene::onPianoNoteOff);
}

void XBBaseScene::subscribeToKinectEvents()
{
}
