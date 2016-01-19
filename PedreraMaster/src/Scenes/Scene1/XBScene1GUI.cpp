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
    directorGroup.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
    directorGroup.add(springDamping.setup("damping", 0.0, 0.000, 0.201));
    directorGroup.add(restLength.setup("restLength", 0.0f, -10.0f, 20.0f));

    sceneGui.add(drag.setup("drag", 0.1, 0.01, 1.00));
    directorGroup.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
    directorGroup.add(particleMass.setup("particleMass", 0.2, 0.0f, 1.00));
    directorGroup.add(mouseStrength.setup("mouseStrength", -100000, -6000, -200000));
    directorGroup.add(mouseSlope.setup("mouseSlope", 70, 60.0f, 260));
    directorGroup.add(fixedStrength.setup("fixedStrength", 0.06, 0.01, 0.50));
    directorGroup.add(fixedDamping.setup("fixedDamping",  0.00, 0.000, 0.201));
    directorGroup.add(fixedRestLength.setup("fixedRestLength", 0.0f, 0.0f, 40.0f));

    directorGroup.add(xDamping.setup("xDamping", 1.0f, 0.00, 1.00));
    sceneGui.add(&directorGroup);
    
    violinGroup.setup("Violin&Cello");
    violinGroup.add(violinBorderSize.setup("Line size",  2.0, 1.0, 10.0));
    violinGroup.add(ViolinBorderFade.setup("Border", 100.0f, 1.0f, 200.0f));
    violinGroup.add(showTimeMarker.setup("Show time line", false));
    sceneGui.add(&violinGroup);
    
    particlesGroup.setup("Particles");
    particlesGroup.add(particleSize.setup("Particle size", 10, 2, 40));
    particlesGroup.add(particleLife.setup("Particle life", 30, 1, 60));
    particlesGroup.add(particleVelocity.setup("Particle velocity", ofVec2f(0,0), ofVec2f(0,0), ofVec2f(150,150)));
    particlesGroup.add(particleSpread.setup("Particle spread", ofVec2f(5,5), ofVec2f(0,0), ofVec2f(150,150)));
    sceneGui.add(&particlesGroup);

    sceneGui.add(blurAmount.setup("Blur amount", 1., 0, 8.));
    
    loadSettings();
}
