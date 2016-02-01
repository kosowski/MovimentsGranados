//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene2.h"
#include "XBScene2GUI.h"

#define NODE_SIZE 6
#define GRID_X_RES 40
#define GRID_Y_RES 40


void XBScene2::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    auxFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB, 1);
    auxFbo.begin();
    ofClear(0);
    auxFbo.end();

    initWaves();
    rectMask.load("resources/img/Esc2Barra_v01.png");
    initWindows("resources/Esc2Cello.svg", celloWindows, celloWaves, 2, 2);
    initWindows("resources/Esc2Piano.svg", pianoWindows, pianoWaves, 2, 2);
    initWindows("resources/Esc2Violinv02.svg", violinWindows, violinWaves, 1, 10);
    
    initWindowsOutlines("resources/Esc2Cello.svg", celloOutlines);
    initWindowsOutlines("resources/Esc2Piano.svg", pianoOutlines);
    
    windowMask.load("resources/img/Esc2_fade_Completo_v2.png");
    
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0);
}


void XBScene2::update()
{
    XBBaseScene::update();

    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    // update waves
    if(myGUI->simulateHands){
        rightHand.pos.x = ofGetMouseX() / (float) ofGetWidth();
        rightHand.pos.y = ofGetMouseY() / (float) ofGetHeight();
        leftHand.pos.x = (rightHand.pos.x - 0.5)  + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005) - 0.5);
        leftHand.pos.y = rightHand.pos.y  + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005 + 1000) - 0.5);
    }
    for (int i = 0; i < waves.size(); i++) {
        // if simulate mode ON, use the mouse
        waves[i].setAttractor(0, rightHand.pos.x * MAIN_WINDOW_WIDTH, rightHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius);
        waves[i].setAttractor(1, leftHand.pos.x * MAIN_WINDOW_WIDTH, leftHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius);
        waves[i].update();
    }
    
    // update cello windows
    for (int i = 0; i < celloOutlinesToDraw.size(); i++) {
        celloOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (celloOutlinesToDraw[i].life * myGUI->alphaFactor > 255) {
            celloOutlinesToDraw.erase(celloOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }
    
    // update piano windows
    for (int i = 0; i < pianoOutlinesToDraw.size(); i++) {
        pianoOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (pianoOutlinesToDraw[i].life * myGUI->alphaFactor > 255) {
            pianoOutlinesToDraw.erase(pianoOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }
}

void XBScene2::drawIntoFBO()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;

    auxFbo.begin();
    {
        ofPushMatrix();
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);

        ofBackground(0);

        //draw cello windows
        if (fakeCelloEvent || celloEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
            int windowIndex = drawWindow(celloNote, celloWindows, celloWaves);
            if(ofGetFrameNum() % myGUI->windowFrequency == 0){
                if(windowIndex == 2){ // if third floor, light up two windows
                    celloOutlinesToDraw.push_back(celloOutlines[windowIndex+1]);
                }
                else if(windowIndex==3) // if fourth floor, increment one to skip double window
                    windowIndex++;
                celloOutlinesToDraw.push_back(celloOutlines[windowIndex]);
            }
            ofPopStyle();
        }

        //draw piano windows
        if (fakePianoEvent || pianoEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, myGUI->colorPianoA);
             int windowIndex = drawWindow(pianoNote, pianoWindows, pianoWaves);
            if(ofGetFrameNum() % myGUI->windowFrequency == 0){
                if(windowIndex == 2){ // if third floor, light up two windows
                    pianoOutlinesToDraw.push_back(pianoOutlines[windowIndex+1]);
                }
                else if(windowIndex==3) // if fourth floor, increment one to skip double window
                    windowIndex++;
                pianoOutlinesToDraw.push_back(pianoOutlines[windowIndex]);
            }
            ofPopStyle();
        }

        //draw violin windows
        if (fakeViolinEvent || violinEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
            drawWindow(violinNote, violinWindows, violinWaves);
            ofPopStyle();
        }
        
        // mask windows outsides
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        windowMask.draw(0, 0);
        ofPopStyle();

        ofPopMatrix();
    }
    auxFbo.end();

    fbo.begin();
    {
        ofPushMatrix();
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);

        if (showFacadeImage)
            templateImage.draw(0, 0);
        else
            ofBackground(0);
        
        // draw directors waves
        ofPushStyle();
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        ofSetLineWidth(myGUI->lineWidth);
        for (Wave w:waves)
            w.display();
        ofPopStyle();
        
        // draw expanding piano windows
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < pianoOutlinesToDraw.size(); i++) {
            expandingPolyLine &e = pianoOutlinesToDraw[i];
            ofPushMatrix();
            ofTranslate(e.centroid);
            ofScale(1 + e.life * myGUI->growFactor , 1 + e.life * myGUI->growFactor);
            e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA * myGUI->alphaStart - e.life * myGUI->alphaFactor, 0, 255)));
            e.path.draw();
            ofPopMatrix();
        }
        ofPopStyle();
        
        // draw expanding cello windows
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < celloOutlinesToDraw.size(); i++) {
            expandingPolyLine &e = celloOutlinesToDraw[i];
            ofPushMatrix();
            ofTranslate(e.centroid);
            ofScale(1 + e.life  * myGUI->growFactor, 1 + e.life * myGUI->growFactor);
            e.path.setFillColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, ofClamp(myGUI->colorCelloA * myGUI->alphaStart - e.life * myGUI->alphaFactor , 0, 255)));
            e.path.draw();
            ofPopMatrix();
        }
        ofPopStyle();

        // apply mask to remove windows interiors
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();
        
        ofPopMatrix();
        
        // add fbo with windows contents
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        auxFbo.draw(0, 0);
        ofPopStyle();

        drawMusiciansWindows();
        
        drawGUI();
        drawFadeRectangle();


    }
    fbo.end();
    
    blur.apply(&fbo, myGUI->blurAmount, 1);
    
    applyPostFX();
}

