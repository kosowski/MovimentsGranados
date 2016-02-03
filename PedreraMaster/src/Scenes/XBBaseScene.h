//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBBASESCENE_H
#define PEDRERAMASTER_XBBASESCENE_H

#include "ofMain.h"
#include "XBBaseGUI.h"
#include "XBOSCManager.h"
#include "ofxSvg.h"
#include "ofxCustomGlow.h"
#include "Defaults.h"

static const float MAX_MIDI_VALUE = 127.0f;
struct kinectHand{
    ofVec3f pos;
    ofVec3f velocity;
};

class XBBaseScene
{
public:

    XBBaseScene(string name);

    virtual void setup(XBBaseGUI *gui);
    virtual void enteredScene();
    virtual void leftScene();
    virtual void update() {};
    virtual void drawIntoFBO() = 0;
    virtual void drawGUI();
    void drawMusiciansWindows();
    void applyPostFX();

    const string &getName() const { return name; }
    const ofFbo &getMainFBO() const { return fbo; }

    void setFBOAlpha(float _fboAlpha) { fboAlpha = _fboAlpha; };
    float *getFBOAlpha() { return &fboAlpha; }

    virtual void keyReleased(int key);
    virtual void keyPressed(int key);

protected:

    void drawFadeRectangle();

    ofFbo fbo;
    float fboAlpha;
    ofImage templateImage;
    ofImage mask;
    ofImage violinBG, pianoBG, celloBG;
    bool showTemplate;
    bool showFacadeImage;
    
    float celloEnergy = 0;
    float violinEnergy = 0;
    float pianoEnergy = 0;
    float energyThreshold = 0.01;
    float celloNote;
    float pianoNote;
    float violinNote;
   
    kinectHand leftHand;
    kinectHand rightHand;
    
    ofxCustomGlow glowEffect;

    XBBaseGUI *gui;

    bool active;

    void subscribeToCelloEvents();
    void subscribeToViolinEvents();
    void subscribeToPianoEvents();
    void subscribeToKinectEvents();

    virtual void onCelloStarted() {};
    virtual void onCelloStopped() {};
    virtual void onCelloPitchChanged(float &pitch) ;
    virtual void onCelloEnergyChanged(float &energy) ;
    virtual void onCelloSilenceChanged(bool &isSilent);
    virtual void onCelloPauseChanged(bool &isPaused){};
    virtual void onCelloOnsetDetected() {};

    virtual void onViolinStarted() {};
    virtual void onViolinStopped() {};
    virtual void onViolinPitchChanged(float &pitch) ;
    virtual void onViolinEnergyChanged(float &energy) ;
    virtual void onViolinSilenceChanged(bool &isSilent);
    virtual void onViolinPauseChanged(bool &isPaused) {};
    virtual void onViolinOnsetDetected() {};

    virtual void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn) ;
    virtual void onPianoNoteOff(int &noteOff) ;

    virtual void onKinectStateChanged(string &kState) {};
    void onKinectLPositionChanged(XBOSCManager::KinectPosVelArgs &lPos) ;
    void onKinectLVelocityChanged(XBOSCManager::KinectPosVelArgs &lVel) ;
    void onKinectRPositionChanged(XBOSCManager::KinectPosVelArgs &rPos) ;
    void onKinectRVelocityChanged(XBOSCManager::KinectPosVelArgs &rVel) ;

private:

    string name;
};

struct expandingPolyLine{
    ofPolyline line;
    ofPath path;
    float life;
    float amplitude;
    ofPoint centroid;
};

#endif //PEDRERAMASTER_XBBASESCENE_H
