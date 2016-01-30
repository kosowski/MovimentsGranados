//
//  Wave.cpp
//  PedreraMaster
//
//  Created by nacho on 09/01/16.
//
//

// The Nature of Code
// Daniel Shiffman
// http://natureofcode.com

#include "SimpleWave.h"


SimpleWave::SimpleWave(ofPoint o, int w_, float a, float p) {
    origin.set(o.x, o.y);
    w = w_;
    for (int i = 0; i < numComponents; i++) {
        if(i !=0)
            period = p * ofRandom(0.2, .8);
        else
            period = p;
        amplitude.push_back( a / (i+1) );
        dx.push_back( (TWO_PI / period) * xspacing);
    }
    //period = p;
    //amplitude = a;
    //dx = (TWO_PI / period) * xspacing;
    float numParticles = w/xspacing;
    particles.resize(numParticles);
    for (int i = 0; i < numParticles; i++) {
        particles.push_back(ofPoint (0,0));
    }
    maxDist = ofPoint(0,0).distance( ofPoint(ofGetWidth(), ofGetHeight()));
}


void SimpleWave::update() {
    // Increment theta (try different values for 'angular velocity' here
    theta += 0.15;
    
    // Accumulate wave height values
    for (int j = 0; j < numComponents; j++) {
        float x = theta;
        for (int i = 0; i < particles.size(); i++) {
            
            if(j==0)
                particles[i].set(origin.x+i*xspacing, origin.y);
            
            float dist = 1 - particles[i].distance( attractor) / maxDist;
            // Every other wave is cosine instead of sine
            // if (j % 2 == 0)
            particles[i] += ofPoint(0, sin(x)*amplitude[j]);
            //else
            //particles[i].location.add(0,  cos(x)*amplitude[j] * dist);
            x+=dx[j];
        }
    }
}


void SimpleWave::display() {
    for (int i = 0; i < particles.size(); i++) {
        if(i != particles.size() - 1)
            ofDrawLine( particles[i].x,   particles[i].y,
                       particles[i+1].x, particles[i+1].y );
    }
}

void SimpleWave::setAttractor(float x, float y, float strength, float radius){
    attractor.x=x;attractor.y=y;
    attractorStrength = strength;
    attractorRadius = radius;
}