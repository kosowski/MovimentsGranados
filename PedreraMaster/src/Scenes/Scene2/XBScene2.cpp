//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene2.h"
#include "XBScene2GUI.h"

#define NODE_SIZE 6
#define GRID_X_RES 40
#define GRID_Y_RES 40

void XBScene2::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

//    guiOld.setup();
//
//    guiOld.add(springStrength.setup("strength", 0.03, 0.01, 0.50));
//    guiOld.add(springDamping.setup("damping", 0.01, 0.000, 0.201));
//    guiOld.add(drag.setup("drag", 0.1, 0.01, 1.00));
//    guiOld.add(gravity.setup("gravity", 0.0, 0.00, 1.00));
//    guiOld.add(particleMass.setup("particleMass", 0.2, 0., 1.00));
//    guiOld.add(mouseStrength.setup("mouseStrength", -96000, -6000, -200000));
//    guiOld.add(mouseSlope.setup("mouseSlope", 160, 60., 260));
//    guiOld.add(fixedStrength.setup("fixedStrength", 0.03, 0.01, 0.50));
//    guiOld.add(fixedDamping.setup("fixedDamping", 0.01, 0.000, 0.201));

    directorColor.set(77, 125, 140);
    initSystem();
}

void XBScene2::initSystem()
{
    XBScene2GUI *myGUI = (XBScene2GUI *)gui;

    physics = new ParticleSystem(myGUI->gravity, myGUI->drag);
    physics->clear();


    p_mouse = physics->makeParticle(1.0, 0.0, 0.0, 0.0);
    p_mouse->setFixed(true);


    float gridStepX = (float) (ofGetWindowWidth() / GRID_X_RES);
    float gridStepY = (float) (ofGetWindowHeight() / GRID_Y_RES);

    for (int y = 0; y < GRID_Y_RES; y++)
    {
        for (int x = 0; x < GRID_X_RES; x++)
        {
            ofVec3f pos = ofVec3f(x * gridStepX, y * gridStepY, 0.0f);
            Particle *p = physics->makeParticle(myGUI->particleMass, pos.x, pos.y, pos.z);

            //fixed particle to which free particle is attached through a spring
            Particle *fixed = physics->makeParticle(myGUI->particleMass, pos.x, pos.y, pos.z);
            fixed->setFixed(true);
            physics->makeSpring(fixed, p, myGUI->fixedStrength, myGUI->fixedDamping, 2.0);

            physics->makeAttraction(p_mouse, p, myGUI->mouseStrength, myGUI->mouseSlope);

            particles.push_back(p);
            if (x > 0) {
                Spring *s = physics->makeSpring(particles[y * GRID_X_RES + x - 1], particles[y * GRID_X_RES + x], myGUI->springStrength, myGUI->springDamping, gridStepX);
                visibleSprings.push_back(s);
            }
        }
    }


    for (int y = 1; y < GRID_Y_RES; y++) {
        for (int x = 0; x < GRID_X_RES; x++) {
            Spring *s = physics->makeSpring(particles[(y - 1) * GRID_X_RES + x], particles[y * GRID_X_RES + x], myGUI->springStrength, myGUI->springDamping, gridStepY);
            visibleSprings.push_back(s);
        }
    }
}

void XBScene2::update()
{
    XBBaseScene::update();

    p_mouse->position.set(ofGetMouseX(), ofGetMouseY(), 0);
    //physics->setGravity( gravityDir.x, gravityDir.y, gravityDir.z );
    physics->tick();
}

void XBScene2::drawIntoFBO()
{
    fbo.begin();
    {

        ofBackground(0);
        // draw vertices
//        ofSetColor(0.63f, 0.63f, 0.63f);
//        for ( int i = 0; i <particles.size(); ++i )
//        {
//            Particle* v = particles[i];
//            ofDrawCircle( v->position.x, v->position.y, NODE_SIZE/2.0f );
//        }

        // draw springs
        ofSetColor(directorColor);
        for (int i = 0; i < visibleSprings.size(); ++i) {
            Spring *e = visibleSprings[i];
            Particle *a = e->getOneEnd();
            Particle *b = e->getTheOtherEnd();
            ofDrawLine(a->position.x, a->position.y, b->position.x, b->position.y);
        }

        drawGUI();
        drawFadeRectangle();
    }
    fbo.end();
}

void XBScene2::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    switch (key) {
        case 'z':
        case 'Z': {
            particles.clear();
            visibleSprings.clear();
            delete physics;
            initSystem();
            break;
        }
        default:
            break;
    }
}

