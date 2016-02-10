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
    directorGroup.add(lineWidth.setup("Line width", 1.0f, 1.0f, 5.0f));
    directorGroup.add(minPeriod.setup("Min wave period", 800.0f, 200.0f, 2000.0f));
    directorGroup.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));
    directorGroup.add(minAttractorStrength.setup("Min strength",  0.5, 0.0, 10.0));
    directorGroup.add(attractorStrength.setup("Max strength",  4, 0.0, 10.0));
    directorGroup.add(attractorRadius.setup("attractorRadius", 10.0f, 0.00, 14.00));
    directorGroup.add(dampingWaves.setup("Damping",  0.80, 0.00, 1.00));
    sceneGui.add(&directorGroup);
    
    pianoGroup.setup("Piano");
    pianoGroup.add(stoneGrowFactor.setup("Amplitude", 0.01, 0.000, 1.));
    pianoGroup.add(stoneDamping.setup("Damping", 1., 0.4, 1.00));
    pianoGroup.add(stoneAlphaDecrease.setup("Alpha speed", 2.3, 1, 20));
    pianoGroup.add(stoneTime.setup("Duration", 3.5, 0.0, 5.0));
    pianoGroup.add(stoneFrequency.setup("Pulsation freq", .05, 0.0, .5));
    sceneGui.add(&pianoGroup);
    
    windowGroup.setup("Window");
    windowGroup.add(barHeight.setup("Audio bar height", 30., 20., 120.));
    windowGroup.add(alphaStart.setup("Alpha attack", 0.8, 0.0f, 1.0f));
    windowGroup.add(alphaFactor.setup("Alpha decay", 18.0f, 0.0f, 30.0f));
    windowGroup.add(growFactor.setup("Grow speed", .05f, 0.0f, .1f));
    windowGroup.add(windowFrequency.setup("Pulsation frequency",20, 1, 40));
    sceneGui.add(&windowGroup);
    
    sceneGui.add(blurAmount.setup("Blur amount", 1., 0, 8.));

    loadSettings();
}