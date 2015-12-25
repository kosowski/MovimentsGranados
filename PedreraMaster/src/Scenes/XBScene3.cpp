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
    fbo.begin();
    {
        ofSetColor(ofColor::blue);
        ofSetCircleResolution(64);
        ofDrawCircle(ofGetWidth()/2 + 100, ofGetHeight()/2, 150);

        drawFadeRectangle();
    }
    fbo.end();
}
