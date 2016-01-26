//
//  CustomBox2dParticle.h
//  PedreraMaster
//
//  Created by nacho on 22/01/16.
//
//

#ifndef CustomBox2dParticle_h
#define CustomBox2dParticle_h

#include "ofMain.h"
#include "ofxBox2dCircle.h"

class CustomBox2dParticle : public ofxBox2dCircle {
    
public:
    
    int life;
    
    CustomBox2dParticle() {
    }
    
    
    ofColor color;
//    ofTexture texture;
    void update(){
//        cout << "update custom particle" << endl;
        life--;
        if(life<0)
            alive = false;
    }
    
    void draw(ofTexture& texture) {
        ofPushMatrix();
        ofTranslate(getPosition().x, getPosition().y, 0);
        ofSetColor(color);
//        ofFill();
//        ofEnableAlphaBlending();
//        texture.setAnchorPercent(0.50, 0.50);
//        ofRectMode(CENTER);
        texture.draw(0, 0, getRadius() * 2, getRadius() *2 );
        ofPopMatrix();
        
    };
    
    void setup(b2World * b2dworld, float x, float y, float r, int l) {
        ofxBox2dCircle::setup(b2dworld, x, y, r);
        life = l;
        b2Filter f;
        f.categoryBits = 0x0002;
        f.maskBits =0x0001;
        setFilterData(f);
    };
    
    void setColor(ofColor col){
        color = col;
    };
    
};  

static bool shouldRemove(shared_ptr<CustomBox2dParticle> shape, int w, int h){
     return  !shape.get()->alive && !ofRectangle(0, 0, w, h).inside(shape.get()->getPosition());
};




#endif /* CustomBox2dParticle_h */
