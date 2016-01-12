//
// Created by nacho on 09/01/16.
//

#include "XBScene4.h"
#include "XBScene4GUI.h"

void XBScene4::setup()
{
    XBBaseScene::setup();

    gui = new XBScene4GUI(this);

    guiOld.setup();
    guiOld.add(minPeriod.setup("Min wave period", 800., 200., 2000.));
    guiOld.add(maxPeriod.setup("Max wave period",  1600, 200.0, 3000.0));
    
    guiOld.add(attractorStrength.setup("attractorStrength",  3, 0.0, 10.0));
    guiOld.add(attractorRadius.setup("attractorRadius", 3., 0.00, 14.00));
    
    directorColor.set(77,125,140);
    initSystem();
}

void XBScene4::initSystem(){
    // Initialize a wave with starting point, width, amplitude, and period
    
    for(int i= 0; i < NUM_WAVES; i++)
        waves.push_back( Wave(ofPoint(20, ofGetHeight() / NUM_WAVES * i), ofGetWidth(), 20, ofRandom(minPeriod, maxPeriod)));
}

void XBScene4::update()
{
    XBBaseScene::update();
    for(int i=0; i< waves.size();i++){
        waves[i].setAttractor(ofGetMouseX(), ofGetMouseY(), attractorStrength, attractorRadius);
        waves[i].update();
    }
}

void XBScene4::drawIntoFBO()
{
    fbo.begin();
    {
        ofBackground(0);
        ofSetColor(directorColor);
        for(Wave w:waves)
            w.display();
        
        drawGUI();
        drawFadeRectangle();
    }
    fbo.end();
}

void XBScene4::keyReleased(int key){
    XBBaseScene::keyReleased(key);
    if(key == 'z'){
        waves.clear();
        initSystem();
    }
}
