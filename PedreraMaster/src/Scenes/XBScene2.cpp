//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene2.h"

void XBScene2::setup()
{
    XBBaseScene::setup();
}

void XBScene2::update()
{
    XBBaseScene::update();
}

void XBScene2::drawIntoFBO()
{
    fbo.begin();
    {
        ofSetColor(ofColor::green);
        ofSetCircleResolution(64);
        ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, 150);

        drawFadeRectangle();
    }
    fbo.end();
}
