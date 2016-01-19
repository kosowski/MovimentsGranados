//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene1.h"
#include "XBScene1GUI.h"

#define NODE_SIZE 3
#define GRID_X_RES 120
#define GRID_Y_RES 10

void XBScene1::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
    
    //ofSetBackgroundAuto(true);
    
    initSVG();
    initSystem();
    initParticles();
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0 );
}



void XBScene1::update()
{
    XBBaseScene::update();
    
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    // increase time marker
    violinTimeIndex--;
    if(violinTimeIndex < 0)
        violinTimeIndex =  ofGetHeight() / MAIN_WINDOW_SCALE;

    if(violinLineIndex == 0)
        currentViolinNote.getClosestPoint(ofPoint(0, violinTimeIndex), &violinLineIndex);
    
    celloTimeIndex++;
    if(celloTimeIndex > ofGetWidth() / MAIN_WINDOW_SCALE){
        celloTimeIndex =  0;
        currentViolinNote.getClosestPoint(ofPoint(0, violinTimeIndex), &violinLineIndex);
    }
    
//    if(violinLineIndex == 0)
    
    // if note ON, update emitter location along the current line
    if(fakeEvent){
        //        violinLine.addVertex(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        violinLineIndex++;
        vEmitter.setPosition(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        particleSystem.addParticles(vEmitter);
    }
    if(fakeCelloEvent){
        //        violinLine.addVertex(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        celloLineIndex++;
        xEmitter.setPosition(currentCelloNote.getPointAtIndexInterpolated(celloLineIndex));
        particleSystem.addParticles(xEmitter);
    }
    updateEmitters();
    
    
    // director update
    p_mouse->position.set(ofGetMouseX(), ofGetMouseY(), 0);
    physics->tick();
    for ( int i = 0; i <particles.size(); ++i )
    {
        Particle* v = particles[i];
        v->velocity.set(v->velocity.x * myGUI->xDamping,v->velocity.y ,0);
    }
}

void XBScene1::drawIntoFBO()
{
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    fbo.begin();
    {
        ofBackground(0);
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        
        // draw springs
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        for ( int i = 0; i < visibleSprings.size(); ++i )
        {
            Spring* e = visibleSprings[i];
            Particle* a = e->getOneEnd();
            Particle* b = e->getTheOtherEnd();
            ofDrawLine( a->position.x, a->position.y, b->position.x, b->position.y );
        }
        
        // draw particles
        if(myGUI->showTimeMarker){
            ofSetColor(220);
            ofDrawLine(0,violinTimeIndex, ofGetWidth() /MAIN_WINDOW_SCALE, violinTimeIndex);
            ofDrawLine(celloTimeIndex, 0, celloTimeIndex, ofGetHeight() / MAIN_WINDOW_SCALE);
        }
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        particleSystem.draw(pTex);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPopStyle();
        
        
        drawGUI();
        drawFadeRectangle();
    }
    fbo.end();
    
    blur.apply(&fbo, myGUI->blurAmount, 1);
}

//--------------------------------------------------------------
void XBScene1::keyPressed(int key){
    XBBaseScene::keyPressed(key);
    
    if(key == 'x'){
        if(fakeEvent == false){
            fakeEvent = true;
            int wichLine = (int) ofRandom( verticalLines.size());
            //            cout << "Line changed to " << ofToString(wichLine) << endl;
            currentViolinNote = verticalLines[wichLine];
            //TODO: find a better way to do this
            ofPoint startPoint = currentViolinNote.getClosestPoint(ofPoint(0, violinTimeIndex), &violinLineIndex);
        }
    }
    else if(key == 'c'){
        if(fakeCelloEvent == false){
            fakeCelloEvent = true;
            int wichLine = (int) ofRandom( horizontalLines.size());
            //            cout << "Line changed to " << ofToString(wichLine) << endl;
            currentCelloNote = horizontalLines[wichLine];
            //TODO: find a better way to do this
            ofPoint startPoint = currentCelloNote.getClosestPoint(ofPoint( celloTimeIndex, 0), &celloLineIndex);
        }
    }

}

void XBScene1::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);
    
    switch(key)
    {
        case 'z':
        case 'Z':
        {
            particles.clear();
            visibleSprings.clear();
            delete physics;
            initSystem();
            break;
        }
        case 'x':
        case 'X':
        {
            fakeEvent = false;
//            linesToDraw.push_back(violinLine);
//            violinLine.clear();
        }
        case 'c':
        case 'C':
        {
            fakeCelloEvent = false;
//            violinLine.clear();
        }

        default: break;
    }
}

