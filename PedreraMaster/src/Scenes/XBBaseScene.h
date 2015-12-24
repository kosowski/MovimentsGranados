//
// Created by Miquel Àngel Soler on 24/12/15.
//

#ifndef PEDRERAMASTER_XBBASESCENE_H
#define PEDRERAMASTER_XBBASESCENE_H

#include "ofMain.h"

class XBBaseScene
{
public:

    XBBaseScene(string name);

    virtual void setup();
    virtual void update();
    virtual void drawIntoFBO() = 0;

    const string &getName() const { return name; }
    const ofFbo &getMainFBO() const { return mainFBO; }

protected:

    ofFbo mainFBO;

private:

    string name;
};


#endif //PEDRERAMASTER_XBBASESCENE_H
