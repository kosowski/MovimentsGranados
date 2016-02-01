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

    loadSettings();
}
