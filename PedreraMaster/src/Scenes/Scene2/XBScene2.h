//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE2_H
#define PEDRERAMASTER_XBSCENE2_H

#include "XBBaseScene.h"
#include "ofxTraerPhysics.h"
#include "Wave.h"
#include "SimpleWave.h"
#include "BlurEffect.h"

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
    void onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn);
  
private:
    void updateViolin();
    void updateCello();
    void updatePiano();
    void updateDirector();
    
    void drawDirector();
    void drawPiano();
    void drawCello();
    void drawWindows();
    
    void initWaves();
    void initWindows(string name,  vector<ofRectangle>& vectorWindows, vector<SimpleWave>& vectorWaves,int starIndex, int floor);
    void arrangeWindows(int index, vector<ofRectangle>& elements);
    void initWindowsOutlines(string name, vector<expandingPolyLine> &vectorWindows);
    void initStones();
    
    int drawWindow(float note, vector<ofRectangle>& windows, vector<SimpleWave> &waves);
    
    ofFbo celloPianoFbo, violinFbo;
    ofImage violinMask, celloPianoMask;
    ofxSVG svg;
    vector<ofRectangle> celloWindows;
    vector<ofRectangle> violinWindows;
    vector<ofRectangle> pianoWindows;
    
    vector<SimpleWave> celloWaves;
    vector<SimpleWave> violinWaves;
    vector<SimpleWave> pianoWaves;
    
    vector<expandingPolyLine> celloOutlines;
    vector<expandingPolyLine> celloOutlinesToDraw;
    vector<expandingPolyLine> pianoOutlines;
    vector<expandingPolyLine> pianoOutlinesToDraw;
    
    vector<expandingPolyLine> stones;
    vector<expandingPolyLine> stonesToDraw;
    vector<int> midiToRowMapping;

    ofImage windowMask;
    ofImage rectMask;
    
    vector<Wave> waves;
    
    ofColor directorColor;
    
    BlurEffect blur;
    
    bool fakeCelloEvent = false;
    bool fakePianoEvent = false;
    bool fakeViolinEvent = false;
};


#endif //PEDRERAMASTER_XBSCENE2_H
