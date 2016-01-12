//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene3.h"
#include "XBScene3GUI.h"

void XBScene3::setup()
{
    XBBaseScene::setup();

    gui = new XBScene3GUI(this);

    int w = 200;
    int h = 250;
    
    //GUI init
    colorPickerV.setSize(10, ofGetHeight() - h - 20, w, h);
    colorPickerX.setSize(10, ofGetHeight() - 2 * (h + 20), w, h);
    violinColor.set(72, 92, 81);
    celloColor.set(207, 87, 60);
    colorPickerV.setColor(violinColor);
    colorPickerX.setColor(celloColor);
    
    guiOld.setup();
    guiOld.add(particleSize.setup("Particle size", 10, 2, 40));
    guiOld.add(particleLife.setup("Particle life", 30, 1, 60));
    guiOld.add(particleVelocity.setup("Particle velocity", ofVec2f(0,50), ofVec2f(0,0), ofVec2f(150,150)));
    guiOld.add(particleSpread.setup("Particle spread", ofVec2f(80,5), ofVec2f(0,0), ofVec2f(150,150)));
    
    // creatures init
    v.setup(ofGetWidth() / 2, ofGetHeight() / 2);
    x.setup(ofGetWidth() / 2, ofGetHeight() / 2);
    
    //particles init
    emitParticles = false;
    vEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    vEmitter.setVelocity(particleVelocity);
    vEmitter.velSpread = particleSpread;
    vEmitter.life = particleLife;
    vEmitter.lifeSpread = 5.0;
    vEmitter.numPars = 1;
    vEmitter.color = ofColor(violinColor);
    vEmitter.size = particleSize;
    
    xEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    xEmitter.setVelocity(particleVelocity);
    xEmitter.velSpread = particleSpread;
    xEmitter.life = particleLife;
    xEmitter.lifeSpread = 5.0;
    xEmitter.numPars = 1;
    xEmitter.color = ofColor(violinColor);
    xEmitter.size = particleSize;
    
    ofLoadImage(pTex, "p.png");
    
    
}

void XBScene3::update()
{
    XBBaseScene::update();
    
    // update color pickers
    colorPickerV.update();
    colorPickerX.update();
    
    // update vion and cello positions and colours
    v.update();
    x.update();
    v.setColor(colorPickerV.getColor());
    x.setColor(colorPickerX.getColor());
    
    //update emitters
    if(emitParticles){
        //GUI related
        vEmitter.size = particleSize;
        vEmitter.setVelocity(particleVelocity);
        vEmitter.velSpread = particleSpread;
        vEmitter.life = particleLife;
        vEmitter.color.set(colorPickerV.getColor());

        xEmitter.size = particleSize;
        xEmitter.setVelocity(particleVelocity);
        xEmitter.velSpread = particleSpread;
        xEmitter.life = particleLife;
        xEmitter.color.set(colorPickerX.getColor());
        
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
}

void XBScene3::drawGUI()
{
    XBBaseScene::drawGUI();
    
    if (showGUI) {
        colorPickerV.draw();
        colorPickerX.draw();
    }
}

void XBScene3::keyReleased(int key){
    XBBaseScene::keyReleased(key);
    if(key == 'p'){
        emitParticles = !emitParticles;
    }
}

