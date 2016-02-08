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

    string getOSCHost() { return oscHost; }

private:

    void createSettingsFile(string filename);

    string oscHost;
};


#endif //PEDRERAMASTER_XVSETTINGSMANAGER_H
