//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBSCENE1_H
#define PEDRERAMASTER_XBSCENE1_H

#include "XBBaseScene.h"

class XBScene1 : public XBBaseScene
{
public:

    XBScene1(const string &name) : XBBaseScene(name) {}

    virtual void setup();
    virtual void update();
    virtual void drawIntoFBO();
};


#endif //PEDRERAMASTER_XBSCENE1_H
