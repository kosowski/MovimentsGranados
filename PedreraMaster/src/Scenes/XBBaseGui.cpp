//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBBaseGUI.h"

XBBaseGUI::XBBaseGUI(XBBaseScene *_scene)
{
    scene = _scene;
}

void XBBaseGUI::update()
{
    for (int i=0; i<colorPickers.size(); ++i)
        colorPickers[i].update();
}

void XBBaseGUI::draw()
{
    for (int i=0; i<colorPickers.size(); ++i)
        colorPickers[i].draw();
}
