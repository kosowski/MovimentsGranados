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

    creaturesGroup.setup("ViolinAndCello");
    creaturesGroup.add(size.setup("Size", 10.0, 1.0, 20));
    creaturesGroup.add(maxSpeed.setup("Speed", 1.0, 0.0, 10));
    creaturesGroup.add(maxForce.setup("Force", 0.10, 0.00, 1.00));
    creaturesGroup.add(pathSpeed.setup("Path increment", 1.0, 0.0, 5.0));
    creaturesGroup.add(stiffness.setup("stiffness", 0.05, 0.00, 0.50));
    creaturesGroup.add(damping.setup("damping", 0.75, 0.00, 1.00));
    creaturesGroup.add(mass.setup("mass", 6., 0.0, 10.0));
    creaturesGroup.add(glowAmount.setup("Glow amount", 0.9, 0.0, 1.0));
    creaturesGroup.add(glowRadius.setup("Glow radius", 10, 0, 20));
    creaturesGroup.add(linkAudio.setup("Alpha audio", false));
    creaturesGroup.add(showPath.setup("Show path", false));

    sceneGui.add(&creaturesGroup);
    
    particlesGroup.setup("Particles");
    particlesGroup.add(particleSize.setup("Particle size", 10, 2, 40));
    particlesGroup.add(particleLife.setup("Particle life", 60, 1, 200));
    particlesGroup.add(maxDistance.setup("Max distance", 400, 1, 2000));
    particlesGroup.add(maxParticles.setup("Max particles", 1, 0, 10));
    particlesGroup.add(particleVelocity.setup("Particle velocity", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(150,150)));
    particlesGroup.add(particleSpread.setup("Particle spread", ofVec2f(10,0), ofVec2f(0,0), ofVec2f(150,150)));
    sceneGui.add(&particlesGroup);
    
    pianoGroup.setup("Piano");
    pianoGroup.add(stoneGrowFactor.setup("Amplitude", 0.8, -1.0, 1.));
    pianoGroup.add(stoneDamping.setup("Damping", 0.98, 0.4, 1.00));
    pianoGroup.add(stoneAlphaDecrease.setup("Alpha speed", 2.3, 1, 20));
    pianoGroup.add(stoneTime.setup("Duration", 2, 0.0, 5.0));
    pianoGroup.add(stoneFrequency.setup("Pulsation freq", .05, 0.0, .5));
    pianoGroup.add(stonePhase.setup("Initial phase", 3, 0, 3));
    sceneGui.add(&pianoGroup);
    
    directorGroup.setup("Director");
    directorGroup.add(lineWidth.setup("Line width", 1.0f, 1.0f, 40.0f));
    directorGroup.add(minPeriod.setup("Min wave period", 800.0f, 200.0f, 2000.0f));
    directorGroup.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));
    
    directorGroup.add(attractorStrength.setup("attractorStrength",  5, 0.0, 10.0));
    directorGroup.add(attractorRadius.setup("attractorRadius", 10.0f, 0.00, 14.00));
    sceneGui.add(&directorGroup);

    sceneGui.add(blurAmount.setup("Blur amount", 1., 0, 8.));
//    sceneGui.add(showTemplate.setup("Show template", false));

    loadSettings();
}
