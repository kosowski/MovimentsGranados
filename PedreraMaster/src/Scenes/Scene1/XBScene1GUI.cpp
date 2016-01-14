//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene1GUI.h"

XBScene1GUI::XBScene1GUI() : XBBaseGUI()
{
    sceneSettingsFilename = "scene1.xml";
    colorSettingsFilename = "colors1.xml";
}

void XBScene1GUI::setup()
{
    XBBaseGUI::setup();

    sceneGui.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
    sceneGui.add(springDamping.setup("damping", 0.0, 0.000, 0.201));
    sceneGui.add(restLength.setup("restLength", 0.0f, -10.0f, 20.0f));

    sceneGui.add(drag.setup("drag", 0.1, 0.01, 1.00));
    sceneGui.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
    sceneGui.add(particleMass.setup("particleMass", 0.2, 0.0f, 1.00));
    sceneGui.add(mouseStrength.setup("mouseStrength", -100000, -6000, -200000));
    sceneGui.add(mouseSlope.setup("mouseSlope", 70, 60.0f, 260));
    sceneGui.add(fixedStrength.setup("fixedStrength", 0.06, 0.01, 0.50));
    sceneGui.add(fixedDamping.setup("fixedDamping",  0.00, 0.000, 0.201));
    sceneGui.add(fixedRestLength.setup("fixedRestLength", 0.0f, 0.0f, 40.0f));

    sceneGui.add(xDamping.setup("xDamping", 1.0f, 0.00, 1.00));

    loadSettings();
}
