//
// Created by Miquel Àngel Soler on 27/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE6_H
#define PEDRERAMASTER_XBSCENE6_H

#include "XBBaseScene.h"
#include "Wave.h"
#include "SimpleWave.h"
#include "BlurEffect.h"

typedef enum
{
    S6_1_INITIAL    = 0,
    S6_2_DETECTED   = 1,
    S6_3_LIVE       = 2,
    S6_4_THANKS     = 3,
    S6_NUM_STATES   = 4
} S6State;

class XBScene6 : public XBBaseScene
{
public:

    XBScene6(const string &name);
    ~XBScene6();

    virtual void setup(XBBaseGUI *gui);
    virtual void enteredScene();
    virtual void update();
    void updateS6_1();
    void updateS6_2();
    void updateS6_3();
    void updateS6_4();
    virtual void drawIntoFBO();
    void drawS6_1();
    void drawS6_2();
    void drawS6_3();
    void drawS6_4();

    void keyReleased(int key);

private:

    S6State state;

    ofTrueTypeFont *fontMsgBold;
    ofTrueTypeFont *fontMsgNormal;
    ofTrueTypeFont *fontCountdown;

    float state2CountdownStartTime;
    float state2CountdownElapsedTime;

    bool state3IsDetecting;
    float state3StartTime;
    float state3ElapsedTime;

    float state4StartTime;
    float state4ElapsedTime;
    
    ofTexture pTex;
    vector<Wave> waves;
    ofFbo wavesMask;
    ofxSVG svg;
    BlurEffect blur;

    void onKinectStateChanged(string &kState);

    void goToState(S6State newState);
    void goToNextState();
    void sendToMaxToSound(bool sound);

    void drawText(string message, ofTrueTypeFont *font, float x, float y, float scaleFactor, ofColor color);
    void initWaves();
    void updateDirector();
    void drawDirector();
    
    ofxOscSender to_Max_Active_Sender;
};


#endif //PEDRERAMASTER_XBSCENE6_H
