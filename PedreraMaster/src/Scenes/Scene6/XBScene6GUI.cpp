//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene6GUI.h"

XBScene6GUI::XBScene6GUI()
{
    sceneSettingsFilename = "scene6.xml";
    colorSettingsFilename = "colors6.xml";
}

void XBScene6GUI::setup()
{
    XBBaseGUI::setup();

    kinectGroup.setup("Kinect Control");
    kinectGroup.add(kinectMode.setup("Automatic", false));
    sceneGui.add(&kinectGroup);

    s3Interaction.setup("Interaction");
    s3Interaction.add(interactionMaxTime.setup("Max Time", 1, 1, 5));
    sceneGui.add(&s3Interaction);

    titlesGroup.setup("Titles");
    titlesGroup.add(titleX.setup("Title X", 0.5f, 0.0f, 1.0f));
    titlesGroup.add(titleY.setup("Title Y", 0.47f, 0.0f, 1.0f));
    titlesGroup.add(titleScale.setup("Title Scale", 0.5f, 0.1f, 1.0f));
    sceneGui.add(&titlesGroup);

    subtitlesGroup.setup("Subtitles");
    subtitlesGroup.add(subtitleX.setup("Subtitle X", 0.5f, 0.0f, 1.0f));
    subtitlesGroup.add(subtitleY.setup("Subtitle Y", 0.53f, 0.0f, 1.0f));
    subtitlesGroup.add(subtitleScale.setup("Subtitle Scale", 0.25f, 0.1f, 1.0f));
    sceneGui.add(&subtitlesGroup);

    countdownGroup.setup("Countdown");
    countdownGroup.add(countdownX.setup("Countdown X", 0.5f, 0.0f, 1.0f));
    countdownGroup.add(countdownY.setup("Countdown Y", 0.7f, 0.0f, 1.0f));
    countdownGroup.add(countdownScale.setup("Countdown Scale", 1.0f, 0.1f, 1.0f));
    sceneGui.add(&countdownGroup);

    directorGroup.setup("Director");
    directorGroup.add(lineWidth.setup("Line width", 1.0f, 1.0f, 40.0f));
    directorGroup.add(minPeriod.setup("Min wave period", 800.0f, 200.0f, 2000.0f));
    directorGroup.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));
    directorGroup.add(attractorStrength.setup("attractorStrength",  4, 0.0, 10.0));
    directorGroup.add(attractorRadius.setup("attractorRadius", 10.0f, 0.00, 14.00));
    directorGroup.add(dampingWaves.setup("Damping",  0.80, 0.00, 1.00));
    directorGroup.add(maskRadius.setup("Mask radius", 300, 10, 600.));
    directorGroup.add(maskAlpha.setup("Mask decay", 10, 0, 120));
    sceneGui.add(&directorGroup);
    
    sceneGui.add(blurAmount.setup("Blur amount", 1., 0, 8.));
    
    loadSettings();
}
