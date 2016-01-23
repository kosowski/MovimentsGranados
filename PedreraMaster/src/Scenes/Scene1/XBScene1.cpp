//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene1.h"
#include "XBScene1GUI.h"

bool maskWindows = true;

void XBScene1::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);
    
    initWindows();
    initParticles();
    initWaves();
    initStones();
    initLines();
    
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0 );
}



void XBScene1::update()
{
    XBBaseScene::update();
    
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    // increase time marker
    violinTimeIndex-= myGUI->timeIncrement;
    if(violinTimeIndex < 0)
        violinTimeIndex =  ofGetHeight() / MAIN_WINDOW_SCALE;

    if(violinLineIndex <= 0)
        currentViolinNote.getClosestPoint(ofPoint(0, violinTimeIndex), &violinLineIndex);
    
    celloTimeIndex+= myGUI->timeIncrement;
    if(celloTimeIndex > ofGetWidth() / MAIN_WINDOW_SCALE){
        celloTimeIndex =  0;
        currentViolinNote.getClosestPoint(ofPoint(0, violinTimeIndex), &violinLineIndex);
    }
    
    // if note ON, update emitter location along the current line
    if(fakeEvent || violinEnergy > energyThreshold){
        //        violinLine.addVertex(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        violinLineIndex++;
        vEmitter.setPosition(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        particleSystem.addParticles(vEmitter);
    }
    if(fakeCelloEvent || celloEnergy > energyThreshold){
        //        violinLine.addVertex(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        celloLineIndex++;
        xEmitter.setPosition(currentCelloNote.getPointAtIndexInterpolated(celloLineIndex));
        particleSystem.addParticles(xEmitter);
    }
    updateEmitters();
    
    // update piano's stones
    for (int i=0; i<stonesToDraw.size(); i++) {
        stonesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        stonesToDraw[i].amplitude *= myGUI->stoneDamping;
        if(stonesToDraw[i].life >  ofGetFrameRate() * myGUI->stoneTime){
            stonesToDraw.erase(stonesToDraw.begin()+i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
    
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
}

void XBScene1::drawIntoFBO()
{
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    fbo.begin();
    {
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        if(showFacadeImage)
            templateImage.draw(0,0);
        else
            ofBackground(0);
        if(showTemplate){
            ofSetColor(255);
            svg.draw();
        }

        // draw directors waves
        ofPushStyle();
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        ofSetLineWidth(myGUI->lineWidth);
        for(Wave w:waves)
            w.display();
        ofPopStyle();
        
        
        // draw expanding stones from piano
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
        
        // draw time markers
        if(myGUI->showTimeMarker){
            ofSetColor(220);
            ofDrawLine(0,violinTimeIndex, ofGetWidth() /MAIN_WINDOW_SCALE, violinTimeIndex);
            ofDrawLine(celloTimeIndex, 0, celloTimeIndex, ofGetHeight() / MAIN_WINDOW_SCALE);
        }
        
        // if cello is under a window, paint it
        if(fakeCelloEvent || celloEnergy > energyThreshold){
            for(ofPolyline pl:windowsOutlines){
                if(pl.inside(xEmitter.positionStart)){
                    ofPushStyle();
                    ofSetColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));
                    pl.draw();
                    ofPopStyle();
                    break;
                }
            }
        }
        
        // if violin is under a window, paint it
        if(fakeEvent || violinEnergy > energyThreshold){
            for(ofPolyline pl:windowsOutlines){
                if(pl.inside(vEmitter.positionStart)){
                    ofPushStyle();
                    ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));
                    pl.draw();
                    ofPopStyle();
                    break;
                }
            }
        }
        
        //draw particles
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        particleSystem.draw(pTex);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPopStyle();
        
         // mask for removing the windows
        if(maskWindows == true){
            ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
            mask.draw(0, 0);
            ofPopStyle();
        }
        
        drawGUI();
        drawFadeRectangle();
    }
    fbo.end();
    
    blur.apply(&fbo, myGUI->blurAmount, 1);
}

//--------------------------------------------------------------

void XBScene1::onViolinPitchChanged(float &pitch) {
    int wichLine = floor(pitch * (verticalLines.size() - 1) );
    currentViolinNote = verticalLines[wichLine];
    violinLineIndex = findIntersectionVertical(currentViolinNote, violinTimeIndex);
}

void XBScene1::onViolinEnergyChanged(float &energy) {
    if(energy <= energyThreshold)
        violinEnergy = 0;
    else
        violinEnergy = energy;
}

void XBScene1::onCelloPitchChanged(float &pitch) {
    int wichLine = floor(pitch * (horizontalLines.size() - 1) );;
    currentCelloNote = horizontalLines[wichLine];
    celloLineIndex = findIntersectionHorizontal(currentCelloNote, celloTimeIndex);
}

void XBScene1::onCelloEnergyChanged(float &energy) {
    if(energy <= energyThreshold)
        celloEnergy = 0;
    else
        celloEnergy = energy;
}

void XBScene1::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn) {
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    int wichLine = floor(noteOn.pitch * (stones.size() - 1) );;
    expandingPolyLine e = stones[wichLine];
    e.life= 1;
    e.amplitude = myGUI->stoneGrowFactor;
    stonesToDraw.push_back(e);
}

void XBScene1::onPianoNoteOff(int &noteOff) {
    cout << "Piano NoteOff: p=" << noteOff << endl;
}


