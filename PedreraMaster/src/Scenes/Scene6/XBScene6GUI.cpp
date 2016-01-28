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

    messagesGroup.setup("Messages Position");
    messagesGroup.add(mainMsgX.setup("Message X", 0.5f, 0.0f, 1.0f));
    messagesGroup.add(mainMsgY.setup("Message Y", 0.5f, 0.0f, 1.0f));
    messagesGroup.add(countdownMsgX.setup("Countdown X", 0.5f, 0.0f, 1.0f));
    messagesGroup.add(countdownMsgY.setup("Countdown Y", 0.5f, 0.0f, 1.0f));
    sceneGui.add(&messagesGroup);

    loadSettings();
}
