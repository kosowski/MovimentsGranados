//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene3GUI.h"
#include "XBScene3.h"

XBScene3GUI::XBScene3GUI(XBScene3 *_scene) : XBBaseGUI(_scene)
{
    scene = _scene;
}

void XBScene3GUI::draw()
{
    gui.draw();
}
