//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE3_H
#define PEDRERAMASTER_XBSCENE3_H
#define NUM_WAVES 1

#include "XBBaseScene.h"
#include "CustomBox2dParticle.h"
#include "ofxBox2d.h"
#include "Vehicle.h"
#include "ofxParticles.h"
#include "BlurEffect.h"
#include "Wave.h"
#include "ofxCustomGlow.h"

class XBScene3 : public XBBaseScene
{
public:
    
    XBScene3(const string &name) : XBBaseScene(name) {}
    
    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    virtual void drawGUI();
    void keyReleased(int key);
    void onViolinPitchChanged(float &pitch);
    void onViolinEnergyChanged(float &energy);
    void onCelloPitchChanged(float &pitch);
    void onCelloEnergyChanged(float &energy);
    void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn);
    void onPianoNoteOff(int &noteOff);
    void onKinectStateChanged(string &kState) {};
    void onKinectLPositionChanged(XBOSCManager::KinectPosVelArgs &lPos) ;
    void onKinectLVelocityChanged(XBOSCManager::KinectPosVelArgs &lVel);
    void onKinectRPositionChanged(XBOSCManager::KinectPosVelArgs &rPos);
    void onKinectRVelocityChanged(XBOSCManager::KinectPosVelArgs &rVel);

private:
    void updateViolin();
    void updateCello();
    void updateVioinCello();
    void updatePiano();
    void updateDirector();
    
    void drawDirector();
    void drawPiano();
    void drawViolinCello();

    void initStones();
    void initParticles();
    void initPaths();
    void initWaves();
    void initPhysics();
    
    bool fakeCelloEvent = false;
    float celloNote;
    bool fakePianoEvent = false;
    float pianoNote;
    bool fakeViolinEvent = false;
    float violinNote;
    
    Vehicle v, x;
    ofPolyline vPath;
    float vPathIndex = 0;
    ofPolyline xPath;
    float xPathIndex = 0;
    
    ofxParticleSystem particleSystem;
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    
    ofxSVG svg;
    vector<vector<expandingPolyLine>> stones;
    vector<expandingPolyLine> stonesToDraw;
    vector<int> midiToRowMapping;
    
    vector<Wave> waves;
    
    ofxBox2d  box2d;
    vector<shared_ptr<CustomBox2dParticle> > circles;
    vector<shared_ptr<ofxBox2dEdge> > edges;
    
    bool collisionOn = true;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE3_H
