#pragma once

#include "ofMain.h"
#include "Rope.h"

class Vehicle{

public:
    void setup(int x, int y);
    void applyForce(const ofPoint & force);
    void seek(const ofPoint & target);
    void update();
    void draw(ofTexture& tex);
    void setColor(ofColor c);
    ofPoint getLocation(){return nodes[0];};
    void setMaxSpeed(float s);
    void setMaxForce(float f);
    void setTailDamping(float f);
    void setTailStiffness(float s);
    void setMass(float f);
    void setSize(float radius);
    float glowRadius;
    float glowAmount;
private:
    
    ofPoint location;
    ofPoint velocity;
    ofPoint acceleration;
    float maxSpeed;
    float maxForce; 
    int r, cor;
    float offset;
    int numNodes = 4;
    ofColor colour;

    vector <ofPoint> nodes;
    vector <Rope> springs;
};