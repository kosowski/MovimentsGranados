//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBBaseScene.h"
#include "XBOSCManager.h"

XBBaseScene::XBBaseScene(string _name)
{
    name = _name;

    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
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

    fboAlpha = 255.0f;

    XBOSCManager::getInstance().subscribeToCelloEvents(this);
}

void XBBaseScene::update()
{
//    cout << name << " alpha: " << fboAlpha << endl;
}

void XBBaseScene::drawFadeRectangle()
{
    if (int(fboAlpha) == 255) return;

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
    ofClear(0, 0, 0, (int)fboAlpha);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}
