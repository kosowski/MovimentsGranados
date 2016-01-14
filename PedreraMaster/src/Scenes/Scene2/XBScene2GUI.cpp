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

    sceneGui.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
    sceneGui.add(springDamping.setup("damping", 0.01, 0.000, 0.201));
    sceneGui.add(drag.setup("drag", 0.1, 0.01, 1.00));
    sceneGui.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
    sceneGui.add(particleMass.setup("particleMass", 0.2, 0., 1.00));
    sceneGui.add(mouseStrength.setup("mouseStrength", -96000, -6000, -200000));
    sceneGui.add(mouseSlope.setup("mouseSlope", 160, 60., 260));
    sceneGui.add(fixedStrength.setup("fixedStrength", 0.03, 0.01, 0.50));
    sceneGui.add(fixedDamping.setup("fixedDamping", 0.01, 0.000, 0.201));

    loadSettings();
}
