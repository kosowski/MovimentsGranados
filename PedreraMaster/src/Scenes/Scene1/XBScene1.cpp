//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene1.h"
#include "XBScene1GUI.h"
#include "XBSettingsManager.h"

bool maskWindows = true;
float celloWindowAlpha = 0;
float violinWindowAlpha = 0;

void XBScene1::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    wavesMask.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    wavesMask.begin();
    ofSetBackgroundAuto(false);
    ofBackground(0, 0, 0);
    wavesMask.end();

    initWindows();
    initParticles();
    initWaves();
    initStones();
    initLines();

    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0);
}


void XBScene1::update()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    XBBaseScene::update();

    updateViolin();
    updateCello();
    updateEmitters();
    updatePiano();
    updateDirector();
}

void XBScene1::drawIntoFBO()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    fbo.begin();
    {
        ofPushMatrix();
        ofScale(windowScale, windowScale);
        if (showFacadeImage)
            templateImage.draw(0, 0);
        else
            ofBackground(0);

        if (showTemplate) {
            ofSetColor(255);
            svg.draw();
        }

        drawDirector();
        drawPiano();

        // draw time markers
        if (myGUI->showTimeMarker) {
            ofSetColor(220);
            ofDrawLine(0, violinTimeIndex, ofGetWidth() / windowScale, violinTimeIndex);
            ofDrawLine(celloTimeIndex, 0, celloTimeIndex, ofGetHeight() / windowScale);
        }

        if(myGUI->drawWindows)
            drawWindows();

        // mask for removing the windows
        if (maskWindows == true) {
            ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
            mask.draw(0, 0);
            ofPopStyle();
        }
        drawViolinCello();
        ofPopMatrix();
        drawMusiciansWindows();

        drawGUI();
        drawFadeRectangle();
    }
    fbo.end();
    blur.apply(&fbo, 1, myGUI->blurAmount);
    applyPostFX();
}

void XBScene1::updateViolin()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    // increase time marker
    violinTimeIndex -= myGUI->timeIncrement;
    if (violinTimeIndex < 0){
        violinTimeIndex = ofGetHeight() / windowScale;
        violinLineIndex = (unsigned int) findIntersectionVertical(currentViolinNote, violinTimeIndex);
         if( myGUI->celloVertical)
             celloLineIndex = findIntersectionVertical(currentCelloNote, violinTimeIndex);
    }
    
