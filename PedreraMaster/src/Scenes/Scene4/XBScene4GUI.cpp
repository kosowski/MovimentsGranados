//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene4GUI.h"

XBScene4GUI::XBScene4GUI() : XBBaseGUI()
{
    sceneSettingsFilename = "scene4.xml";
    colorSettingsFilename = "colors4.xml";
}

void XBScene4GUI::setup()
{
    XBBaseGUI::setup();

    sceneGui.add(lineWidth.setup("Line width", 4., 1., 40.));
    sceneGui.add(minPeriod.setup("Min wave period", 800., 200., 2000.));
    sceneGui.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));

    sceneGui.add(attractorStrength.setup("attractorStrength",  7, 0.0, 10.0));
    sceneGui.add(attractorRadius.setup("attractorRadius", 10., 0.00, 14.00));

    sceneGui.add(blurAmount.setup("Blur amount", 0., 0.00, 8.00));
    
    sceneGui.add(alpha.setup("Alpha", 255, 0, 255));
    sceneGui.add(enableSmooth.setup("Smooth", false));

    loadSettings();
}
