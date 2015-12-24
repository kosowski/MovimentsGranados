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
    mainFBO.begin();
    {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(ofColor::darkGreen);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    mainFBO.end();
}
