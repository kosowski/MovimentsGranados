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
    initPaths();
    vPathIndex = 0;//vPath.size()/2;
    xPathIndex= 0;//xPath.size()/2;
    v.setup(vPath.getPointAtIndexInterpolated(vPathIndex).x, vPath.getPointAtIndexInterpolated(vPathIndex).y);
    x.setup(xPath.getPointAtIndexInterpolated(xPathIndex).x, xPath.getPointAtIndexInterpolated(xPathIndex).y);

    initWaves();
    initParticles();
    initSVG();
   
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0 );
}

void XBScene3::update()
{
    XBBaseScene::update();

    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    // update violin and cello positions and colours
    vPathIndex += myGUI-> pathSpeed;
    v.setSize(myGUI->size);
    v.setMaxForce(myGUI->maxForce);
    v.setMaxSpeed(myGUI->maxSpeed);
    v.setTailStiffness(myGUI->stiffness);
    v.setTailDamping(myGUI->damping);
    v.setMass(myGUI->mass);
    ofPoint vTarget = vPath.getPointAtIndexInterpolated(vPathIndex);
    v.seek(vTarget);
    v.update();
    
    xPathIndex += myGUI-> pathSpeed;
    x.setSize(myGUI->size);
    x.setMaxForce(myGUI->maxForce);
    x.setMaxSpeed(myGUI->maxSpeed);
    x.setTailStiffness(myGUI->stiffness);
    x.setTailDamping(myGUI->damping);
    x.setMass(myGUI->mass);
    ofPoint xTarget = xPath.getPointAtIndexInterpolated(xPathIndex);
    x.seek(xTarget);
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
    
    // update piano's stones
    for (int i=0; i<stonesToDraw.size(); i++) {
        stonesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        stonesToDraw[i].amplitude *= myGUI->stoneDamping;
        if(stonesToDraw[i].life >  ofGetFrameRate() * myGUI->stoneTime){
            stonesToDraw.erase(stonesToDraw.begin()+i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
    
    // update waves
    for(int i=0; i< waves.size();i++){
        float mouseX = ofGetMouseX() / (float)ofGetWidth();
        float mouseY = ofGetMouseY() / (float)ofGetHeight();
        waves[i].setAttractor(0, mouseX * MAIN_WINDOW_WIDTH, mouseY * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius);
        float fakeX = (mouseX - 0.5) * MAIN_WINDOW_WIDTH + 600 * (ofNoise( ofGetElapsedTimeMillis() * 0.0005) - 0.5 );
        float fakeY = mouseY * MAIN_WINDOW_HEIGHT + 600 * ( ofNoise( ofGetElapsedTimeMillis() * 0.0005 + 1000) - 0.5 );
        waves[i].setAttractor(1, fakeX, fakeY, myGUI->attractorStrength, myGUI->attractorRadius);
        waves[i].update();
    }

}

void XBScene3::drawIntoFBO()
{
    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    fbo.begin();
    {
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        if(showFacadeImage)
            templateImage.draw(0,0);
        else
            ofBackground(0);
        if(showTemplate){
            ofSetColor(255);
            svg.draw();
        }
        
        // draw directors waves
        ofPushStyle();
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        ofSetLineWidth(myGUI->lineWidth);
        for(Wave w:waves)
            w.display();
        ofPopStyle();
        
        ofPushStyle();
        // draw expanding stones from piano
        for (int i=0; i<stonesToDraw.size(); i++) {
            expandingPolyLine &e = stonesToDraw[i];
            ofPushMatrix();
            ofTranslate(e.centroid);
//             ofScale(e.life * myGUI->stoneGrowFactor, e.life * myGUI->stoneGrowFactor);
            ofScale( 1 +  e.amplitude * sin(myGUI->stoneFrequency * e.life),
                     1 + e.amplitude * sin(myGUI->stoneFrequency * e.life ) );
            e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA - e.life * myGUI->stoneAlphaDecrease, 0, 255) ) );
            e.path.draw();
            ofPopMatrix();
        }
        ofPopStyle();
        
        ofPushStyle();
        //draw particles from violin and cello
        if(emitParticles){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            particleSystem.draw(pTex);
        }
        
        // draw violin and cello
        if(myGUI->showPath){
            ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));
            vPath.draw();
            ofSetColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));
            xPath.draw();
        }
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        v.draw();
        x.draw();
        ofPopStyle();
        
        // mask for removing the windows
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();
        
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
    
    XBScene3GUI *myGUI = (XBScene3GUI *)gui;

    if(key == 'p'){
        emitParticles = !emitParticles;
    }
    else if(key == 'x'){
//        currentOutlines.push_back(outlines[ (int) ofRandom(outlines.size() - 1)]);
        expandingPolyLine e = stones[ (int) ofRandom(stones.size() - 1)];
        e.life= 1;
        e.amplitude = myGUI->stoneGrowFactor;
        stonesToDraw.push_back(e);
    }
    else if(key == 'z'){
        waves.clear();
        initWaves();
    }
}

void XBScene3::initPaths(){
    svg.load("resources/caminosEscena3.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        if(lines.size() > 1)
            cout << "Error en formato caminos escena 3" << endl;

        ofPolyline pl = lines[0].getResampledBySpacing(1);
        if(i== 1)
            vPath = pl;
        else if( i== 2)
            xPath = pl;
        
    }

}

void XBScene3::initSVG()
{
    svg.load("resources/Esc3y4Piano.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
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

void XBScene3::initWaves(){
    // Initialize a wave with starting point, width, amplitude, and period
    
    XBScene3GUI *myGUI = (XBScene3GUI *)gui;
    
    int spacing = 10;

    // create horzontal waves
    svg.load("resources/horizontalesv02.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
           ofPolyline l(lines[j].getResampledBySpacing(spacing));
           waves.push_back( Wave( l.getVertices(), 20, ofRandom(myGUI->minPeriod, myGUI->maxPeriod), spacing, 0));
        }
    }
    
    // create vertical waves
    svg.load("resources/verticalesv02.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
             waves.push_back( Wave( l.getVertices(), 20, ofRandom(myGUI->minPeriod, myGUI->maxPeriod), spacing, 1));
        }
    }
}