int XBScene2::drawWindow(float note, vector<ofRectangle> &windows, vector<SimpleWave> &waves)
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;

    int currentWindow = 0;
    float mappedPitch;
    if (note < 0.25) {
        currentWindow = 0;
        mappedPitch = ofMap(note, 0, 0.25, 0, 1);
    }
    else if (note >= 0.25 && note < 0.5) {
        currentWindow = 1;
        mappedPitch = ofMap(note, 0.25, 0.5, 0, 1);
    }
    else if (note >= 0.5 && note < 0.75) {
        currentWindow = 2;
        mappedPitch = ofMap(note, 0.5, 0.75, 0, 1);
    }
    else if (note >= 0.75) {
        currentWindow = 3;
        mappedPitch = ofMap(note, 0.75, 1, 0, 1);
    }
    ofRectangle window = windows[currentWindow];
    ofFill();
    float y = ofMap(mappedPitch, 0, 1, window.getMaxY(), window.getMinY());

    rectMask.draw(window.x, y - myGUI->barHeight / 2, window.width, myGUI->barHeight);
//    ofPushStyle();
//    ofSetLineWidth(4);
//    waves[currentWindow].update();
//    ofPushMatrix();
//    ofTranslate(0,y);
//    waves[currentWindow].display();
//    ofPopMatrix();
//    ofPopStyle();
    return currentWindow;
}

//--------------------------------------------------------------
void XBScene2::onViolinPitchChanged(float &pitch)
{
    if(!active)
        return;
    violinNote = pitch;
}

void XBScene2::onViolinEnergyChanged(float &energy)
{
    if(!active)
        return;
    if (energy <= energyThreshold)
        violinEnergy = 0;
    else
        violinEnergy = energy;
}

void XBScene2::onCelloPitchChanged(float &pitch)
{
    if(!active)
        return;
    celloNote = pitch;
}

void XBScene2::onCelloEnergyChanged(float &energy)
{
    if(!active)
        return;
    if (energy <= energyThreshold)
        celloEnergy = 0;
    else
        celloEnergy = energy;
}

void XBScene2::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn)
{
    if(!active)
        return;
//    cout << "Piano NoteOn:  p=" << noteOn.pitch << " v=" << noteOn.velocity << endl;
    pianoNote = noteOn.pitch / MAX_MIDI_VALUE;
    pianoEnergy = noteOn.velocity / MAX_MIDI_VALUE;
}

void XBScene2::onPianoNoteOff(int &noteOff)
{
//    cout << "Piano NoteOff: p=" << noteOff << endl;
    pianoEnergy = 0;
}

