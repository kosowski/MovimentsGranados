//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene2GUI.h"
#include "XBScene2.h"

XBScene2GUI::XBScene2GUI(XBScene2 *_scene) : XBBaseGUI(_scene)
{
    scene = _scene;

    gui.setup();

    gui.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
    gui.add(springDamping.setup("damping", 0.01, 0.000, 0.201));
    gui.add(drag.setup("drag", 0.1, 0.01, 1.00));
    gui.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
    gui.add(particleMass.setup("particleMass", 0.2, 0., 1.00));
    gui.add(mouseStrength.setup("mouseStrength", -96000, -6000, -200000));
    gui.add(mouseSlope.setup("mouseSlope", 160, 60., 260));
    gui.add(fixedStrength.setup("fixedStrength", 0.03, 0.01, 0.50));
    gui.add(fixedDamping.setup("fixedDamping", 0.01, 0.000, 0.201));
}

void XBScene2GUI::draw()
{
    gui.draw();
}
