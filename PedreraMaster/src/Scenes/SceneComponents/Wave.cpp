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
    float history = 1;
    attractors.resize(history * 2);
    for(int i=0; i< history * 2;i++)
        attractors.push_back(attractor{ ofPoint(0, 0), 0 ,0 });
}

Wave::Wave(ofPoint o, int w_, float a, float p, float s, int ori = 0){
    w = w_;
    xspacing = s;
    for(int i=0; i< w; i+=xspacing ){
        origin.push_back(ofPoint(o.x + i, o.y));
    }

    
    for (int i = 0; i < numComponents; i++) {
        period = p / (2*(i+1));
        amplitude.push_back( a / (i+1) );
        dx.push_back( (TWO_PI / period) * xspacing);
    }
    for (int i = 0; i < origin.size(); i++) {
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
                float dist = ofClamp(1 - particles[i].distance( attractors[k].pos) / maxDist, 0.0f, 1.0f);
                attraction += pow(dist, attractors[k].radius) * attractors[k].strength;
            }
            // Every other wave is cosine instead of sine
            // if (j % 2 == 0)
            //particles[i] += ofPoint(0, sin(x)*amplitude[j] * pow(dist, attractorRadius) * attractorStrength);
            if(orientation == 0)
                particles[i] += ofPoint(0, sin(x)*amplitude[j] * attraction );
            else
                particles[i] += ofPoint( sin(x)*amplitude[j] * attraction, 0 );
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

void Wave::setAttractor(int index, float x, float y, float strength, float radius, float decay){
    if( index >= attractors.size()){
        attractors.push_back( attractor{ofPoint(x, y), strength, radius});
        index = attractors.size() - 1;
    }
//    if(ofGetFrameNum() % 2 == 0)
//         updateAttractor( (index+1)%2, decay);
    attractors[index].pos.x=x;
    attractors[index].pos.y=y;
    attractors[index].strength = strength;
    attractors[index].radius = radius;
//    cout << "Set attractor " << ofToString(index) << " at " << x << "," << y <<endl;
}

// attractors hold current attractor position at index 0 and 1 (for both hands) and a list of previous positions
// so index 2 is previous pos to index 0, index 4 to 2....and index 3 is the previous to 1, 5 to 3...
// thats why we decrement the index by two, each call updates the history of one hand
void Wave::updateAttractor(int index, float decay){
    for(int i = attractors.size() - 1 - index; i > 0;i-=2){
        attractors[i].pos.set( attractors[i - 2].pos );
        attractors[i].strength = attractors[i - 2].strength * decay;
        attractors[i].radius = attractors[i - 2].radius ;
    }
}