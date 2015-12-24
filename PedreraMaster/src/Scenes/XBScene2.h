//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE2_H
#define PEDRERAMASTER_XBSCENE2_H

#include "XBBaseScene.h"

class XBScene2 : public XBBaseScene
{
public:

    XBScene2(const string &name) : XBBaseScene(name) {}

    virtual void setup();
    virtual void update();
    virtual void drawIntoFBO();
};


#endif //PEDRERAMASTER_XBSCENE2_H
