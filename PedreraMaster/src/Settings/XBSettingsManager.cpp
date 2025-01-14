//
// Created by Miquel Àngel Soler on 2/2/16.
//

#include "XBSettingsManager.h"
#include "ofxXmlSettings.h"

const static string STR_TAG_MAIN            = "AppSettings";
const static string STR_TAG_WINDOWSCALE     = "WindowScale";
const static string STR_TAG_FADEOUTTIME     = "FadeOutTime";
const static string STR_TAG_FADEINTIME      = "FadeInTime";


bool XBSettingsManager::loadFile(string filename)
{
    ofxXmlSettings settings;

    bool loaded = settings.loadFile(filename);
    if (loaded)
    {
        // Check XML validity
        {
            loaded = settings.tagExists(STR_TAG_MAIN);
            loaded = loaded &&
                    settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_WINDOWSCALE) &&
                    settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_FADEOUTTIME) &&
                    settings.tagExists(STR_TAG_MAIN + ":" + STR_TAG_FADEINTIME);

            if (!loaded)
            {
                createSettingsFile(filename);
                loadFile(filename);
            }
        }

        windowScale = float(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_WINDOWSCALE, 0.0));
        fadeOutTime = float(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_FADEOUTTIME, 2.0));
        fadeInTime = float(settings.getValue(STR_TAG_MAIN + ":" + STR_TAG_FADEINTIME, 2.0));
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
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_WINDOWSCALE, 0.7);
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_FADEOUTTIME, 2.0);
    settings.setValue(STR_TAG_MAIN + ":" + STR_TAG_FADEINTIME, 2.0);
    settings.saveFile(filename);
}
