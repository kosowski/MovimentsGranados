//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene1.h"

void XBScene1::setup()
{
    XBBaseScene::setup();
}

void XBScene1::update()
{
    XBBaseScene::update();
}

void XBScene1::drawIntoFBO()
{
    fbo.begin();
    {
        ofSetColor(ofColor::red);
        ofSetCircleResolution(64);
        ofDrawCircle(ofGetWidth()/2 - 100, ofGetHeight()/2, 150);

        drawFadeRectangle();
    }
    fbo.end();
}
