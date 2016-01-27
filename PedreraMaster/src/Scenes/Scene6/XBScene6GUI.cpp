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

    loadSettings();

}
