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
    void onCelloPitchChanged(float &pitch);
    void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn);

    void initWaves();
    void initLines();
    void initParticles();
    void initStones();
    void initWindows();
    void updateEmitters();
    
private:
    void updateViolin();
    void updateCello();
    void updatePiano();
    void updateDirector();
    
    void drawDirector();
    void drawPiano();
    void drawViolinCello();
    void drawWindows();
    
    void addFadingWindow(int index, vector<expandingPolyLine> &vector, float initValue, float instrumentAlpha);
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
    
    int celloInsideWindow = -1;
    int violinInsideWindow = -1;
    vector<expandingPolyLine> fadingViolinWindowsToDraw;
    vector<expandingPolyLine> fadingCelloWindowsToDraw;
    
    bool emitParticles;
    ofxParticleSystem particleSystem;
    ofxParticleEmitter vEmitter, xEmitter;
    ofTexture pTex;
    ofTexture headTexture;
    
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    vector<int> midiToRowMapping;
    
    vector<Wave> waves;
    ofFbo wavesMask;
    
    BlurEffect blur;
};


#endif //PEDRERAMASTER_XBSCENE1_H
