//
// Created by nacho cossio on 11/01/16.
//

#ifndef PEDRERAMASTER_XBSCENE4_H
#define PEDRERAMASTER_XBSCENE4_H

#include "XBBaseScene.h"
#include "Wave.h"
#include "BlurEffect.h"
#include "ofxMaskedGrayScott.h"

class XBScene4 : public XBBaseScene
{
public:
    
    XBScene4(const string &name) : XBBaseScene(name) {}
    
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

private:

    void initWindows(string name,  vector<ofRectangle>& vectorWindows, int starIndex, int floor);
    void arrangeWindows(int index, vector<ofRectangle>& elements);
    void initWindowsOutlines(string name, vector<expandingPolyLine> &vectorWindows);
    int drawWindow(float note, vector<ofRectangle>& windows);
    void initReactionDiffusion();
    void initWaves();
    void initStones();
    
    bool fakeCelloEvent = false;
    float celloNote;
    bool fakePianoEvent = false;
    float pianoNote;
    bool fakeViolinEvent = false;
    float violinNote;
    
    ofFbo auxFbo;
    ofImage windowMask;
    ofImage rectMask;
    ofxSVG svg;
    vector<int> midiToRowMapping;
    vector<vector<expandingPolyLine>> stones;
    vector<expandingPolyLine> stonesToDraw;
    
    vector<Wave> waves;
    
    vector<ofRectangle> celloWindows;
    vector<ofRectangle> violinWindows;
    
    vector<expandingPolyLine> celloOutlines;
    vector<expandingPolyLine> celloOutlinesToDraw;
    vector<expandingPolyLine> violinOutlines;
    vector<expandingPolyLine> violinOutlinesToDraw;
    
    ofxMaskedGrayScott grayV;
    ofxMaskedGrayScott grayX;
    ofShader convertToBW;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE4_H
