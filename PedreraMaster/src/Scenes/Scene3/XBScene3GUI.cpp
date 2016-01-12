//
// Created by Miquel Àngel Soler on 12/1/16.
//

#include "XBScene3GUI.h"
#include "XBScene3.h"

XBScene3GUI::XBScene3GUI(XBScene3 *_scene) : XBBaseGUI(_scene)
{
    scene = _scene;

    gui.setup();
    gui.add(particleSize.setup("Particle size", 10, 2, 40));
    gui.add(particleLife.setup("Particle life", 30, 1, 60));
    gui.add(particleVelocity.setup("Particle velocity", ofVec2f(0,50), ofVec2f(0,0), ofVec2f(150,150)));
    gui.add(particleSpread.setup("Particle spread", ofVec2f(80,5), ofVec2f(0,0), ofVec2f(150,150)));
}

void XBScene3GUI::draw()
{
    gui.draw();
}