//    if (violinLineIndex <= 0)
//        currentViolinNote.getClosestPoint(ofPoint(0, violinTimeIndex), &violinLineIndex);

    // if note ON, update emitter location along the current line
    if (fakeEvent || violinEnergy > energyThreshold) {
        //        violinLine.addVertex(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        ofPoint endPoint = currentViolinNote.getPointAtIndexInterpolated(violinLineIndex);
        violinLineIndex += myGUI->timeIncrement;
        vEmitter.setPosition(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex), endPoint);
        particleSystem.addParticles(vEmitter);
    }

    // update fading windows outlines
    for (int i = 0; i < fadingViolinWindowsToDraw.size(); i++) {
        fadingViolinWindowsToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (fadingViolinWindowsToDraw[i].life * myGUI->windowFade > 255) {
            fadingViolinWindowsToDraw.erase(fadingViolinWindowsToDraw.begin() + i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
}

void XBScene1::updateCello()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    // if cello line is horizontal, update the indices, it it isnt, it will be updated in updateVioliln()
    if( !myGUI->celloVertical){
        celloTimeIndex += myGUI->timeIncrement;
        if (celloTimeIndex > ofGetWidth() / windowScale) {
            celloTimeIndex = 0;
            celloLineIndex = findIntersectionHorizontal(currentCelloNote, celloTimeIndex);
        }
    }

    if (fakeCelloEvent || celloEnergy > energyThreshold) {
        //        violinLine.addVertex(currentViolinNote.getPointAtIndexInterpolated(violinLineIndex));
        ofPoint endPoint = currentCelloNote.getPointAtIndexInterpolated(celloLineIndex);
        celloLineIndex += myGUI->timeIncrement;
        xEmitter.setPosition(currentCelloNote.getPointAtIndexInterpolated(celloLineIndex), endPoint);
        particleSystem.addParticles(xEmitter);
    }

    // update fading windows
    for (int i = 0; i < fadingCelloWindowsToDraw.size(); i++) {
        fadingCelloWindowsToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (fadingCelloWindowsToDraw[i].life * myGUI->windowFade > 255) {
            fadingCelloWindowsToDraw.erase(fadingCelloWindowsToDraw.begin() + i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
}

void XBScene1::updatePiano()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    // update piano's stones
    for (int i = 0; i < stonesToDraw.size(); i++) {
        stonesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        stonesToDraw[i].amplitude *= myGUI->stoneDamping;
        if (stonesToDraw[i].life > ofGetFrameRate() * myGUI->stoneTime) {
            stonesToDraw.erase(stonesToDraw.begin() + i); // fixed this erase call
            i--; // new code to keep i index valid
        }
    }
    pianoEnergy *= gui->pianoDecay;
    if(ofGetElapsedTimeMillis() - lastPianoNoteTime > 300)
        pianoEnergy *= 0.8;
}

void XBScene1::updateDirector()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    // update waves
    if (myGUI->simulateHands) {
        rightHand.pos.x = ofGetMouseX() / (float) ofGetWidth();
        rightHand.pos.y = ofGetMouseY() / (float) ofGetHeight();
        leftHand.pos.x = (rightHand.pos.x - 0.5) + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005) - 0.5);
        leftHand.pos.y = rightHand.pos.y + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005 + 1000) - 0.5);
    }
    for (int i = 0; i < waves.size(); i++) {
        waves[i].setAttractor(0, rightHand.pos.x * MAIN_WINDOW_WIDTH, rightHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius, myGUI->dampingWaves);
        waves[i].setAttractor(1, leftHand.pos.x * MAIN_WINDOW_WIDTH, leftHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius, myGUI->dampingWaves);
        waves[i].update();
    }
    // update waves mask
    wavesMask.begin();
    ofSetColor(0, 0, 0, myGUI->maskAlpha);
    ofDrawRectangle(0, 0, wavesMask.getWidth(), wavesMask.getHeight());
    ofPushMatrix();
    ofSetColor(255);
    pTex.setAnchorPercent(0.5, 0.5);
    pTex.draw(rightHand.pos.x * wavesMask.getWidth(), rightHand.pos.y * wavesMask.getHeight(), myGUI->maskRadius, myGUI->maskRadius);
    pTex.draw(leftHand.pos.x * wavesMask.getWidth(), leftHand.pos.y * wavesMask.getHeight(), myGUI->maskRadius, myGUI->maskRadius);
    ofPopMatrix();
    wavesMask.end();
}

void XBScene1::drawDirector()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    // draw directors waves
    ofPushStyle();
    ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
    ofSetLineWidth(myGUI->lineWidth);
    for (Wave w:waves)
        w.display();
    ofPopStyle();

    // apply director mask
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    wavesMask.draw(0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    ofPopStyle();
}

void XBScene1::drawPiano()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    // draw expanding stones from piano
    for (int i = 0; i < stonesToDraw.size(); i++) {
        expandingPolyLine &e = stonesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        //             ofScale(e.life * myGUI->stoneGrowFactor, e.life * myGUI->stoneGrowFactor);
        ofScale(1 + e.amplitude * sin(myGUI->stoneFrequency * e.life),
                1 + e.amplitude * sin(myGUI->stoneFrequency * e.life));
        e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA - e.life * myGUI->stoneAlphaDecrease, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
}

void XBScene1::drawViolinCello()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    //draw particles from violin and cello
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    pTex.ofBaseDraws::setAnchorPercent(0.5, 0.5);
    particleSystem.draw(pTex);
    //        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofPopStyle();

    // draw head of violin and cello lines
    int radius = myGUI->headSize;
    ofPushStyle();
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    ofTranslate(xEmitter.positionStart.x, xEmitter.positionStart.y);
    ofSetColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA * celloEnergy));
    headTexture.setAnchorPercent(0.5, 0.5);
    headTexture.draw(0, 0, myGUI->particleSize + radius * celloEnergy, myGUI->particleSize + radius * celloEnergy);
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate(vEmitter.positionStart.x, vEmitter.positionStart.y);
    ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA * violinEnergy));
    headTexture.draw(0, 0, myGUI->particleSize + radius * violinEnergy, myGUI->particleSize + radius * violinEnergy);

    ofPopMatrix();
    ofPopStyle();
}

