	//
//  BlurEffect.cpp
//  PedreraMaster
//
//  Created by nacho on 14/01/16.
//
//

#include "BlurEffect.h"

BlurEffect::BlurEffect() {
    initialized = false;
}

void BlurEffect::setup(float w, float h, int mode) {
    
    firstPass.allocate(w,h);
    
    firstPass.begin();
    ofClear(0, 0, 0, 0);
    firstPass.end();
  
    string vertexShader =
    "void main() {\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
    gl_Position = ftransform();\
    }";
    
    //what the blur shader does: it takes blurAmnt as input and takes for each 'pixel' / fragment a
    //weighted average of itself and 8 horizontal neighbours (which neighbours depends on the blurAmnt
    
    string fragmentShaderHorizontal =
    "uniform sampler2DRect src_tex_unit0;\
    uniform float blurAmount;\
    void main(void) {\
    vec2 st = gl_TexCoord[0].st;\
    vec4 color=vec4(0.);\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -4.0, 0.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -3.0, 0.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -2.0, 0.0));\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -1.0, 0.0));\
    color += 5.0 * texture2DRect(src_tex_unit0, st );\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 1.0, 0.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 2.0, 0.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 3.0, 0.0));\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 4.0, 0.0));\
    color /= 25.0;\
    gl_FragColor = color;\
    }";
    
    // same shader but with only one tap
    string fragmentShaderHorizontalKernel1 =
    "uniform sampler2DRect src_tex_unit0;\
    uniform float blurAmount;\
    void main(void) {\
    vec2 st = gl_TexCoord[0].st;\
    vec4 color=vec4(0.);\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * -1.0, 0.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st );\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmount * 1.0, 0.0));\
    color /= 4.0;\
    gl_FragColor = color;\
    }";
    
    // same but now for vertical neighbours
    
    string fragmentShaderVertical =
    "uniform sampler2DRect src_tex_unit0;\
    uniform float blurAmount;\
    void main(void) {\
    vec2 st = gl_TexCoord[0].st;\
    vec4 color=vec4(0.);\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 4.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 3.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 2.0));\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 1.0));\
    color += 5.0 * texture2DRect(src_tex_unit0, st );\
    color += 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -1.0));\
    color += 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -2.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -3.0));\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -4.0));\
    color /= 25.0;\
    gl_FragColor = color;\
    }";
    
    // same shader but only one tap
    string fragmentShaderVerticalKernel1 =
    "uniform sampler2DRect src_tex_unit0;\
    uniform float blurAmount;\
    void main(void) {\
    vec2 st = gl_TexCoord[0].st;\
    vec4 color=vec4(0.);\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * 1.0));\
    color += 2.0 * texture2DRect(src_tex_unit0, st );\
    color += 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmount * -1.0));\
    color /= 4.0;\
    gl_FragColor = color;\
    }";
    
    if(mode == 0)
        vShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderVerticalKernel1);
    else
        vShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderVertical);
    vShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    vShader.linkProgram();
    
    if(mode == 0)
        hShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderHorizontalKernel1);
    else
        hShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderHorizontal);
    hShader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    hShader.linkProgram();
    
    initialized = true;
}

void BlurEffect::apply(ofFbo *targetFbo, int amount, int iterations) {
    if (!initialized) ofLog(OF_LOG_ERROR, "ofxBlurShader::setup(w,h) needs to be called first");
    
    this->amount = amount;
    this->iterations = iterations;
    
    
    for (int i=0; i<iterations; i++) {
        
        //apply horizontal blur to fbo1 and put the result in fbo2
        firstPass.begin();
        hShader.begin();
        hShader.setUniform1f("blurAmount", amount);
        targetFbo->draw(0,0);
        hShader.end();
        firstPass.end();
        
        //apply vertical blur to fbo2 and put this back in fbo1
        targetFbo->begin();
        vShader.begin();
        vShader.setUniform1f("blurAmount", amount);
        firstPass.draw(0,0);
        hShader.end();
        targetFbo->end();
    }
}


