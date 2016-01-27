//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene2GUI.h"

XBScene2GUI::XBScene2GUI() : XBBaseGUI()
{
    sceneSettingsFilename = "scene2.xml";
    colorSettingsFilename = "colors2.xml";
}

void XBScene2GUI::setup()
{
    XBBaseGUI::setup();

    directorGroup.setup("Director");
    directorGroup.add(lineWidth.setup("Line width", 1.0f, 1.0f, 40.0f));
    directorGroup.add(minPeriod.setup("Min wave period", 800.0f, 200.0f, 2000.0f));
    directorGroup.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));
    directorGroup.add(attractorStrength.setup("attractorStrength",  4, 0.0, 10.0));
    directorGroup.add(attractorRadius.setup("attractorRadius", 10.0f, 0.00, 14.00));
    sceneGui.add(&directorGroup);
    
    loadSettings();
}