void XBScene1::drawWindows()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    // if cello is under a window, paint it
    if (fakeCelloEvent || celloEnergy > energyThreshold) {
        for (int i = 0; i < windowsOutlines.size(); i++) {
            if (windowsOutlines[i].inside(xEmitter.positionStart)) {
                if(celloInsideWindow != i)
                    celloWindowAlpha = 0;
                else
                    celloWindowAlpha+= myGUI->windowAttack;
                if(celloWindowAlpha > 1) celloWindowAlpha = 1;
                ofPushStyle();
                ofSetLineWidth(4);
                ofSetColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA * celloWindowAlpha));
                windowsOutlines[i].draw();
                ofPopStyle();
                //                    if(celloInsideWindow != -1 && celloInsideWindow != i)
                //    cout << "Fade en otra ventana" << endl;
                celloInsideWindow = i;
                break;
            }
            // reached the end of the loop, wich means is not under any window
            if (i == windowsOutlines.size() - 1) {
                if (celloInsideWindow > -1)
                    addFadingWindow(celloInsideWindow, fadingCelloWindowsToDraw, celloWindowAlpha, myGUI->colorCelloA);
                celloInsideWindow = -1;
            }
        }
    }
    else {
        if (celloInsideWindow > -1)
            addFadingWindow(celloInsideWindow, fadingCelloWindowsToDraw, celloWindowAlpha, myGUI->colorCelloA);
        celloInsideWindow = -1;
    }

    // if violin is under a window, paint it
    if (fakeEvent || violinEnergy > energyThreshold) {
        for (int i = 0; i < windowsOutlines.size(); i++) {
            if (windowsOutlines[i].inside(vEmitter.positionStart)) {
                if(violinInsideWindow != i)
                    violinWindowAlpha = 0;
                else
                    violinWindowAlpha+= myGUI->windowAttack;
                if(violinWindowAlpha > 1) violinWindowAlpha = 1;
                ofPushStyle();
                ofSetLineWidth(4);
                ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA * violinWindowAlpha));
                windowsOutlines[i].draw();
                ofPopStyle();
                violinInsideWindow = i;
                break;
            }
            // reached the end of the loop, wich means is not under any window
            if (i == windowsOutlines.size() - 1) {
                if (violinInsideWindow > -1) {
                    addFadingWindow(violinInsideWindow, fadingViolinWindowsToDraw, violinWindowAlpha, myGUI->colorViolinA);
                }
                violinInsideWindow = -1;
            }
        }
    }
    else {
        if (violinInsideWindow > -1)
            addFadingWindow(violinInsideWindow, fadingViolinWindowsToDraw, violinWindowAlpha, myGUI->colorViolinA);
        violinInsideWindow = -1;
    }

//    draw fading window outlines
    ofPushStyle();
    ofSetLineWidth(4);
    for (int i = 0; i < fadingViolinWindowsToDraw.size(); i++) {
        expandingPolyLine &e = fadingViolinWindowsToDraw[i];
        ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, ofClamp(myGUI->colorViolinA - e.life * myGUI->windowFade, 0, 255)));
        e.line.draw();
    }

    for (int i = 0; i < fadingCelloWindowsToDraw.size(); i++) {
        expandingPolyLine &e = fadingCelloWindowsToDraw[i];
        ofSetColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, ofClamp(myGUI->colorCelloA - e.life * myGUI->windowFade, 0, 255)));
        e.line.draw();
    }
    ofPopStyle();
}

void XBScene1::addFadingWindow(int index, vector<expandingPolyLine> &vector, float initValue, float instrumentAlpha)
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    float init = instrumentAlpha *( 1 -  initValue) / myGUI->windowFade;
    cout << init << endl;
    expandingPolyLine e;
    e.life = init;
    e.line = windowsOutlines[index];
    vector.push_back(e);
}

//--------------------------------------------------------------

void XBScene1::onViolinPitchChanged(float &pitch)
{
    if (!active)
        return;

    XBBaseScene::onViolinPitchChanged(pitch);
    int wichLine = floor(ofClamp(pitch, 0., 1.) * (verticalLines.size() - 1));
    currentViolinNote = verticalLines[wichLine];
    violinLineIndex = findIntersectionVertical(currentViolinNote, violinTimeIndex);
    //    int wichLine = floor( ofClamp(pitch, 0., 1.) * (horizontalLines.size() - 1));;
    //    currentViolinNote = horizontalLines[wichLine];
    //    violinLineIndex = findIntersectionHorizontal(currentViolinNote, celloTimeIndex);
}


void XBScene1::onCelloPitchChanged(float &pitch)
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    if (!active)
        return;
    XBBaseScene::onCelloPitchChanged(pitch);
    if(myGUI->celloVertical){
        int wichLine = floor(ofClamp(pitch, 0., 1.) * (verticalLines.size() - 1));
        currentCelloNote = verticalLines[wichLine];
        celloLineIndex = findIntersectionVertical(currentCelloNote, violinTimeIndex);
    }
    else{
        int wichLine = floor( ofClamp(pitch, 0., 1.) * (horizontalLines.size() - 1));;
        currentCelloNote = horizontalLines[wichLine];
        celloLineIndex = findIntersectionHorizontal(currentCelloNote, celloTimeIndex);
    }
}

