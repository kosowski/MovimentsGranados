//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene5.h"

void XBScene5::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
}

void XBScene5::update()
{
    XBBaseScene::update();
}

void XBScene5::drawIntoFBO()
{
    fbo.begin();
    {
        drawFadeRectangle();
    }
    fbo.end();
}
