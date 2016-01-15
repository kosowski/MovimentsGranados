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

    initParticles();
    initSVG();

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
    for (int i=0; i<stonesToDraw.size(); i++) {
        stonesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if(stonesToDraw[i].life >  ofGetFrameRate() * myGUI->stoneTime){
            stonesToDraw.erase(stonesToDraw.begin()+i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
}

void XBScene3::drawIntoFBO()
{
    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    fbo.begin();
    {
        ofBackground(0);
        
        ofFill();
        
        // draw expanding stones from piano
        for (int i=0; i<stonesToDraw.size(); i++) {
            expandingPolyLine &e = stonesToDraw[i];
            ofPushMatrix();
            ofTranslate(e.centroid);
            ofScale(e.life * myGUI->stoneGrowFactor, e.life * myGUI->stoneGrowFactor);
            e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA - e.life * myGUI->stoneAlphaDecrease, 0, 255) ) );
            e.path.draw();
            ofPopMatrix();
        }
        
        //draw particles from violin and cello
        if(emitParticles){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            particleSystem.draw(pTex);
        }
        
        // draw violin and cello
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        v.draw();
        x.draw();
        
        //draw GUI
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
    else if(key == 'x'){
//        currentOutlines.push_back(outlines[ (int) ofRandom(outlines.size() - 1)]);
        expandingPolyLine e = stones[ (int) ofRandom(stones.size() - 1)];
        e.life= 1;
        stonesToDraw.push_back(e);
    }
}

void XBScene3::initSVG()
{
    svg.load("resources/piedras.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        // for every line create a shape centered at zero and store its centroid
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(6);
            expandingPolyLine epl;
            epl.life = 0;
            epl.centroid = pl.getCentroid2D();
            vector<ofPoint> points = pl.getVertices();
            for (int k=0; k<points.size(); k++) {
                // store the polyline for now
                epl.line.addVertex(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                // create a path out of the polyline so it can be drawn filled
                if(i == 0) {
                    epl.path.newSubPath();
                    epl.path.moveTo(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                } else {
                    epl.path.lineTo( points[k].x - epl.centroid.x, points[k].y - epl.centroid.y );
                }
            }
            epl.path.close();
            epl.line.close();
            stones.push_back(epl);
        }
    }
}

void XBScene3::initParticles(){
    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

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
}

