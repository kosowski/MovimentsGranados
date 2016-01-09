//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene3.h"

void XBScene3::setup()
{
    XBBaseScene::setup();
    
    showGui = false;
    int w = 200;
    int h = 250;
    colorPickerV.setSize(10, ofGetHeight() - h - 20, w, h);
    colorPickerX.setSize(10, ofGetHeight() - 2 * (h + 20), w, h);
    ofColor vColor(255,243,183);
    ofColor xColor(252,185,112);
    colorPickerV.setColor( vColor);
    colorPickerX.setColor(xColor);

    v.setup(ofGetWidth() / 2, ofGetHeight() / 2);
    x.setup(ofGetWidth() / 2, ofGetHeight() / 2);

}

void XBScene3::update()
{
    XBBaseScene::update();
    
    v.update();
    x.update();
    
    colorPickerV.update();
    colorPickerX.update();
    
    v.setColor(colorPickerV.getColor());
    x.setColor( colorPickerX.getColor() );

}

void XBScene3::drawIntoFBO()
{
    fbo.begin();
    {
        ofBackground(0);
        v.draw();
        x.draw();
        
        if(showGui){
            colorPickerV.draw();
            colorPickerX.draw();
        }

        drawFadeRectangle();
    }
    fbo.end();
}

void XBScene3::keyReleased(int key){
    if(key == 'g')
        showGui = !showGui;
}
