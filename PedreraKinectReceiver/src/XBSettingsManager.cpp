//
// Created by Miquel Àngel Soler on 2/2/16.
//

#include "XBSettingsManager.h"
#include "ofxXmlSettings.h"

const static string STR_TAG_MAIN = "AppSettings";
const static string STR_TAG_OSCHOST = "OSCHost";

bool XBSettingsManager::loadFile(string filename)
{
    ofxXmlSettings settings;

    bool loaded = settings.loadFile(filename);
    if (loaded)
    {
        // Check XML validity
        {
            loaded = settings.tagExists(STR_TAG_MAIN);
            loaded = loaded && settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_OSCHOST);

            if (!loaded)
            {
                createSettingsFile(filename);
                loadFile(filename);
            }
        }

        oscHost = settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_OSCHOST, "localhost");
    }
    else
    {
        createSettingsFile(filename);
        loadFile(filename);
    }

    return false;
}

void XBSettingsManager::createSettingsFile(string filename)
{
    ofxXmlSettings settings;
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_OSCHOST, "localhost");
    settings.saveFile(filename);
}
