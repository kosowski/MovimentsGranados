//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene6.h"


static const string STR_FONT_PATH = "resources/fonts/";
static const string STR_FONTFILE_MAIN = "GaramondPremrPro.otf";
static const string STR_FONTFILE_COUNTDOWN = "GaramondPremrPro.otf";

//         letterFont->drawStringAsShapes(letter, -width/2, height/2);


/*
ofTrueTypeFont *fontMain;
ofTrueTypeFont *fontCountdown;
*/


XBScene6::XBScene6(const string &name) : XBBaseScene(name)
{

}

void XBScene6::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    state = S6_1_DETECTING;
}

void XBScene6::enteredScene()
{
    cout << "Entered S6" << endl;
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
    fbo.begin();
    {
        switch (state) {
            case S6_1_DETECTING:    drawS6_1(); break;
            case S6_2_COUNTDOWN:    drawS6_2(); break;
            case S6_3_LIVE:         drawS6_3(); break;
            case S6_4_THANKS:       drawS6_4(); break;
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
}

#pragma mark - 2 COUNTDOWN

void XBScene6::updateS6_2()
{
}

void XBScene6::drawS6_2()
{
}

#pragma mark - 3 LIVE

void XBScene6::updateS6_3()
{
}

void XBScene6::drawS6_3()
{
}

#pragma mark - 4 THANKS

void XBScene6::updateS6_4()
{
}

void XBScene6::drawS6_4()
{
}

void XBScene6::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);
}
