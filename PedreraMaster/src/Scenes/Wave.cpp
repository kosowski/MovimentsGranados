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

#include "Wave.h"


Wave::Wave(ofPoint o, int w_, float a, float p) {
    origin.set(o.x, o.y);
    w = w_;
    for (int i = 0; i < numComponents; i++) {
        period = p / (2*(i+1));
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


void Wave::update() {
    // Increment theta (try different values for 'angular velocity' here
    theta += 0.02;
    
    // Accumulate wave height values
    for (int j = 0; j < numComponents; j++) {
        float x = theta;
        for (int i = 0; i < particles.size(); i++) {
            
            if(j==0)
                particles[i].set(origin.x+i*xspacing, origin.y);
            
//            float dist = 1 - particles[i].distance( attractor) / maxDist;
//            ofPoint attractor2 ( attractor.x - ofGetWidth()/2, attractor.y);
//            float dist2 = 1 - particles[i].distance( attractor2) / maxDist;
            
            float attraction = 0;
            for(int k= 0; k< attractors.size(); k++){
                float dist = 1 - particles[i].distance( attractors[k]) / maxDist;
                attraction += pow(dist, attractorRadius) * attractorStrength;
            }
            // Every other wave is cosine instead of sine
            // if (j % 2 == 0)
            //particles[i] += ofPoint(0, sin(x)*amplitude[j] * pow(dist, attractorRadius) * attractorStrength);
            particles[i] += ofPoint(0, sin(x)*amplitude[j] * attraction );
            //else
            //particles[i].location.add(0,  cos(x)*amplitude[j] * dist);
            x+=dx[j];
        }
    }
}


void Wave::display() {
   // setAttractor(ofGetMouseX(), ofGetMouseY());
    // A simple way to draw the wave with an ellipse at each location
    for (int i = 0; i < particles.size(); i++) {
        if(i != particles.size() - 1)
            ofDrawLine( particles[i].x,   particles[i].y,
                 particles[i+1].x, particles[i+1].y );
    }
}

void Wave::setAttractor(int index, float x, float y, float strength, float radius){
    if( index >= attractors.size()){
        attractors.push_back(ofPoint(x, y));
        index = attractors.size() - 1;
    }
    attractors[index].x=x;
    attractors[index].y=y;
    attractorStrength = strength;
    attractorRadius = radius;
    
}