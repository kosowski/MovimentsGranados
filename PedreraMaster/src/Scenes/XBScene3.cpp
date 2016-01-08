//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene3.h"

void XBScene3::setup()
{
    XBBaseScene::setup();
    v.setup(ofGetWidth() / 2, ofGetHeight() / 2);
    x.setup(ofGetWidth() / 2, ofGetHeight() / 2);
}

void XBScene3::update()
{
    XBBaseScene::update();
    
    v.update();
    x.update();
}

void XBScene3::drawIntoFBO()
{
    fbo.begin();
    {
        ofBackground(0);
        v.draw();
        x.draw();

        drawFadeRectangle();
    }
    fbo.end();
}
