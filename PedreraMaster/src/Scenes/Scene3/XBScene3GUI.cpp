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
    
    sceneGui.add(stoneGrowFactor.setup("stone grow speed", 0.025, 0.000, 0.5));
    sceneGui.add(stoneAlphaDecrease.setup("stone alpha speed", 2.3, 1, 20));
    sceneGui.add(stoneTime.setup("stone time", 2, 0.0, 4.0));

    sceneGui.add(blurAmount.setup("Blur amount", 0., 0.00, 8.00));

    loadSettings();
}
