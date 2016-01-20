//
//  BlurEffect.h
//  PedreraMaster
//
//  Created by nacho on 14/01/16.
//
//

#ifndef BlurEffect_h
#define BlurEffect_h

#include "ofMain.h"
class BlurEffect{
public:
    
    BlurEffect();
    void setup(float w, float h, int mode);
    void apply(ofFbo *targetFbo, int amount, int iterations);
    
    int iterations, amount;
    bool initialized;
    
    ofFbo firstPass;
    ofShader hShader, vShader;

};

#endif /* BlurEffect_h */
