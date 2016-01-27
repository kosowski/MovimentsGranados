//
// Created by Miquel Àngel Soler on 27/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE6_H
#define PEDRERAMASTER_XBSCENE6_H

#include "XBBaseScene.h"


class XBScene6 : public XBBaseScene
{
public:

    XBScene6(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();

private:

};


#endif //PEDRERAMASTER_XBSCENE6_H