void XBScene1::initSVG(){
    // load vertical lines
    svg.load("resources/verticales.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(1);
            vector<ofPoint> points = pl.getVertices();
            //check path direction
            if(points.size() > 51){
                if(points[0].y < points[50].y){ //check if the order is top to bottom, we dont want that
                    std::reverse(points.begin(), points.end());
                    pl.clear();
                    pl.addVertices(points);
                }
            }
            verticalLines.push_back(pl);
        }
    }
    
    // LOAD HORIZINTAL LINES
    svg.load("resources/horizontales.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(1);
//            vector<ofPoint> points = pl.getVertices();
//            //check path direction
//            if(points.size() > 51){
//                if(points[0].y < points[50].y){ //check if the order is top to bottom, we dont want that
//                    std::reverse(points.begin(), points.end());
//                    pl.clear();
//                    pl.addVertices(points);
//                }
//            }
            horizontalLines.push_back(pl);
        }
    }
}

void XBScene1::initParticles(){
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
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


void XBScene1::initSystem()
{
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    physics	= new ParticleSystem( myGUI->gravity, myGUI->drag );
    physics->clear();
    
    
    p_mouse = physics->makeParticle(1.0, 0.0, 0.0, 0.0);
    p_mouse->setFixed( true );
    
    
    float gridStepX = (float)( (ofGetWindowWidth() / MAIN_WINDOW_SCALE) / GRID_X_RES );
    float gridStepY = (float)( (ofGetWindowHeight() / MAIN_WINDOW_SCALE)  / GRID_Y_RES );
    
    for( int y=0; y<GRID_Y_RES; y++ ){
        for( int x=0; x<GRID_X_RES; x++ ){
            ofVec3f pos = ofVec3f( x * gridStepX , y * gridStepY , 0.0f );
            
            //free particle
            Particle* p = physics->makeParticle( myGUI->particleMass, pos.x, pos.y, pos.z );
            
            // particles in the extremes are fixed
            if(x== 0 || x == GRID_X_RES - 1)
                p->setFixed(true);
            // the rest are connected through a spring to their fixed particle
            else{
                //fixed particle to which free particle is attached through a spring
                Particle* fixed = physics->makeParticle( myGUI->particleMass, pos.x, pos.y, pos.z );
                fixed->setFixed( true );
                
                Spring* s = physics->makeSpring(fixed, p, myGUI->fixedStrength, myGUI->fixedDamping, myGUI->fixedRestLength);
                //            visibleSprings.push_back(s);
            }
            
            
            physics->makeAttraction(p_mouse, p, myGUI->mouseStrength, myGUI->mouseSlope);
            
            particles.push_back( p );
            if( x > 0 ){
                Spring* s = physics->makeSpring( particles[y*GRID_X_RES + x - 1], particles[y*GRID_X_RES + x], myGUI->springStrength, myGUI->springDamping, gridStepX + myGUI->restLength);
                visibleSprings.push_back(s);
            }
        }
    }
}

void XBScene1::updateEmitters(){
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
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
    
    // update particle system
    float dt = min(ofGetLastFrameTime(), 1.0/10.0);
    particleSystem.update(dt, 1.);
    
    
}
