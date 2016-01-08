//
//  Rope.cpp
//  PedreraMaster
//
//  Created by nacho on 08/01/16.
//
//

#include "Rope.h"

Rope::Rope(ofPoint* pointA, ofPoint* pointB, float m, float g, float r) {
    a = pointA;
    b = pointB;
    mass = m;
    gravity = g;
    radius = r;
}

void Rope::update() {
    float forceX = (a->x - b->x) * stiffness;
    float ax = forceX / mass;
    vx = damping * (vx + ax);
    b->x += vx;
    float forceY = (a->y - b->y) * stiffness;
    //forceY += gravity;
    float ay = forceY / mass;
    vy = damping * (vy + ay);
    b->y += vy;
}

void Rope::draw() {
    ofDrawLine(a->x, a->y, b->x, b->y);
}