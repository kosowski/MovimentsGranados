//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene2.h"
#include "XBScene2GUI.h"
#include "XBSettingsManager.h"

#define NODE_SIZE 6
#define GRID_X_RES 40
#define GRID_Y_RES 40


void XBScene2::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    violinFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB, 1);
    violinFbo.begin();
    ofClear(0);
    violinFbo.end();

    celloPianoFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB, 1);
    celloPianoFbo.begin();
    ofClear(0);
    celloPianoFbo.end();

    rectMask.load("resources/img/Esc2Barra_v01.png");
    initWindows("resources/Esc2Cello.svg", celloWindows, celloWaves, 2);
    arrangeWindows(2, celloWindows);
    initWindows("resources/Esc2Piano.svg", pianoWindows, pianoWaves, 2);
    arrangeWindows(2, violinWindows);
    initWindows("resources/Esc2Violinv02.svg", violinWindows, violinWaves, 1);
    arrangeViolinWindows();
    
    initWindowsOutlines("resources/Esc2Cello.svg", celloOutlines, 2);
    initWindowsOutlines("resources/Esc2Piano.svg", pianoOutlines, 2);
    initWindowsOutlines("resources/Esc2Violin.svg", violinOutlines, 3);

    initWaves();
    initStones();
    
    celloPianoMask.load("resources/img/Esc2CelloPiano.png");
    violinMask.load("resources/img/Esc2Violinv03_fade.png");
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0);
}

void XBScene2::update()
{
    XBBaseScene::update();
    updateDirector();
    updateCello();
    updatePiano();
    updateViolin();
}

void XBScene2::drawIntoFBO()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    fbo.begin();
    {
        ofPushMatrix();
        ofScale(windowScale, windowScale);

        if (showFacadeImage)
            templateImage.draw(0, 0);
        else
            ofBackground(0);

        drawDirector();
        drawPiano();
        drawCello();
        drawViolin();

        // apply mask to remove windows interiors
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();

        ofPopMatrix();

        // add fbo with piano and cello windows interiors
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        celloPianoFbo.draw(0, 0);
        ofPopStyle();

        // add violin fbo
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        violinFbo.draw(0, 0);
        ofPopStyle();

        drawMusiciansWindows();

        drawGUI();
//        drawFadeRectangle();
    }
    fbo.end();
    blur.apply(&fbo, 1, myGUI->blurAmount);
    applyPostFX();
}

void XBScene2::updateCello()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    // update cello windows
    for (int i = 0; i < celloOutlinesToDraw.size(); i++) {
        celloOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (celloOutlinesToDraw[i].life * myGUI->alphaFactor > 255) {
            celloOutlinesToDraw.erase(celloOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }

    // draw cello and piano windows in their fbo
    celloPianoFbo.begin();
    {
        ofPushMatrix();
        ofScale(windowScale, windowScale);
        ofBackground(0);
        //draw cello windows
        if (fakeCelloEvent || celloEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
            int windowIndex = drawWindow(celloNote, celloWindows, celloWaves, 4);
            if (ofGetFrameNum() % myGUI->windowFrequency == 0) {
                if (windowIndex == 2) { // if third floor, light up two windows
                    celloOutlinesToDraw.push_back(celloOutlines[windowIndex + 1]);
                }
                else if (windowIndex == 3) // if fourth floor, increment one to skip double window
                    windowIndex++;
                celloOutlinesToDraw.push_back(celloOutlines[windowIndex]);
            }
            ofPopStyle();
        }
        //draw piano windows
        if (fakePianoEvent || pianoEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, myGUI->colorPianoA);
            int windowIndex = drawWindow(pianoNote, pianoWindows, pianoWaves, 4);
            if (ofGetFrameNum() % myGUI->windowFrequency == 0) {
                if (windowIndex == 2) { // if third floor, light up two windows
                    pianoOutlinesToDraw.push_back(pianoOutlines[windowIndex + 1]);
                }
                else if (windowIndex == 3) // if fourth floor, increment one to skip double window
                    windowIndex++;
                pianoOutlinesToDraw.push_back(pianoOutlines[windowIndex]);
            }
            ofPopStyle();
        }

        // mask windows outsides
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        celloPianoMask.draw(0, 0);
        ofPopStyle();
        ofPopMatrix();
    }
    celloPianoFbo.end();
}

