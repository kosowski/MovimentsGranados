//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene4GUI.h"

XBScene4GUI::XBScene4GUI() : XBBaseGUI()
{
    gui.setup();
    gui.add(minPeriod.setup("Min wave period", 800., 200., 2000.));
    gui.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));

    gui.add(attractorStrength.setup("attractorStrength",  3, 0.0, 10.0));
    gui.add(attractorRadius.setup("attractorRadius", 3., 0.00, 14.00));
}

void XBScene4GUI::draw()
{
    gui.draw();
}
