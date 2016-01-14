//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene3GUI.h"

XBScene3GUI::XBScene3GUI() : XBBaseGUI()
{
    sceneSettingsFilename = "scene3.xml";
    colorSettingsFilename = "colors3.xml";
}

void XBScene3GUI::setup()
{
    XBBaseGUI::setup();

    sceneGui.add(particleSize.setup("Particle size", 10, 2, 40));
    sceneGui.add(particleLife.setup("Particle life", 30, 1, 60));
    sceneGui.add(particleVelocity.setup("Particle velocity", ofVec2f(0,50), ofVec2f(0,0), ofVec2f(150,150)));
    sceneGui.add(particleSpread.setup("Particle spread", ofVec2f(80,5), ofVec2f(0,0), ofVec2f(150,150)));

    loadSettings();
}
