//
//  ofxCustomGlow.h
//  PedreraMaster
//
//  Created by Nacho Cossio on 28/01/16.
//  Based on ofxGlow.h by by Patricio Gonzalez Vivo on 03/07/11.

/*
 *  ofxGlow.h
 *
 *  Created by Patricio Gonzalez Vivo on 03/07/11.
 *  Copyright 2011 http://www.PatricioGonzalezVivo.com All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************
 *
 *  This is a simple implementation of Glow based on bluring and adding the orriginal layer.
 *  There is a better way of doing this by a shader... comming soon...
 *
 */

#pragma once

#include "ofMain.h"

#include "ofxGaussianBlur.h"
class ofxCustomGlow : public ofxGaussianBlur {
public:
    
    ofxCustomGlow(){
        passes = 1;
        internalFormat = GL_RGBA;
        intensity = 1;
        radius = 3;
    }
    
    void draw(int x = 0, int y = 0, float _width = -1, float _height = -1){
        if (_width == -1) _width = width;
        if (_height == -1) _height = height;
        
        ofPushStyle();
        //ofEnableBlendMode(OF_BLENDMODE_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        for(int i = 0; i< intensity; i++)
            pingPong.dst->draw(x, y, _width, _height);
       
        textures[0].draw(x,y, _width, _height);
        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ofPopStyle();
    }
    void setIntensity(int i){intensity = i;}
    
protected:
    int intensity;
};
