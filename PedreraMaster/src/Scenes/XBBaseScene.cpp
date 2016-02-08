//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBBaseScene.h"

static const int FBO_NUM_SAMPLES = 0;

XBBaseScene::XBBaseScene(string _name)
{
    name = _name;
    
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB, FBO_NUM_SAMPLES);
    glowEffect.allocate(ofGetWidth(), ofGetHeight());
    glowEffect.setIntensity(1);
    glowEffect.setRadius(1);
}

void XBBaseScene::setup(XBBaseGUI *_gui)
{
    fbo.begin();
    {
        // Often the FBO will contain artifacts from the memory that the graphics card has just allocated for it,
        // so it's good to clear it before starting to draw it
        ofClear(0, 0, 0, 0);
    }
    fbo.end();
    
    fboAlpha = 255.0f;
    templateImage.load("resources/img/FotoPlantilla_16-10_Foto9615_v2 1200x1920.jpg");
    mask.load("resources/img/Mask_Ventanas_y_Arriba_y_Abajo_invert.png");

    violinBG.load("resources/img/01_Violin.png");
    pianoBG.load("resources/img/01_Piano.png");
    celloBG.load("resources/img/01_Cello.png");

    showTemplate = false;
    showFacadeImage = false;
    
    subscribeToCelloEvents();
    subscribeToViolinEvents();
    subscribeToPianoEvents();
    subscribeToKinectEvents();
    
    gui = _gui;
    active = false;
}

void XBBaseScene::enteredScene()
{
    active = true;
}

void XBBaseScene::leftScene()
{
    active = false;
}

void XBBaseScene::drawGUI()
{
}

void XBBaseScene::drawFadeRectangle()
{
    if (int(fboAlpha) == 255) return;
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    ofClear(0, 0, 0, (int)fboAlpha);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void XBBaseScene::drawMusiciansWindows()
{
    ofPushStyle();
    ofSetColor(ofColor(gui->rgbColorCelloR, gui->rgbColorCelloG, gui->rgbColorCelloB, ofClamp(255* celloEnergy, gui->minAlphaWindow, gui->maxAlphaWindow)));
    celloBG.draw(0,0, ofGetWidth(), ofGetHeight());
    ofSetColor(ofColor(gui->rgbColorViolinR, gui->rgbColorViolinG, gui->rgbColorViolinB, ofClamp(255 * violinEnergy, gui->minAlphaWindow, gui->maxAlphaWindow)));
    violinBG.draw(0,0, ofGetWidth(), ofGetHeight());
    ofSetColor(gui->rgbColorPianoR, gui->rgbColorPianoG, gui->rgbColorPianoB, ofClamp(255 * pianoEnergy, gui->minAlphaWindow, gui->maxAlphaWindow));
    pianoBG.draw(0,0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
}

void XBBaseScene::applyPostFX(){
    if(gui->useGlow){
        glowEffect.setIntensity(gui->glowAmount);
        glowEffect.setRadius(gui->glowradius);
        
        glowEffect << fbo.getTexture();
        glowEffect.update();
        fbo.begin();
        ofSetColor(255,255,255, gui->glowAlpha);
        glowEffect.draw(0,0, fbo.getWidth(), fbo.getHeight());
        fbo.end();
    }
}

 void XBBaseScene::onCelloSilenceChanged(bool &isSilent) {
     if(isSilent)
         celloEnergy = 0;
 }
 void XBBaseScene::onViolinSilenceChanged(bool &isSilent) {
     if(isSilent)
         violinEnergy = 0;
 }

void XBBaseScene::onViolinPitchChanged(float &pitch)
{
    if (!active)
        return;
    violinNote = pitch;
}

void XBBaseScene::onViolinEnergyChanged(float &energy)
{
    if (!active)
        return;
    if (energy <= energyThreshold)
        violinEnergy = 0;
    else
        violinEnergy = energy;
}

void XBBaseScene::onCelloPitchChanged(float &pitch)
{
    if (!active)
        return;
    celloNote = pitch;
}

void XBBaseScene::onCelloEnergyChanged(float &energy)
{
    if (!active)
        return;
    if (energy <= energyThreshold)
        celloEnergy = 0;
    else
        celloEnergy = energy;
}

void XBBaseScene::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn)
{
    if (!active)
        return;
    //    cout << "Piano NoteOn:  p=" << noteOn.pitch << " v=" << noteOn.velocity << endl;
    pianoNote = noteOn.pitch / MAX_MIDI_VALUE;
//    pianoEnergy = noteOn.velocity / MAX_MIDI_VALUE;
    pianoEnergy += (noteOn.velocity / MAX_MIDI_VALUE - pianoEnergy) * (1. - gui->pianoSmoothFactor);
}

void XBBaseScene::onPianoNoteOff(int &noteOff)
{
//    pianoEnergy *= gui->pianoDecay;
    lastPianoNoteTime = ofGetElapsedTimeMillis();
}

void XBBaseScene::onKinectLPositionChanged(XBOSCManager::KinectPosVelArgs &lPos)
{
    leftHand.pos.set(lPos.x, lPos.y, lPos.z);
}

void XBBaseScene::onKinectLVelocityChanged(XBOSCManager::KinectPosVelArgs &lVel)
{
    leftHand.velocity.set(lVel.x, lVel.y, lVel.z);
}

void XBBaseScene::onKinectRPositionChanged(XBOSCManager::KinectPosVelArgs &rPos)
{
    rightHand.pos.set(rPos.x, rPos.y, rPos.z);
}

void XBBaseScene::onKinectRVelocityChanged(XBOSCManager::KinectPosVelArgs &rVel)
{
    rightHand.velocity.set(rVel.x, rVel.y, rVel.z);
}

void XBBaseScene::keyReleased(int key)
{
    switch(key)
    {
        case 'f':
        case 'F':
            showFacadeImage = !showFacadeImage;
            break;
        case 't':
        case 'T':
            showTemplate = !showTemplate;
            break;
        default:
            break;
    }
}

void XBBaseScene::keyPressed(int key)
{
    switch(key)
    {
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
    XBOSCManager &oscManager = XBOSCManager::getInstance();
    ofAddListener(oscManager.eventKinectStateChanged, this, &XBBaseScene::onKinectStateChanged);
    ofAddListener(oscManager.eventKinectLPosition, this, &XBBaseScene::onKinectLPositionChanged);
    ofAddListener(oscManager.eventKinectLVelocity, this, &XBBaseScene::onKinectLVelocityChanged);
    ofAddListener(oscManager.eventKinectRPosition, this, &XBBaseScene::onKinectRPositionChanged);
    ofAddListener(oscManager.eventKinectRVelocity, this, &XBBaseScene::onKinectRVelocityChanged);
}
