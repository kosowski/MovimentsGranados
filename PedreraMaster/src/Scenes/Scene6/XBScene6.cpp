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
static const int THANKS_NUM_SECONDS = 6;


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
    fbo.begin();
    {
        if (showFacadeImage) templateImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        else ofBackground(0);

        switch (state) {
            case S6_1_INITIAL:      drawS6_1(); break;
            case S6_2_DETECTED:     drawS6_2(); break;
            case S6_3_LIVE:         drawS6_3(); break;
            case S6_4_THANKS:       drawS6_4(); break;
            default:                break;
        }
    }
    fbo.end();
}

#pragma mark - 1 DETECTING

void XBScene6::updateS6_1()
{
}

void XBScene6::drawS6_1()
{
    XBScene6GUI *myGUI = (XBScene6GUI *) gui;
    drawText(S1_TITLE, fontMsgBold, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
    drawText(S1_SUBTITLE, fontMsgNormal, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);

    drawFadeRectangle();
}

#pragma mark - 2 COUNTDOWN

void XBScene6::updateS6_2()
{
    state2CountdownElapsedTime = ofGetElapsedTimef() - state2CountdownStartTime;

    if (state2CountdownElapsedTime > COUNTDOWN_NUM_SECONDS)
        goToState(S6_3_LIVE);
}

void XBScene6::drawS6_2()
{
    int countdownNumber = COUNTDOWN_NUM_SECONDS - int(state2CountdownElapsedTime);

    XBScene6GUI *myGUI = (XBScene6GUI *) gui;
    drawText(S2_TITLE, fontMsgBold, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
    drawText(S2_SUBTITLE, fontMsgNormal, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);

    drawText(ofToString(countdownNumber), fontCountdown, myGUI->countdownX, myGUI->countdownY, myGUI->countdownScale, ofColor::green);

    drawFadeRectangle();
}

#pragma mark - 3 LIVE

void XBScene6::updateS6_3()
{
    XBScene6GUI *myGUI = (XBScene6GUI *) gui;

    if (state3IsDetecting) return; // Avoid countdown while detecting person.

    state3ElapsedTime = ofGetElapsedTimef() - state3StartTime;

    if (state3ElapsedTime > myGUI->interactionMaxTime * 60)
        goToState(S6_4_THANKS);
}

void XBScene6::drawS6_3()
{
    XBScene6GUI *myGUI = (XBScene6GUI *) gui;
    if (state3IsDetecting)
    {
        drawText(S3_TITLE, fontMsgNormal, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
        drawText(S3_SUBTITLE, fontMsgNormal, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);
    }

    drawFadeRectangle();
}

#pragma mark - 4 THANKS

void XBScene6::updateS6_4()
{
    state4ElapsedTime = ofGetElapsedTimef() - state4StartTime;

    if (state4ElapsedTime > THANKS_NUM_SECONDS)
        goToState(S6_1_INITIAL);
}

void XBScene6::drawS6_4()
{
    XBScene6GUI *myGUI = (XBScene6GUI *) gui;
    drawText(S4_TITLE, fontMsgNormal, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);

    drawFadeRectangle();
}

#pragma mark - Kinect / key events

void XBScene6::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    switch (key)
    {
        case ' ': goToNextState(); break;
#ifdef OF_DEBUG
        case '.': state3IsDetecting = !state3IsDetecting; break;
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
                state3IsDetecting = true;
            else if (kState == OSC_KINECT_STATE_CAPTURING)
            {
                state3IsDetecting = false;
                state3StartTime = ofGetElapsedTimef();
            }
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
            state2CountdownStartTime = ofGetElapsedTimef();
            break;
        }
        case S6_3_LIVE:
        {
            state3IsDetecting = false;
            state3StartTime = ofGetElapsedTimef();
            break;
        }
        case S6_4_THANKS:
        {
            state4StartTime = ofGetElapsedTimef();
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

#pragma mark - Convenience methods

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
