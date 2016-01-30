//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene6.h"
#include "XBScene6GUI.h"


static const string STR_FONT_PATH = "resources/fonts/";
static const string STR_FONTFILE = "GaramondPremrPro.otf";

static const int MAX_FONT_SIZE = 75;

static const string S1_TITLE = "MOU LA PEDRERA!";
static const string S1_SUBTITLE = "Situa't sobre la marca i aixeca les mans per a ser detectat.";
static const string S2_TITLE = "DETECTAT!";
static const string S2_SUBTITLE = "Mou les mans per a crear onades.";
static const string S3_TITLE = "PERSONA NO DETECTADA";
static const string S3_SUBTITLE = S1_SUBTITLE;
static const string s$_TITLE = "Gràcies per participar!";

XBScene6::XBScene6(const string &name) : XBBaseScene(name)
{
    string fontPath = STR_FONT_PATH + STR_FONTFILE;

    fontTitle = new ofTrueTypeFont();
    fontTitle->load(fontPath, MAX_FONT_SIZE, true, true, true);

    fontSubtitle = new ofTrueTypeFont();
    fontSubtitle->load(fontPath, MAX_FONT_SIZE, true, true, true);

    fontCountdown = new ofTrueTypeFont();
    fontCountdown->load(fontPath, MAX_FONT_SIZE, true, true, true);
}

XBScene6::~XBScene6()
{
    delete fontCountdown;
    delete fontSubtitle;
    delete fontTitle;
}

void XBScene6::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
}

void XBScene6::enteredScene()
{
    state = S6_1_DETECTING;
}

void XBScene6::update()
{
    XBBaseScene::update();
    switch (state) {
        case S6_1_DETECTING:    updateS6_1(); break;
        case S6_2_COUNTDOWN:    updateS6_2(); break;
        case S6_3_LIVE:         updateS6_3(); break;
        case S6_4_THANKS:       updateS6_4(); break;
    }
}

void XBScene6::drawIntoFBO()
{
    ofClear(0);

    switch (state) {
        case S6_1_DETECTING:    drawS6_1(); break;
        case S6_2_COUNTDOWN:    drawS6_2(); break;
        case S6_3_LIVE:         drawS6_3(); break;
        case S6_4_THANKS:       drawS6_4(); break;
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
        ofClear(0);

        XBScene6GUI *myGUI = (XBScene6GUI *) gui;
        ofSetColor(255, 255, 255, 255);

        drawText(S1_TITLE, fontTitle, myGUI->titleX, myGUI->titleY, myGUI->titleScale, ofColor::white);
        drawText(S1_SUBTITLE, fontSubtitle, myGUI->subtitleX, myGUI->subtitleY, myGUI->subtitleScale, ofColor::white);


        // Subtitle

        drawFadeRectangle();
    }
    fbo.end();
}

#pragma mark - 2 COUNTDOWN

void XBScene6::updateS6_2()
{
}

void XBScene6::drawS6_2()
{
    fbo.begin();
    {
        ofClear(0);

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
        ofClear(0);

        drawFadeRectangle();
    }
    fbo.end();
}

void XBScene6::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);
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
        fontTitle->drawString(message, 0, 0);
    }
    ofPopMatrix();

}