void XBScene2::updateViolin()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    // update violin windows
    for (int i = 0; i < violinOutlinesToDraw.size(); i++) {
        violinOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (violinOutlinesToDraw[i].life * myGUI->alphaFactor > 255) {
            violinOutlinesToDraw.erase(violinOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }

    // draw violin windows into its fbo
    violinFbo.begin();
    {
        ofPushMatrix();
        ofScale(windowScale, windowScale);
        ofBackground(0);

        //draw violin windows
        if (fakeViolinEvent || violinEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
            int windowIndex = drawViolinWindow(violinNote, violinWindows, violinWaves, 4);
            if (ofGetFrameNum() % myGUI->windowFrequency == 0)
                violinOutlinesToDraw.push_back(violinOutlines[windowIndex]);
            ofPopStyle();
        }

        // mask windows outsides
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        violinMask.draw(0, 0);
        ofPopStyle();
        ofPopMatrix();
    }
    violinFbo.end();
}

void XBScene2::updatePiano()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    // update piano windows
    for (int i = 0; i < pianoOutlinesToDraw.size(); i++) {
        pianoOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (pianoOutlinesToDraw[i].life * myGUI->alphaFactor > 255) {
            pianoOutlinesToDraw.erase(pianoOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }
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

void XBScene2::updateDirector()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    // update waves
    if (myGUI->simulateHands) {
        rightHand.pos.x = ofGetMouseX() / (float) ofGetWidth();
        rightHand.pos.y = ofGetMouseY() / (float) ofGetHeight();
        leftHand.pos.x = (rightHand.pos.x - 0.5) + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005) - 0.5);
        leftHand.pos.y = rightHand.pos.y + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005 + 1000) - 0.5);
    }
    for (int i = 0; i < waves.size(); i++) {
        // if simulate mode ON, use the mouse
        waves[i].setAttractor(0, rightHand.pos.x * MAIN_WINDOW_WIDTH, rightHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius, myGUI->dampingWaves);
        waves[i].setAttractor(1, leftHand.pos.x * MAIN_WINDOW_WIDTH, leftHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius, myGUI->dampingWaves);
        waves[i].update();
    }
}

void XBScene2::drawDirector()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    // draw directors waves

    float directorAlpha = (fboAlpha < 255.0) ? ofMap(fboAlpha, 0, 255, 0, myGUI->colorDirectorA, true) : myGUI->colorDirectorA;

    ofPushStyle();
    ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, directorAlpha);
    ofSetLineWidth(myGUI->lineWidth);
    for (Wave w:waves)
        w.display();
    ofPopStyle();
}