void XBScene2::onKinectLPositionChanged(XBOSCManager::KinectPosVelArgs &lPos) {
    leftHand.pos.set(lPos.x, lPos.y, lPos.z);
}
void XBScene2::onKinectLVelocityChanged(XBOSCManager::KinectPosVelArgs &lVel){
    leftHand.velocity.set(lVel.x, lVel.y, lVel.z);
}
void XBScene2::onKinectRPositionChanged(XBOSCManager::KinectPosVelArgs &rPos){
    rightHand.pos.set(rPos.x, rPos.y, rPos.z);
}
void XBScene2::onKinectRVelocityChanged(XBOSCManager::KinectPosVelArgs &rVel){
    rightHand.velocity.set(rVel.x, rVel.y, rVel.z);
}

//--------------------------------------------------------------
void XBScene2::keyPressed(int key)
{
    switch(key)
    {
        case 'c':
        case 'C':
        {
            if (!fakeCelloEvent) {
                fakeCelloEvent = true;
                celloNote = ofRandom(1);
            }
            break;
        }
        case 'v':
        case 'V':
        {
            if (!fakePianoEvent) {
                fakePianoEvent = true;
                pianoNote = ofRandom(1);
            }
            break;
        }
        case 'x':
        case 'X':
        {
            if (!fakeViolinEvent) {
                fakeViolinEvent = true;
                violinNote = ofRandom(1);
            }
            break;
        }
        default: break;
    }
}

void XBScene2::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    switch (key) {
        case 'z':
        case 'Z': {
            waves.clear();
            initWaves();
            break;
        }
        case 'c':
        case 'C':
            fakeCelloEvent = false;
            break;
        case 'v':
        case 'V':
            fakePianoEvent = false;
            break;
        case 'x':
        case 'X':
            fakeViolinEvent = false;
            break;
        default:
            break;
    }
}


void XBScene2::initWindows(string name, vector<ofRectangle> &vectorWindows, vector<SimpleWave> &vectorWaves,int startIndex, int arrangFloor)
{
    svg.load(name);
//    int startIndex = 2; //skip full frame and first balcony
    for (int i = startIndex; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero win/Users/nacho/Downloads/Esc2Cello.svgding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline pl = lines[j].getResampledBySpacing(6);
            pl.close();
            vectorWindows.push_back(pl.getBoundingBox());
        }
    }
    arrangeWindows(arrangFloor, vectorWindows);
    // crear wave para cada rectangulo
//    for(int i=0; i< vectorWindows.size();i++){
//        ofPoint o = vectorWindows[i].getTopLeft();
//        o.y = 0;
//        int w =vectorWindows[i].width;
//        SimpleWave sw(o, w/2 + 10, 20.f, ofRandom(180, 220.));
//        vectorWaves.push_back(sw);
//    }
}

void XBScene2::arrangeWindows(int indexToMerge, vector<ofRectangle> &elements)
{
    vector<ofRectangle> arranged;
    for (int i = 0; i < elements.size(); i++) {
        if (i == indexToMerge) {
            ofRectangle r3 = elements[i];
            ofRectangle r4 = elements[i + 1];
            float xMin = r3.getMinX() < r4.getMinX() ? r3.getMinX() : r4.getMinX();
            float yMin = r3.getMinY() < r4.getMinY() ? r3.getMinY() : r4.getMinY();
            float xMax = r3.getMaxX() > r4.getMaxX() ? r3.getMaxX() : r4.getMaxX();
            float yMax = r3.getMaxY() > r4.getMaxY() ? r3.getMaxY() : r4.getMaxY();
            ofRectangle combined(xMin, yMin, xMax - xMin, yMax - yMin);
            arranged.push_back(combined);
            i++;
        }
        else
            arranged.push_back(elements[i]);
    }
    elements.clear();
    elements = arranged;
}

void XBScene2::initWindowsOutlines(string name, vector<expandingPolyLine> &vectorWindows){
    svg.load(name);
    for (int i = 2; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());
        
        // for every line create a shape centered at zero and store its centroid
        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline pl = lines[j].getResampledBySpacing(6);
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
            vectorWindows.push_back(epl);
        }
    }
}

void XBScene2::initWaves()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    int spacing = 10;
    
    // create horzontal waves
    svg.load("resources/horizontalesv03.svg");
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
    svg.load("resources/verticales_v03_pocas_lineas.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++) {
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

