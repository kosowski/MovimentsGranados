//
// Created by Miquel Àngel Soler on 27/1/16.
//

#ifndef PEDRERAMASTER_XBSCENE5_H
#define PEDRERAMASTER_XBSCENE5_H

#include "XBBaseScene.h"


class XBScene5 : public XBBaseScene
{
public:

    XBScene5(const string &name) : XBBaseScene(name) {}

    virtual void setup(XBBaseGUI *gui);
    virtual void update();
    virtual void drawIntoFBO();

private:
};


#endif //PEDRERAMASTER_XBSCENE5_H