void XBScene2::drawPiano()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    // draw expanding piano windows
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < pianoOutlinesToDraw.size(); i++) {
        expandingPolyLine &e = pianoOutlinesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        ofScale(1 + e.life * myGUI->growFactor, 1 + e.life * myGUI->growFactor);
        e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA * myGUI->alphaStart - e.life * myGUI->alphaFactor, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
    ofPopStyle();
    
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

void XBScene2::drawCello()
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;

    // draw expanding cello windows
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < celloOutlinesToDraw.size(); i++) {
        expandingPolyLine &e = celloOutlinesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        ofScale(1 + e.life * myGUI->growFactor, 1 + e.life * myGUI->growFactor);
        e.path.setFillColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, ofClamp(myGUI->colorCelloA * myGUI->alphaStart - e.life * myGUI->alphaFactor, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
    ofPopStyle();
}

void XBScene2::drawViolin(){
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    // draw expanding violin windows
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (int i = 0; i < violinOutlinesToDraw.size(); i++) {
        expandingPolyLine &e = violinOutlinesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        ofScale(1 + e.life * myGUI->growFactor, 1 + e.life * myGUI->growFactor);
        e.path.setFillColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, ofClamp(myGUI->colorViolinA * myGUI->alphaStart - e.life * myGUI->alphaFactor, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
    ofPopStyle();
}

int XBScene2::drawWindow(float note, vector<ofRectangle> &windows, vector<SimpleWave> &waves, int floors)
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    
    int currentWindow = 0;
    float mappedPitch;
    
    float divisionSize = 1./ floors;
    currentWindow = (int) (note / divisionSize);
    if(currentWindow >= floors) currentWindow = floors -1;
    mappedPitch = ofMap(note, currentWindow * divisionSize, (currentWindow + 1)* divisionSize, 0, 1);
    
    ofRectangle window = windows[currentWindow];
    ofFill();
    float y = ofMap(mappedPitch, 0, 1, window.getMaxY(), window.getMinY());
    
    rectMask.draw(window.x, y - myGUI->barHeight / 2, window.width, myGUI->barHeight);
    
    return currentWindow;
}

int XBScene2::drawViolinWindow(float note, vector<ofRectangle> &windows, vector<SimpleWave> &waves, int floors)
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    
    int currentWindow = 0;
    float mappedPitch;
    
    float divisionSize = 1./ floors;
    currentWindow = (int) (note / divisionSize);
    if(currentWindow >= floors) currentWindow = floors -1;
    mappedPitch = note;
    
    ofRectangle window = windows[0];
    ofFill();
    float y = ofMap(mappedPitch, 0, 1, window.getMaxY(), window.getMinY());
    
    rectMask.draw(window.x, y - myGUI->barHeight / 2, window.width, myGUI->barHeight);
    
    return currentWindow;
}

void XBScene2::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn)
{
    if (!active)
        return;
    XBBaseScene::onPianoNoteOn(noteOn);
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    
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
void XBScene2::keyPressed(int key)
{
    XBScene2GUI *myGUI = (XBScene2GUI *) gui;
    switch (key) {
        case 'c':
        case 'C': {
            if (!fakeCelloEvent) {
                fakeCelloEvent = true;
                celloNote = ofRandom(1);
            }
            break;
        }
        case 'v':
        case 'V': {
            if (!fakePianoEvent) {
                fakePianoEvent = true;
                pianoNote = ofRandom(1);
            }
            int index = midiToRowMapping[(int) ofRandom(127)];
            expandingPolyLine e = stones[index];
            e.life = 1;
            e.amplitude = myGUI->stoneGrowFactor;
            stonesToDraw.push_back(e);
            break;
        }
        case 'x':
        case 'X': {
            if (!fakeViolinEvent) {
                fakeViolinEvent = true;
                violinNote = ofRandom(1);
            }
            break;
        }
        default:
            break;
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


void XBScene2::initWindows(string name, vector<ofRectangle> &vectorWindows, vector<SimpleWave> &vectorWaves, int startIndex)
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

void XBScene2::initWindowsOutlines(string name, vector<expandingPolyLine> &vectorWindows, int startIndex)
{
    svg.load(name);
    for (int i = startIndex; i < svg.getNumPath(); i++) {
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

void XBScene2::initStones()
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

void XBScene2::arrangeViolinWindows(){
    float xMin = violinWindows[0].getMinX();
    float yMin = violinWindows[0].getMinY();
    float xMax = violinWindows[0].getMaxX();
    float yMax = violinWindows[0].getMaxY();
    for (int i = 1; i < violinWindows.size(); i++) {
        ofRectangle r3 = violinWindows[i];
        if( r3.getMinX() < xMin) xMin = r3.getMinX();
        if( r3.getMinY() < yMin) yMin = r3.getMinY();
        if( r3.getMaxX() > xMax) xMax = r3.getMaxX();
        if( r3.getMaxY() > yMax) yMax = r3.getMaxY();
    }
    violinWindows.clear();
    ofRectangle combined(xMin, yMin, xMax - xMin, yMax - yMin);
    violinWindows.push_back(combined);
}