//--------------------------------------------------------------
int XBScene1::findIntersectionVertical(ofPolyline &line, int posY){
    //TODO get closer at first, check the posY with the length of the line, so if it is higher than half the length we start comparing at the middle of the line
    //loop through polyline points, when posY changes from lower to higher from the y component of the polyline point
    vector<ofPoint> vertices = line.getVertices();
    for (int i=0; i< vertices.size(); i++) {
        if(posY < vertices[i].y)
            continue;
        else
            return i;
    }
}

int XBScene1::findIntersectionHorizontal(ofPolyline &line, int posX){
    //loop through polyline points, left to right, when posX changes from  higher to lower from the x component of the polyline point
    vector<ofPoint> vertices = line.getVertices();
    for (int i=0; i< vertices.size(); i++) {
        if(posX > vertices[i].x)
            continue;
        else
            return i;
    }
}

//--------------------------------------------------------------
void XBScene1::keyPressed(int key){
    XBBaseScene::keyPressed(key);
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    if(key == 'x'){
        if(fakeEvent == false){
            fakeEvent = true;
            int wichLine = (int) ofRandom( verticalLines.size());
            //            cout << "Line changed to " << ofToString(wichLine) << endl;
            currentViolinNote = verticalLines[wichLine];
            violinLineIndex = findIntersectionVertical(currentViolinNote, violinTimeIndex);
        }
    }
    else if(key == 'c'){
        if(fakeCelloEvent == false){
            fakeCelloEvent = true;
            int wichLine = (int) ofRandom( horizontalLines.size());
            //            cout << "Line changed to " << ofToString(wichLine) << endl;
            currentCelloNote = horizontalLines[wichLine];
            celloLineIndex = findIntersectionHorizontal(currentCelloNote, celloTimeIndex);
        }
    }
    else if(key == 'v'){
        //        currentOutlines.push_back(outlines[ (int) ofRandom(outlines.size() - 1)]);
        expandingPolyLine e = stones[ (int) ofRandom(stones.size() - 1)];
        e.life= 1;
        e.amplitude = myGUI->stoneGrowFactor;
        stonesToDraw.push_back(e);
    }
    else if(key == 'm')
        maskWindows = !maskWindows;

}

void XBScene1::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);
    
    switch(key)
    {
        case 'z':
        case 'Z':
        {
        }
        case 'x':
        case 'X':
        {
            fakeEvent = false;
        }
        case 'c':
        case 'C':
        {
            fakeCelloEvent = false;
//            violinLine.clear();
        }

        default: break;
    }
}

void XBScene1::initLines(){
    // LOAD HORIZINTAL LINES
    svg.load("resources/horizontales.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(1);
            vector<ofPoint> points = pl.getVertices();
            //check path direction
            if(points.size() > 51){
                if(points[0].x > points[50].x){ //check if the order is right to left, we dont want that
                    std::reverse(points.begin(), points.end());
                    pl.clear();
                    pl.addVertices(points);
                }
            }
            horizontalLines.push_back(pl);
        }
    }
    // load vertical lines
    //    svg.load("resources/verticales.svg");
    svg.load("resources/verticales_v03_pocas_lineas.svg");
    for (int i = 0; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(1);
            vector<ofPoint> points = pl.getVertices();
            //check path direction
            if(points.size() > 51){
                if(points[0].y < points[50].y){ //check if the order is top to bottom, we dont want that
                    std::reverse(points.begin(), points.end());
                    pl.clear();
                    pl.addVertices(points);
                }
            }
            verticalLines.push_back(pl);
        }
    }
}

void XBScene1::initWindows(){
    int spacing = 10;
    svg.load("resources/ventanas.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
            l.close();
            windowsOutlines.push_back(l);
        }
    }
}

void XBScene1::initParticles(){
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    emitParticles = false;
    vEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    vEmitter.setVelocity(myGUI->particleVelocity);
    vEmitter.velSpread = myGUI->particleSpread;
    vEmitter.life = myGUI->particleLife;
    vEmitter.lifeSpread = 5.0;
    vEmitter.numPars = 1;
    vEmitter.color = ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
    vEmitter.size = myGUI->particleSize;
    
    xEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    xEmitter.setVelocity(myGUI->particleVelocity);
    xEmitter.velSpread = myGUI->particleSpread;
    xEmitter.life = myGUI->particleLife;
    xEmitter.lifeSpread = 5.0;
    xEmitter.numPars = 1;
    xEmitter.color = ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
    xEmitter.size = myGUI->particleSize;
    
    ofLoadImage(pTex, "resources/particle.png");
}

void XBScene1::initStones(){
    svg.load("resources/Esc1Piano.svg");
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

void XBScene1::initWaves()
{
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    int spacing = 10;
    
    // create horzontal waves
    svg.load("resources/horizontalesv02.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        //cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
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

void XBScene1::updateEmitters(){
    XBScene1GUI *myGUI = (XBScene1GUI *)gui;
    
    //GUI related
    vEmitter.size = myGUI->particleSize;
    vEmitter.setVelocity(myGUI->particleVelocity);
    vEmitter.velSpread = myGUI->particleSpread;
    vEmitter.life = myGUI->particleLife;
    vEmitter.color.set(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));
    
    xEmitter.size = myGUI->particleSize;
    xEmitter.setVelocity(myGUI->particleVelocity);
    xEmitter.velSpread = myGUI->particleSpread;
    xEmitter.life = myGUI->particleLife;
    xEmitter.color.set(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));
    
    // update particle system
    float dt = min(ofGetLastFrameTime(), 1.0/10.0);
    particleSystem.update(dt, 1.);
    
    
}
