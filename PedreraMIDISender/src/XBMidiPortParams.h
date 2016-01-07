//
// Created by Miquel Àngel Soler on 7/1/16.
//

#ifndef PEDRERAMIDISENDER_XBMIDIPORTPARAMS_H
#define PEDRERAMIDISENDER_XBMIDIPORTPARAMS_H

#include "ofMain.h"

class XBMidiPortParams
{
public:

    XBMidiPortParams(string portName);

    const ofParameterGroup &getParams() const   { return params; }
    bool getIsEnabled()                         { return enabled; }

private:

    ofParameterGroup params;
    ofParameter<bool> enabled;

    string portName;
};


#endif //PEDRERAMIDISENDER_XBMIDIPORTPARAMS_H
