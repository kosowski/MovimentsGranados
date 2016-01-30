//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene6.h"
#include "XBScene6GUI.h"
#include "../../Shared/OSCSettings.h"


static const string STR_FONT_PATH       = "resources/fonts/";
static const string STR_FONTFILE_BOLD   = "NeutraText-Demi.otf";
static const string STR_FONTFILE_NORMAL = "NeutraText-Book.otf";

static const int MAX_FONT_SIZE = 75;

static const string S1_TITLE    = "MOU LA PEDRERA!";
static const string S1_SUBTITLE = "Situa't sobre la marca i aixeca les mans per a ser detectat.";
static const string S2_TITLE    = "DETECTAT!";
static const string S2_SUBTITLE = "Mou les mans per a crear onades.";
static const string S3_TITLE    = "PERSONA NO DETECTADA";
static const string S3_SUBTITLE = S1_SUBTITLE;
static const string S4_TITLE    = "Gràcies per participar!";

static const int COUNTDOWN_NUM_SECONDS = 5;


XBScene6::XBScene6(const string &name) : XBBaseScene(name)
{
    string fontPath = STR_FONT_PATH + STR_FONTFILE_BOLD;

    fontMsgBold = new ofTrueTypeFont();
    fontMsgBold->load(fontPath, MAX_FONT_SIZE, true, true, true);

    fontPath = STR_FONT_PATH + STR_FONTFILE_NORMAL;

    fontMsgNormal = new ofTrueTypeFont();
    fontMsgNormal->load(fontPath, MAX_FONT_SIZE, true, true, true);

    fontCountdown = new ofTrueTypeFont();
    fontCountdown->load(fontPath, MAX_FONT_SIZE, true, true, true);
}

XBScene6::~XBScene6()
{
    delete fontCountdown;
    delete fontMsgNormal;
    delete fontMsgBold;
}

void XBScene6::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
}

void XBScene6::enteredScene()
{
    state = S6_1_INITIAL;
}

void XBScene6::update()
{
    XBBaseScene::update();
    switch (state) {
        case S6_1_INITIAL:    updateS6_1(); break;
        case S6_2_DETECTED:    updateS6_2(); break;
        case S6_3_LIVE:         updateS6_3(); break;
        case S6_4_THANKS:       updateS6_4(); break;
        default:                break;
    }
}

void XBScene6::drawIntoFBO()
{
//    ofClear(0);

    switch (state) {
        case S6_1_INITIAL:    drawS6_1(); break;
        case S6_2_DETECTED:    drawS6_2(); break;
        case S6_3_LIVE:         drawS6_3(); break;
        case S6_4_THANKS:       drawS6_4(); break;
        default:                break;
    }
}

#pragma mark - 1 DETECTING

void XBScene6::updateS6_1()
{
}

void XBScene6::drawS6_1()
{
    fbo.begin();
    {
        ofBackground(0,0,0,255);

        XBScene6GUI *myGUI = (XBScene6GUI *) gui;
        drawText(S1_TITLE, fontMsgBold, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
        drawText(S1_SUBTITLE, fontMsgNormal, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);

        drawFadeRectangle();
    }
    fbo.end();
}

#pragma mark - 2 COUNTDOWN

void XBScene6::updateS6_2()
{
    countdownElapsedTime = ofGetElapsedTimef() - countdownStartTime;

    if (countdownElapsedTime > COUNTDOWN_NUM_SECONDS)
        goToState(S6_3_LIVE);
}

void XBScene6::drawS6_2()
{
    int countdownNumber = COUNTDOWN_NUM_SECONDS - int(countdownElapsedTime);

    fbo.begin();
    {
        ofBackground(0,0,0,255);

        XBScene6GUI *myGUI = (XBScene6GUI *) gui;
        drawText(S2_TITLE, fontMsgBold, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
        drawText(S2_SUBTITLE, fontMsgNormal, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);

        drawText(ofToString(countdownNumber), fontCountdown, myGUI->countdownX, myGUI->countdownY, myGUI->countdownScale, ofColor::green);

        drawFadeRectangle();
    }
    fbo.end();
}

#pragma mark - 3 LIVE

void XBScene6::updateS6_3()
{
}

void XBScene6::drawS6_3()
{
    fbo.begin();
    {
        ofBackground(0,0,0,255);

        XBScene6GUI *myGUI = (XBScene6GUI *) gui;
        if (showUndetectedMessage)
        {
            drawText(S3_TITLE, fontMsgNormal, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
            drawText(S3_SUBTITLE, fontMsgNormal, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);
        }

        drawFadeRectangle();
    }
    fbo.end();
}

#pragma mark - 4 THANKS

void XBScene6::updateS6_4()
{
}

void XBScene6::drawS6_4()
{
    fbo.begin();
    {
        ofBackground(0,0,0,255);

        XBScene6GUI *myGUI = (XBScene6GUI *) gui;
        drawText(S4_TITLE, fontMsgNormal, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);

        drawFadeRectangle();
    }
    fbo.end();
}

void XBScene6::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    switch (key)
    {
        case ' ': goToNextState(); break;
#ifdef OF_DEBUG
        case '.': showUndetectedMessage = !showUndetectedMessage; break;
#endif
        default: break;
    }
}

void XBScene6::onKinectStateChanged(string &kState)
{
    XBScene6GUI *myGUI = (XBScene6GUI *) gui;

    switch (state)
    {
        case S6_1_INITIAL:
        {
            if ((kState == OSC_KINECT_STATE_POSITIONED) && (myGUI->kinectMode))
                goToState(S6_2_DETECTED);

            break;
        }
        case S6_2_DETECTED:
        {
            break;
        }
        case S6_3_LIVE:
        {
            if (kState == OSC_KINECT_STATE_DETECTING)
                showUndetectedMessage = true;
            else if (kState == OSC_KINECT_STATE_CAPTURING)
                showUndetectedMessage = false;
            break;
        }
        case S6_4_THANKS:
        {
            break;
        }
        default: break;
    }
}

void XBScene6::goToState(S6State newState)
{
    state = newState;

    switch(state)
    {
        case S6_2_DETECTED:
        {
            countdownStartTime = ofGetElapsedTimef();
            break;
        }
        case S6_3_LIVE:
        {
            showUndetectedMessage = false;
            break;
        }
        default: break;
    }
}

void XBScene6::goToNextState()
{
    S6State newState = S6State((state + 1) % S6_NUM_STATES);
    goToState(newState);
}

void XBScene6::drawText(string message, ofTrueTypeFont *font, float x, float y, float scaleFactor, ofColor color)
{
    ofSetColor(color);

    ofPushMatrix();
    {
        float width = font->stringWidth(message) * scaleFactor;
        float height = font->stringHeight(message) * scaleFactor;

        ofTranslate(ofPoint((ofGetWidth() * x) - width/2, (ofGetHeight() * y) - height/2));
        ofScale(scaleFactor, scaleFactor);
        fontMsgBold->drawString(message, 0, 0);
    }
    ofPopMatrix();
}
