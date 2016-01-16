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
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0 );
}



void XBScene1::update()
{
    XBBaseScene::update();
    
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    // increase time marker
    timeIndex++;
    if(timeIndex > ofGetHeight())
        timeIndex =0;
    
    // if note ON, add next vertex to violinLine
    if(fakeEvent){
        violinLine.addVertex(currentNote.getPointAtIndexInterpolated(violinLineIndex));
        violinLineIndex++;
    }
    
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
        // draw vertices
        //        ofSetColor(180);
        //        for ( int i = 0; i <particles.size(); ++i )
        //        {
        //            Particle* v = particles[i];
        //            ofDrawCircle( v->position.x, v->position.y, NODE_SIZE/2.0f );
        //        }
        
        // draw springs
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        for ( int i = 0; i < visibleSprings.size(); ++i )
        {
            Spring* e = visibleSprings[i];
            Particle* a = e->getOneEnd();
            Particle* b = e->getTheOtherEnd();
            ofDrawLine( a->position.x, a->position.y, b->position.x, b->position.y );
        }
        
        if(fakeEvent){
            ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            for (int i=0; i<3; i++) {
                ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, ofClamp(myGUI->colorViolinA - myGUI->ViolinBorderFade * i, 0, 255) ));
                ofSetLineWidth(myGUI->violinBorderSize * i+1);
                violinLine.draw();
            }
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofPopStyle();
        }
        
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
            int wichLine = indices[ (int) ofRandom( (sizeof(indices)/sizeof(*indices)) - 1)];
            //            cout << "Line changed to " << ofToString(wichLine) << endl;
            currentNote = verticalLines[wichLine];
            //TODO: find a better way to do this
            ofPoint startPoint = currentNote.getClosestPoint(ofPoint(0, timeIndex), &violinLineIndex);
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
            linesToDraw.push_back(violinLine);
            violinLine.clear();
        }
        default: break;
    }
}

void XBScene1::initSVG(){
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
            if(points.size() > 51)
                if(points[0].y > points[50].y){ //bottom to top, we dont want that
                    std::reverse(points.begin(), points.end());
                    pl.clear();
                    pl.addVertices(points);
                }
            
            verticalLines.push_back(pl);
        }
    }
}

void XBScene1::initSystem()
{
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    physics	= new ParticleSystem( myGUI->gravity, myGUI->drag );
    physics->clear();
    
    
    p_mouse = physics->makeParticle(1.0, 0.0, 0.0, 0.0);
    p_mouse->setFixed( true );
    
    
    float gridStepX = (float)( ofGetWindowWidth() / GRID_X_RES );
    float gridStepY = (float)( ofGetWindowHeight()  / GRID_Y_RES );
    
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
    
    
    //    for( int y=1; y<GRID_Y_RES; y++ ){
    //        for( int x=0; x<GRID_X_RES; x++ ){
    //            physics->makeSpring( particles[ (y - 1) * GRID_X_RES + x], particles[y*GRID_X_RES + x], springStrength, springDamping, gridStepY );
    //        }
    //    }
    
}
