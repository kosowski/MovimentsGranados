//
// Created by Miquel Àngel Soler on 12/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE3GUI_H
#define PEDRERAMASTER_XBSCENE3GUI_H

#include "XBBaseGUI.h"

class XBScene3;

class XBScene3GUI : public XBBaseGUI
{
    XBScene3GUI(XBScene3 *scene);
    void draw();
};


#endif //PEDRERAMASTER_XBSCENE3GUI_H
