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
    const ofFbo &getMainFBO() const { return fbo; }

    void setFBOAlpha(float _fboAlpha) { fboAlpha = _fboAlpha; };
    float *getFBOAlpha() { return &fboAlpha; }

protected:

    void drawFadeRectangle();

    ofFbo fbo;
    float fboAlpha;

private:

    string name;
};


#endif //PEDRERAMASTER_XBBASESCENE_H