void XBScene1::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn)
{
    if (!active)
        return;
    XBBaseScene::onPianoNoteOn(noteOn);
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    if (noteOn.pitch < 0 || noteOn.pitch > MAX_MIDI_VALUE) {
        cout << "Wrong MIDI note received " << ofToString(noteOn.pitch) << endl;
        return;
    }
    int wichLine = midiToRowMapping[noteOn.pitch];
    expandingPolyLine e = stones[wichLine];
    e.life = 1;
    e.amplitude = myGUI->stoneGrowFactor;
    stonesToDraw.push_back(e);
}


//--------------------------------------------------------------
int XBScene1::findIntersectionVertical(ofPolyline &line, int posY)
{
    //TODO get closer at first, check the posY with the length of the line, so if it is higher than half the length we start comparing at the middle of the line
    //loop through polyline points, when posY changes from lower to higher from the y component of the polyline point
    vector<ofPoint> vertices = line.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        if (posY < vertices[i].y)
            continue;
        else
            return i;
    }
}

int XBScene1::findIntersectionHorizontal(ofPolyline &line, int posX)
{
    //loop through polyline points, left to right, when posX changes from  higher to lower from the x component of the polyline point
    vector<ofPoint> vertices = line.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        if (posX > vertices[i].x)
            continue;
        else
            return i;
    }
}

//--------------------------------------------------------------
void XBScene1::keyPressed(int key)
{
    XBBaseScene::keyPressed(key);
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    switch (key) {
        case 'x':
        case 'X': {
            if (!fakeEvent) {
                fakeEvent = true;
                int wichLine = (int) ofRandom(verticalLines.size());
                currentViolinNote = verticalLines[wichLine];
                violinLineIndex = (unsigned int) findIntersectionVertical(currentViolinNote, violinTimeIndex);
            }
            break;
        }
        case 'c':
        case 'C': {
            if (!fakeCelloEvent) {
                fakeCelloEvent = true;
                if(myGUI->celloVertical){
                    int wichLine = (int) ofRandom(verticalLines.size());
                    currentCelloNote = verticalLines[wichLine];
                    celloLineIndex = findIntersectionVertical(currentCelloNote, violinTimeIndex);
                }
                else{
                    int wichLine = (int) ofRandom(horizontalLines.size());
                    currentCelloNote = horizontalLines[wichLine];
                    celloLineIndex = findIntersectionHorizontal(currentCelloNote, celloTimeIndex);
                }
            }
            break;
        }
        case 'v':
        case 'V': {
            int index = midiToRowMapping[(int) ofRandom(127)];
            expandingPolyLine e = stones[index];
            e.life = 1;
            e.amplitude = myGUI->stoneGrowFactor;
            stonesToDraw.push_back(e);
            break;
        }
        case 'm':
        case 'M': {
            maskWindows = !maskWindows;
            break;
        }
        default:
            break;
    }
}

void XBScene1::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    switch (key) {
        case 'z':
        case 'Z': {
            waves.clear();
            initWaves();
        }
        case 'x':
        case 'X': {
            fakeEvent = false;
        }
        case 'c':
        case 'C': {
            fakeCelloEvent = false;
            //            violinLine.clear();
        }

        default:
            break;
    }
}

