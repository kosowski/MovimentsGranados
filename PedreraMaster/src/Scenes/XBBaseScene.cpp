//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBBaseScene.h"

XBBaseScene::XBBaseScene(string _name)
{
    mainFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
    name = _name;
}

void XBBaseScene::setup()
{
    mainFBO.begin();
    {
        // Often the FBO will contain artifacts from the memory that the graphics card has just allocated for it,
        // so it's good to clear it before starting to draw it
        ofClear(0, 0, 0, 0);
    }
    mainFBO.end();
}

void XBBaseScene::update()
{
}
