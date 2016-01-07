//
// Created by Miquel Àngel Soler on 7/1/16.
//

#include "XBMidiPortParams.h"

XBMidiPortParams::XBMidiPortParams(string _portName)
{
    portName = _portName;

    params.setName(portName);
    params.add(enabled.set("Enabled", false));
}
