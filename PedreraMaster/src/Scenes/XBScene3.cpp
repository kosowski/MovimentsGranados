//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene3.h"

void XBScene3::setup()
{
    XBBaseScene::setup();
}

void XBScene3::update()
{
    XBBaseScene::update();
}

void XBScene3::drawIntoFBO()
{
    mainFBO.begin();
    {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(ofColor::darkBlue);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    mainFBO.end();
}
