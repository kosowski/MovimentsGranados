//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene3.h"
#include "XBScene3GUI.h"

void XBScene3::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    int w = 200;
    int h = 250;
    
    // creatures init
    v.setup(ofGetWidth() / 2, ofGetHeight() / 2);
    x.setup(ofGetWidth() / 2, ofGetHeight() / 2);

    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    //particles init
    emitParticles = false;
    vEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    vEmitter.setVelocity(myGUI->particleVelocity);
    vEmitter.velSpread = myGUI->particleSpread;
    vEmitter.life = myGUI->particleLife;
    vEmitter.lifeSpread = 5.0;
    vEmitter.numPars = 1;
    vEmitter.color = ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
    vEmitter.size = myGUI->particleSize;
    
    xEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    xEmitter.setVelocity(myGUI->particleVelocity);
    xEmitter.velSpread = myGUI->particleSpread;
    xEmitter.life = myGUI->particleLife;
    xEmitter.lifeSpread = 5.0;
    xEmitter.numPars = 1;
    xEmitter.color = ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
    xEmitter.size = myGUI->particleSize;
    
    ofLoadImage(pTex, "resources/particle.png");

    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0 );
}

void XBScene3::update()
{
    XBBaseScene::update();

    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    // update violin and cello positions and colours
    v.update();
    x.update();
    v.setColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));
    x.setColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));
    
    //update emitters
    if(emitParticles){
        //GUI related
        vEmitter.size = myGUI->particleSize;
        vEmitter.setVelocity(myGUI->particleVelocity);
        vEmitter.velSpread = myGUI->particleSpread;
        vEmitter.life = myGUI->particleLife;
        vEmitter.color.set(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));

        xEmitter.size = myGUI->particleSize;
        xEmitter.setVelocity(myGUI->particleVelocity);
        xEmitter.velSpread = myGUI->particleSpread;
        xEmitter.life = myGUI->particleLife;
        xEmitter.color.set(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));
        
        //update emitters following creatures
        vEmitter.setPosition(v.getLocation());
        particleSystem.addParticles(vEmitter);
        
        xEmitter.setPosition(x.getLocation());
        particleSystem.addParticles(xEmitter);
        
        // update particle system
        float dt = min(ofGetLastFrameTime(), 1.0/10.0);
        particleSystem.update(dt, 1.);
    }
    
}

void XBScene3::drawIntoFBO()
{
    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    fbo.begin();
    {
        ofBackground(0);
        
        if(emitParticles){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            particleSystem.draw(pTex);
        }
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        v.draw();
        x.draw();
        
        drawGUI();
        
        drawFadeRectangle();
    }
    fbo.end();
    
    blur.apply(&fbo, myGUI->blurAmount, 1);

}

void XBScene3::drawGUI()
{
    XBBaseScene::drawGUI();
}

void XBScene3::keyReleased(int key){
    XBBaseScene::keyReleased(key);
    if(key == 'p'){
        emitParticles = !emitParticles;
    }
}

