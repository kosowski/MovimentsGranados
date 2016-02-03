//
// Created by Miquel Àngel Soler on 2/2/16.
//

#ifndef PEDRERAMASTER_XVSETTINGSMANAGER_H
#define PEDRERAMASTER_XVSETTINGSMANAGER_H

#include "ofMain.h"

class XBSettingsManager
{
public:

    static XBSettingsManager &getInstance() {
        static XBSettingsManager instance;
        return instance;
    }

    bool loadFile(string filename);

    float getWindowScale()          { return windowScale; }
    float getTransitionTime()       { return transitionTime; }

private:

    void createSettingsFile(string filename);

    float windowScale;
    float transitionTime;
};


#endif //PEDRERAMASTER_XVSETTINGSMANAGER_H
