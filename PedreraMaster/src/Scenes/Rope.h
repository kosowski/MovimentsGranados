//
//  Rope.h
//  PedreraMaster
//
//  Created by nacho on 08/01/16.
//
//

#ifndef Rope_h
#define Rope_h

#include "ofMain.h"

class Rope{
public:
    float vx, vy; // The x- and y-axis velocities
    float gravity;
    float mass;
    float radius;
    float stiffness = 0.05;
    float damping = 0.75;
    ofPoint* a;
    ofPoint* b;
    
    Rope(ofPoint* a, ofPoint* b, float m, float g, float r);
    void update();
    void draw();
};
#endif /* Rope_h */
