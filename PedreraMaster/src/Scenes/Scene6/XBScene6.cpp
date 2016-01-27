//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene6.h"

void XBScene6::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
}

void XBScene6::update()
{
    XBBaseScene::update();
}

void XBScene6::drawIntoFBO()
{
    fbo.begin();
    {

    }
    fbo.end();
}
