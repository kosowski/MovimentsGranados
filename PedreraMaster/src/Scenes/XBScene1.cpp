//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene1.h"

#define NODE_SIZE 3
#define GRID_X_RES 120
#define GRID_Y_RES 10


void XBScene1::setup()
{
    XBBaseScene::setup();
    
    //ofSetBackgroundAuto(true);
    gui.setup();
    gui.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
    gui.add(springDamping.setup("damping", 0.0, 0.000, 0.201));
    gui.add(restLength.setup("restLength", 0., -10., 20.));
    
    gui.add(drag.setup("drag", 0.1, 0.01, 1.00));
    gui.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
    gui.add(particleMass.setup("particleMass", 0.2, 0., 1.00));
    gui.add(mouseStrength.setup("mouseStrength", -100000, -6000, -200000));
    gui.add(mouseSlope.setup("mouseSlope", 70, 60., 260));
    gui.add(fixedStrength.setup("fixedStrength", 0.06, 0.01, 0.50));
    gui.add(fixedDamping.setup("fixedDamping",  0.00, 0.000, 0.201));
    gui.add(fixedRestLength.setup("fixedRestLength", 0., 0., 40.));
    
    gui.add(xDamping.setup("xDamping", 1., 0.00, 1.00));
    
    directorColor.set(77,125,140);
    initSystem();
}

void XBScene1::initSystem(){
    
    physics	= new ParticleSystem( gravity, drag );
    physics->clear();
    
    
    p_mouse = physics->makeParticle(1.0, 0.0, 0.0, 0.0);
    p_mouse->setFixed( true );
    
    
    float gridStepX = (float)( ofGetWindowWidth() / GRID_X_RES );
    float gridStepY = (float)( ofGetWindowHeight()  / GRID_Y_RES );
    
    for( int y=0; y<GRID_Y_RES; y++ ){
        for( int x=0; x<GRID_X_RES; x++ ){
            ofVec3f pos = ofVec3f( x * gridStepX , y * gridStepY , 0.0f );
            
            //free particle
            Particle* p = physics->makeParticle( particleMass, pos.x, pos.y, pos.z );
            
            // particles in the extremes are fixed
            if(x== 0 || x == GRID_X_RES - 1)
                p->setFixed(true);
            // the rest are connected through a spring to their fixed particle
            else{
                //fixed particle to which free particle is attached through a spring
                Particle* fixed = physics->makeParticle( particleMass, pos.x, pos.y, pos.z );
                fixed->setFixed( true );
                
                Spring* s = physics->makeSpring(fixed, p, fixedStrength, fixedDamping, fixedRestLength);
                //            visibleSprings.push_back(s);
            }
            
            
            physics->makeAttraction(p_mouse, p, mouseStrength, mouseSlope);
            
            particles.push_back( p );
            if( x > 0 ){
                Spring* s = physics->makeSpring( particles[y*GRID_X_RES + x - 1], particles[y*GRID_X_RES + x], springStrength, springDamping, gridStepX + restLength);
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

void XBScene1::update()
{
    XBBaseScene::update();
    
    p_mouse->position.set(ofGetMouseX(), ofGetMouseY(), 0);
    physics->tick();
    for ( int i = 0; i <particles.size(); ++i )
    {
        Particle* v = particles[i];
        v->velocity.set(v->velocity.x * xDamping,v->velocity.y ,0);
    }
}

void XBScene1::drawIntoFBO()
{
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
        ofSetColor(directorColor);
        for ( int i = 0; i < visibleSprings.size(); ++i )
        {
            Spring* e = visibleSprings[i];
            Particle* a = e->getOneEnd();
            Particle* b = e->getTheOtherEnd();
            ofDrawLine( a->position.x, a->position.y, b->position.x, b->position.y );
        }
        
        if(showGui)
            gui.draw();

        drawFadeRectangle();
    }
    fbo.end();
}

void XBScene1::keyReleased(int key){
    if(key == 'z'){
        particles.clear();
        visibleSprings.clear();
        delete physics;
        initSystem();
    }
    else if(key == 'g')
        showGui = !showGui;
}
