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

    directorGroup.setup("Director");
    directorGroup.add(flashDirector.setup("Flash director", false));
    directorGroup.add(flashTime.setup("Flash time", .4f, .2f, 1.0f));
    directorGroup.add(enableDirector.setup("Enable director", true));
    directorGroup.add(lineWidth.setup("Line width", 1.0f, 1.0f, 5.0f));
    directorGroup.add(minPeriod.setup("Min wave period", 800.0f, 200.0f, 2000.0f));
    directorGroup.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));
    directorGroup.add(minAttractorStrength.setup("Min strength",  0.5, 0.0, 10.0));
    directorGroup.add(attractorStrength.setup("Max strength",  4, 0.0, 10.0));
    directorGroup.add(attractorRadius.setup("attractorRadius", 10.0f, 0.00, 14.00));
    directorGroup.add(dampingWaves.setup("Damping",  0.80, 0.00, 1.00));
    directorGroup.add(maskRadius.setup("Mask radius", 300, 10, 600.));
    directorGroup.add(maskAlpha.setup("Mask decay", 10, 0, 15));
    directorGroup.add(maskLevel.setup("Mask level", 10, 0, 255));
    sceneGui.add(&directorGroup);
    
    pianoGroup.setup("Piano");
    pianoGroup.add(stoneGrowFactor.setup("Amplitude", 0.01, 0.000, 1.));
    pianoGroup.add(stoneDamping.setup("Damping", 1., 0.4, 1.00));
    pianoGroup.add(stoneAlphaDecrease.setup("Alpha speed", 2.3, 1, 20));
    pianoGroup.add(stoneTime.setup("Duration", 3.5, 0.0, 5.0));
    pianoGroup.add(stoneFrequency.setup("Pulsation freq", .05, 0.0, .5));
    sceneGui.add(&pianoGroup);
    
    violinGroup.setup("Violin and Cello");
    violinGroup.add(celloVertical.setup("Cello vertical", false));
    violinGroup.add(particleSize.setup("Particle size", 10, 2, 40));
    violinGroup.add(particleLife.setup("Particle life", 3, 1, 60));
    violinGroup.add(numParticles.setup("Particle  number", 10, 1, 20));
    violinGroup.add(headSize.setup("Head size", 20, 10, 80));
    violinGroup.add(windowAttack.setup("Window attack", 0.01, 0.00, 0.05));
    violinGroup.add(windowFade.setup("Window fade speed", 10, 1, 20));
    violinGroup.add(drawWindows.setup("draw windows", false));
     violinGroup.add(windowOutlineWidth.setup("Window width", 12, 4, 20));
    violinGroup.add(timeIncrement.setup("Marker speed", 1, 1, 30));
    violinGroup.add(showTimeMarker.setup("Show time line", false));
    violinGroup.add(particleVelocity.setup("Particle velocity", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(150,150)));
    violinGroup.add(particleSpread.setup("Particle spread", 5, 0, 30));
    sceneGui.add(&violinGroup);
    
    sceneGui.add(blurAmount.setup("Blur amount", 1., 0, 8.));
    
    loadSettings();
}
