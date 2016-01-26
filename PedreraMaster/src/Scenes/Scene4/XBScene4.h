//
// Created by nacho cossio on 11/01/16.
//

#ifndef PEDRERAMASTER_XBSCENE4_H
#define PEDRERAMASTER_XBSCENE4_H

#include "XBBaseScene.h"
#include "ofxGui.h"
#include "Wave.h"
#include "BlurEffect.h"
#include "ofxMaskedGrayScott.h"
#include "ofxSvg.h"

class XBScene4 : public XBBaseScene
{
public:

    XBScene4(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();
    void keyReleased(int key);
    void onViolinPitchChanged(float &pitch);
    void onViolinEnergyChanged(float &energy);
    void onCelloPitchChanged(float &pitch);
    void onCelloEnergyChanged(float &energy);
    void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn);
    void onPianoNoteOff(int &noteOff);
    
    void initReactionDiffusion();
    void initWaves();
    void initSVG();

    
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

    ofxSVG svg;
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    
    vector<Wave> waves;
    
    ofxMaskedGrayScott gray;
    ofShader convertToBW;

    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE4_H
