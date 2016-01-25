//
// Created by nacho on 09/01/16.
//

#include "XBScene4.h"
#include "XBScene4GUI.h"

void XBScene4::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
    
    initWaves();
    initSVG();
    initReactionDiffusion();
    
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 1 );
}


void XBScene4::update()
{
    XBBaseScene::update();
    XBScene4GUI *myGUI = (XBScene4GUI *)gui;
    
    // update waves
    for(int i=0; i< waves.size();i++){
        float mouseX = ofGetMouseX() / (float)ofGetWidth();
        float mouseY = ofGetMouseY() / (float)ofGetHeight();
        waves[i].setAttractor(0, mouseX * MAIN_WINDOW_WIDTH, mouseY * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius);
        float fakeX = (mouseX - 0.5) * MAIN_WINDOW_WIDTH + 600 * (ofNoise( ofGetElapsedTimeMillis() * 0.0005) - 0.5 );
        float fakeY = mouseY * MAIN_WINDOW_HEIGHT + 600 * ( ofNoise( ofGetElapsedTimeMillis() * 0.0005 + 1000) - 0.5 );
        waves[i].setAttractor(1, fakeX, fakeY, myGUI->attractorStrength, myGUI->attractorRadius);
        waves[i].update();
    }
    
    // update piano's stones
    for (int i=0; i<stonesToDraw.size(); i++) {
        stonesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        stonesToDraw[i].amplitude *= myGUI->stoneDamping;
        if(stonesToDraw[i].life >  ofGetFrameRate() * myGUI->stoneTime){
            stonesToDraw.erase(stonesToDraw.begin()+i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
    // update reaction diffusion
    gray.setK( ofMap(ofGetMouseX(),0,640,0.005,0.095,true) );
    gray.setF( ofMap(ofGetMouseY(),0,480,0.01,0.028,true) );
    gray.update();
}

void XBScene4::drawIntoFBO()
{
    XBScene4GUI *myGUI = (XBScene4GUI *)gui;
    
    fbo.begin();
    {
        ofPushMatrix();
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        if(showFacadeImage)
            templateImage.draw(0,0);
        else
            ofBackground(0);
        if(showTemplate){
            ofSetColor(255);
            svg.draw();
        }
        
        ofPopMatrix();
        
        // draw reaction diffusion with shader to convert it to B&W with transparency
        convertToBW.begin();
        gray.draw();
        convertToBW.end();
        ofPushMatrix();
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        
        // draw directors waves
        ofPushStyle();
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        ofSetLineWidth(myGUI->lineWidth);
        for(Wave w:waves)
            w.display();
        ofPopStyle();
        
        
        // draw expanding stones from piano
        ofPushStyle();
        for (int i=0; i<stonesToDraw.size(); i++) {
            expandingPolyLine &e = stonesToDraw[i];
            ofPushMatrix();
            ofTranslate(e.centroid);
            //             ofScale(e.life * myGUI->stoneGrowFactor, e.life * myGUI->stoneGrowFactor);
            ofScale( 1 +  e.amplitude * sin(myGUI->stoneFrequency * e.life),
                    1 + e.amplitude * sin(myGUI->stoneFrequency * e.life ) );
            e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA - e.life * myGUI->stoneAlphaDecrease, 0, 255) ) );
            e.path.draw();
            ofPopMatrix();
        }
        ofPopStyle();
        
        // mask for removing the windows
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();
        
        drawGUI();
        drawFadeRectangle();
        
        ofPopMatrix();
        
    }
    fbo.end();
    
    blur.apply(&fbo, myGUI->blurAmount, 1);
}

void XBScene4::keyReleased(int key){
    XBBaseScene::keyReleased(key);
    XBScene4GUI *myGUI = (XBScene4GUI *)gui;
    
    if(key == 'z'){
        waves.clear();
        initWaves();
    }
    else if(key == 'x'){
        //        currentOutlines.push_back(outlines[ (int) ofRandom(outlines.size() - 1)]);
        expandingPolyLine e = stones[ (int) ofRandom(stones.size() - 1)];
        e.life= 1;
        e.amplitude = myGUI->stoneGrowFactor;
        stonesToDraw.push_back(e);
    }
}

void XBScene4::initReactionDiffusion(){
    gray.allocate(ofGetWidth(), ofGetHeight());
    
    ofImage seed;
    seed.load("resources/ventanas_v02.png");
    gray.begin();
    seed.draw(0,0, ofGetWidth(), ofGetHeight());
    gray.end();
    
    ofImage mask;
    mask.load("resources/ventanas_mascara.png");
    gray.begin(1);
    mask.draw(0,0, ofGetWidth(), ofGetHeight());
    gray.end(1);
    
    convertToBW.load("resources/shaders/convertToBW");
}

void XBScene4::initWaves(){
    // Initialize a wave with starting point, width, amplitude, and period
    
    XBScene4GUI *myGUI = (XBScene4GUI *)gui;
    
    int spacing = 10;
    
    // create horzontal waves
    svg.load("resources/horizontalesv02.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
            waves.push_back( Wave( l.getVertices(), 20, ofRandom(myGUI->minPeriod, myGUI->maxPeriod), spacing, 0));
        }
    }
    
    // create vertical waves
    svg.load("resources/verticales_v03_pocas_lineas.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
            waves.push_back( Wave( l.getVertices(), 20, ofRandom(myGUI->minPeriod, myGUI->maxPeriod), spacing, 1));
        }
    }
}

void XBScene4::initSVG()
{
    svg.load("resources/Esc3y4Piano.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        // for every line create a shape centered at zero and store its centroid
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(6);
            expandingPolyLine epl;
            epl.life = 0;
            epl.centroid = pl.getCentroid2D();
            vector<ofPoint> points = pl.getVertices();
            for (int k=0; k<points.size(); k++) {
                // store the polyline for now
                epl.line.addVertex(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                // create a path out of the polyline so it can be drawn filled
                if(i == 0) {
                    epl.path.newSubPath();
                    epl.path.moveTo(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                } else {
                    epl.path.lineTo( points[k].x - epl.centroid.x, points[k].y - epl.centroid.y );
                }
            }
            epl.path.close();
            epl.line.close();
            stones.push_back(epl);
        }
    }
}