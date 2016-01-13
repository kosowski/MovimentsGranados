//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene1GUI.h"

XBScene1GUI::XBScene1GUI() : XBBaseGUI()
{
    gui.setup();
    gui.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
    gui.add(springDamping.setup("damping", 0.0, 0.000, 0.201));
    gui.add(restLength.setup("restLength", 0., -10., 20.));

    gui.add(drag.setup("drag", 0.1, 0.01, 1.00));
    gui.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
    gui.add(particleMass.setup("particleMass", 0.2, 0., 1.00));
    gui.add(mouseStrength.setup("mouseStrength", -100000, -6000, -200000));
    gui.add(mouseSlope.setup("mouseSlope", 70, 60., 260));
    gui.add(fixedStrength.setup("fixedStrength", 0.06, 0.01, 0.50));
    gui.add(fixedDamping.setup("fixedDamping",  0.00, 0.000, 0.201));
    gui.add(fixedRestLength.setup("fixedRestLength", 0., 0., 40.));

    gui.add(xDamping.setup("xDamping", 1., 0.00, 1.00));
}

void XBScene1GUI::draw()
{
    gui.draw();
}
