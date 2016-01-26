//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBBASESCENE_H
#define PEDRERAMASTER_XBBASESCENE_H

#include "ofMain.h"
#include "XBBaseGUI.h"
#include "XBOSCManager.h"
#include "ofxGui.h"
#include "Defaults.h"

static const float MAX_MIDI_VALUE = 127.0f;

class XBBaseScene
{
public:

    XBBaseScene(string name);

    virtual void setup(XBBaseGUI *gui);
    virtual void update() {};
    virtual void drawIntoFBO() = 0;
    virtual void drawGUI();

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
    bool showTemplate;
    bool showFacadeImage;

    XBBaseGUI *gui;

    void subscribeToCelloEvents();
    void subscribeToViolinEvents();
    void subscribeToPianoEvents();
    void subscribeToKinectEvents();

    virtual void onCelloStarted() {};
    virtual void onCelloStopped() {};
    virtual void onCelloPitchChanged(float &pitch) {};
    virtual void onCelloEnergyChanged(float &energy) {};
    virtual void onCelloSilenceChanged(bool &isSilent) {};
    virtual void onCelloPauseChanged(bool &isPaused) {};
    virtual void onCelloOnsetDetected() {};

    virtual void onViolinStarted() {};
    virtual void onViolinStopped() {};
    virtual void onViolinPitchChanged(float &pitch) {};
    virtual void onViolinEnergyChanged(float &energy) {};
    virtual void onViolinSilenceChanged(bool &isSilent) {};
    virtual void onViolinPauseChanged(bool &isPaused) {};
    virtual void onViolinOnsetDetected() {};

    virtual void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn) {};
    virtual void onPianoNoteOff(int &noteOff) {};

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
