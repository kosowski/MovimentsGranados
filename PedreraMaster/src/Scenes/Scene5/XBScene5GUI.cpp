//
// Created by Miquel Àngel Soler on 27/1/16.
//

#include "XBScene5GUI.h"

XBScene5GUI::XBScene5GUI()
{
    sceneSettingsFilename = "scene5.xml";
    colorSettingsFilename = "colors5.xml";
}

void XBScene5GUI::setup()
{
    XBBaseGUI::setup();

    loadSettings();
}
