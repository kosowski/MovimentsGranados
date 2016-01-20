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


Wave::Wave(vector<ofPoint>& o, float a, float p, float s, int ori = 0) {
    origin = o;
    xspacing = s;
    orientation = ori;
    for (int i = 0; i < numComponents; i++) {
        period = p / (2*(i+1));
        amplitude.push_back( a / (i+1) );
        dx.push_back( (TWO_PI / period) * xspacing);
    }
    for (int i = 0; i < o.size(); i++) {
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
                particles[i].set(origin[i].x, origin[i].y);
    
            float attraction = 0;
            for(int k= 0; k< attractors.size(); k++){
                float dist = ofClamp(1 - particles[i].distance( attractors[k]) / maxDist, 0.0f, 1.0f);
                attraction += pow(dist, attractorRadius) * attractorStrength;
            }
            // Every other wave is cosine instead of sine
            // if (j % 2 == 0)
            //particles[i] += ofPoint(0, sin(x)*amplitude[j] * pow(dist, attractorRadius) * attractorStrength);
            if(orientation == 0)
                particles[i] += ofPoint(0, sin(x)*amplitude[j] * attraction );
            else
                particles[i] += ofPoint( sin(x)*amplitude[j] * attraction, x );
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
//    cout << "Set attractor " << ofToString(index) << " at " << x << "," << y <<endl;
}