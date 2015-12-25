//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBBaseScene.h"

XBBaseScene::XBBaseScene(string _name)
{
    name = _name;

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    fboAlpha = 0.0f;
}

void XBBaseScene::setup()
{
    fbo.begin();
    {
        // Often the FBO will contain artifacts from the memory that the graphics card has just allocated for it,
        // so it's good to clear it before starting to draw it
        ofClear(0, 0, 0, 0);
    }
    fbo.end();
}

void XBBaseScene::update()
{
//    cout << name << " alpha: " << fboAlpha << endl;
}

void XBBaseScene::drawFadeRectangle()
{
    ofSetColor(0, 0, 0, (int)fboAlpha);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
}
