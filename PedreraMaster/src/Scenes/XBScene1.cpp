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
    mainFBO.begin();
    {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(ofColor::darkRed);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    mainFBO.end();
}
