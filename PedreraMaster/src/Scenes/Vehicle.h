#pragma once

#include "ofMain.h"
#include "Rope.h"

class Vehicle{

public:
    void setup(int x, int y);
    void applyForce(const ofPoint & force);
    void seek(const ofPoint & target);
    void update();
    void draw();
    void setColor(ofColor c);
    ofPoint getLocation(){return location;};
    
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