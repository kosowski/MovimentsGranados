//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE1_H
#define PEDRERAMASTER_XBSCENE1_H

#include "XBBaseScene.h"
#include "BlurEffect.h"
#include "ofxParticles.h"
#include "Wave.h"


class XBScene1 : public XBBaseScene
{
public:
    
    XBScene1(const string &name) : XBBaseScene(name) {}
    
    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyPressed(int key);
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
    
    void initWaves();
    void initLines();
    void initParticles();
    void initStones();
    void initWindows();
    void updateEmitters();
    
private:

    int findIntersectionVertical(ofPolyline &line, int posY);
    int findIntersectionHorizontal(ofPolyline &line, int posX);
        
    ofxSVG svg;
    vector<ofPolyline> verticalLines;
    vector<ofPolyline> horizontalLines;
    int violinTimeIndex = 0;
    int celloTimeIndex = 0;

    bool fakeEvent = false;
    bool fakeCelloEvent = false;
    
    vector<ofPolyline> windowsOutlines;
    unsigned int violinLineIndex = 0;
    ofPolyline currentViolinNote;
    ofPolyline violinLine;
    
    unsigned int celloLineIndex = 0;
    ofPolyline currentCelloNote;
    ofPolyline celloLine;
    
    bool emitParticles;
    ofxParticleSystem particleSystem;
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    vector<int> midiToRowMapping;
    
    vector<Wave> waves;
    
    BlurEffect blur;
    
    float celloEnergy = 0;
    float violinEnergy = 0;
    float energyThreshold = 0.01;
    kinectHand leftHand;
    kinectHand rightHand;
};


#endif //PEDRERAMASTER_XBSCENE1_H
