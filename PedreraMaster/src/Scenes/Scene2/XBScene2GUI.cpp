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
    
    windowGroup.setup("Window");
    windowGroup.add(alphaFactor.setup("Alpha decay", 18.0f, 0.0f, 30.0f));
    windowGroup.add(growFactor.setup("Speed", .05f, 0.0f, .5f));
    windowGroup.add(windowFrequency.setup("Frequency",8, 2, 20));
    sceneGui.add(&windowGroup);

    loadSettings();
}