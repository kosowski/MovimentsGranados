//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE2_H
#define PEDRERAMASTER_XBSCENE2_H

#include "XBBaseScene.h"
#include "ofxTraerPhysics.h"

using namespace  traer::physics ;

class XBScene2 : public XBBaseScene
{
public:

    XBScene2(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyReleased(int key);
    void keyPressed(int key);
    void onViolinPitchChanged(float &pitch);
    void onViolinEnergyChanged(float &energy);
    void onCelloPitchChanged(float &pitch);
    void onCelloEnergyChanged(float &energy);
    void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn);
    void onPianoNoteOff(int &noteOff);
    

    void initSystem();
    void initWindows(string name,  vector<ofRectangle>& vectorWindows, int starIndex, int floor);
    void arrangeWindows(int index, vector<ofRectangle>& elements);
    
    void drawWindow(float note, vector<ofRectangle>& windows);
    
    ofFbo auxFbo;
    ofxSVG svg;
    vector<ofRectangle> celloWindows;
    vector<ofRectangle> violinWindows;
    vector<ofRectangle> pianoWindows;
    vector<ofPolyline> celloLines;
    ofImage windowMask;
    
    ParticleSystem * 	physics;
    vector<   Particle * > particles;
    vector<   Spring * > visibleSprings;
    Particle*			selectedParticle;
    Particle* p_mouse;
    
    ofColor directorColor;
    
    bool fakeCelloEvent = false;
    float celloNote;
    float celloEnergy = 0;
    bool fakePianoEvent = false;
    float pianoNote;
    float pianoEnergy = 0;
    bool fakeViolinEvent = false;
    float violinNote;
    float violinEnergy = 0;
    
    float energyThreshold = 0.01;
};


#endif //PEDRERAMASTER_XBSCENE2_H