void XBScene1::initLines()
{
    // LOAD HORIZINTAL LINES
    svg.load("resources/horizontalesv04.svg");
    for (int i = 1; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline pl = lines[j].getResampledBySpacing(1);
            vector<ofPoint> points = pl.getVertices();
            //check path direction
            if (points.size() > 51) {
                if (points[0].x > points[50].x) { //check if the order is right to left, we dont want that
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
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

//        for (int j = 0; j < (int) lines.size(); j++) {
        int j = 0; // take only the first one and skip erroneous data
            ofPolyline pl = lines[j].getResampledBySpacing(1);
            vector<ofPoint> points = pl.getVertices();
            //check path direction
            if (points.size() > 51) {
                if (points[0].y < points[50].y) { //check if the order is top to bottom, we dont want that
                    std::reverse(points.begin(), points.end());
                    pl.clear();
                    pl.addVertices(points);
                }
            }
            verticalLines.push_back(pl);
//        }
    }
}

void XBScene1::initWindows()
{
    int spacing = 10;
    svg.load("resources/ventanas_v02.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
            l.close();
            windowsOutlines.push_back(l);
        }
    }
}

void XBScene1::initParticles()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    emitParticles = false;
    vEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    vEmitter.setVelocity( ofVec3f(myGUI->particleVelocity->x, myGUI->particleVelocity->y, 0));
    vEmitter.velSpread = ofVec3f(myGUI->particleSpread, myGUI->particleSpread,0);
    vEmitter.life = myGUI->particleLife;
    vEmitter.lifeSpread = 5.0;
    vEmitter.numPars = myGUI->numParticles;
    vEmitter.color = ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
    vEmitter.size = myGUI->particleSize;

    xEmitter.setPosition(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
    xEmitter.setVelocity( ofVec3f(myGUI->particleVelocity->x, myGUI->particleVelocity->y, 0));
    xEmitter.velSpread = ofVec3f(myGUI->particleSpread, myGUI->particleSpread,0);
    xEmitter.life = myGUI->particleLife;
    xEmitter.lifeSpread = 5.0;
    xEmitter.numPars = 20;
    xEmitter.color = ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
    xEmitter.size = myGUI->particleSize;

    ofLoadImage(pTex, "resources/img/particle_2.png");
    pTex.setAnchorPercent(0.5, 0.5);
    
    ofLoadImage(headTexture, "resources/img/particle_3_head.png");
    headTexture.setAnchorPercent(0.5, 0.5);
}

void XBScene1::initStones()
{
    svg.load("resources/Esc1Pianov02.svg");
    //cout << "Scene 1 piano svg has " << ofToString(svg.getNumPath()) << endl;
    for (int i = 1; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        // for every line create a shape centered at zero and store its centroid
        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline pl = lines[j].getResampledBySpacing(10);
            expandingPolyLine epl;
            epl.life = 0;
            epl.centroid = pl.getCentroid2D();
            vector<ofPoint> points = pl.getVertices();
            for (int k = 0; k < points.size(); k++) {
                // store the polyline for now
                epl.line.addVertex(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                // create a path out of the polyline so it can be drawn filled
                if (i == 0) {
                    epl.path.newSubPath();
                    epl.path.moveTo(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                } else {
                    epl.path.lineTo(points[k].x - epl.centroid.x, points[k].y - epl.centroid.y);
                }
            }
            epl.path.close();
            epl.line.close();
            stones.push_back(epl);
        }
    }
    string filePath = "resources/mapping_35_rows_midi.txt";
    //Load file placed in bin/data
    ofFile file(filePath);

    if (!file.exists()) {
        ofLogError("The file " + filePath + " is missing");
    }
    ofBuffer buffer(file);

    //Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        midiToRowMapping.push_back(ofToInt(line));
    }
}

void XBScene1::initWaves()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;
    int spacing = 10;

    // create horzontal waves
    svg.load("resources/horizontalesv04_pocas_01.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        //cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
            waves.push_back(Wave(l.getVertices(), 20, ofRandom(myGUI->minPeriod, myGUI->maxPeriod), spacing, 0));
        }
    }

    // create vertical waves
    svg.load("resources/verticalesv06_pocas_01.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline l(lines[j].getResampledBySpacing(spacing));
            waves.push_back(Wave(l.getVertices(), 20, ofRandom(myGUI->minPeriod, myGUI->maxPeriod), spacing, 1));
        }
    }
}

void XBScene1::updateEmitters()
{
    XBScene1GUI *myGUI = (XBScene1GUI *) gui;

    //GUI related
    vEmitter.size = myGUI->particleSize;
    vEmitter.setVelocity( ofVec3f(myGUI->particleVelocity->x, myGUI->particleVelocity->y, 0));
    vEmitter.velSpread = ofVec3f(myGUI->particleSpread, myGUI->particleSpread,0);
    vEmitter.life = myGUI->particleLife;
    vEmitter.numPars = myGUI->numParticles;
    vEmitter.color.set(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));

    xEmitter.size = myGUI->particleSize;
    xEmitter.setVelocity( ofVec3f(myGUI->particleVelocity->x, myGUI->particleVelocity->y, 0));
    xEmitter.velSpread = ofVec3f(myGUI->particleSpread, myGUI->particleSpread,0);
    xEmitter.life = myGUI->particleLife;
    xEmitter.numPars = myGUI->numParticles;
    xEmitter.color.set(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));

    // update particle system
    float dt = min(ofGetLastFrameTime(), 1.0 / 10.0);
    particleSystem.update(dt, 1.);


}
