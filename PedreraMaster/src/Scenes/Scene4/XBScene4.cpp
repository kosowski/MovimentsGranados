//
// Created by nacho on 09/01/16.
//

#include "XBScene4.h"
#include "XBScene4GUI.h"
#include "XBSettingsManager.h"

void XBScene4::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    auxFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB, 1);
    auxFbo.begin();
    ofClear(0);
    auxFbo.end();
    windowMask.load("resources/img/Esc2_fade_Completo_v2.png");

    initWindows("resources/Esc2Cello.svg", celloWindows, 2, 2);
    initWindows("resources/Esc2Piano.svg", violinWindows, 2, 2);

    initWindowsOutlines("resources/Esc2Cello.svg", celloOutlines);
    initWindowsOutlines("resources/Esc2Piano.svg", violinOutlines);

    rectMask.load("resources/img/Esc2Barra_v01.png");
    initWaves();
    initStones();
    initReactionDiffusion();
    
    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 1);
}

void XBScene4::update()
{
    XBBaseScene::update();

    updateDirector();
    updatePiano();
    updateCello();
    updateViolin();
}

void XBScene4::drawIntoFBO()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    updateViolinCelloFbo();

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
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        drawDirector();
        drawPiano();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        drawViolin();
        drawCello();
        
        // mask for removing the windows
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();

        drawViolinWindow();
        drawCelloWindow();

        ofPopMatrix();

        // draw window interiors
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        auxFbo.draw(0, 0);
        ofPopStyle();

        drawMusiciansWindows();

        drawGUI();
        drawFadeRectangle();
    }
    fbo.end();
    blur.apply(&fbo, 1, myGUI->blurAmount);
    applyPostFX();
}

void XBScene4::updateDirector()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;

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
}

void XBScene4::updatePiano()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
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

void XBScene4::updateCello()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    // update cello windows
    for (int i = 0; i < celloOutlinesToDraw.size(); i++) {
        celloOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (celloOutlinesToDraw[i].life * myGUI->growFactor > 1) {
            celloOutlinesToDraw.erase(celloOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }
    grayX.setK(myGUI->violinK);
    grayX.setF(myGUI->violinF);
//    grayX.setK( presets.get(myGUI->presetIndex).kill);
//    grayX.setF(presets.get(myGUI->presetIndex).feed);
    grayX.setPasses((int) ofMap(celloEnergy, 0, 1, myGUI->minCelloSpeed, myGUI->maxCelloSpeed));
    grayX.update();
}

void XBScene4::updateViolin()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    
    // update piano windows
    for (int i = 0; i < violinOutlinesToDraw.size(); i++) {
        violinOutlinesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        if (violinOutlinesToDraw[i].life * myGUI->growFactor > 1) {
            violinOutlinesToDraw.erase(violinOutlinesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }
    
    // update reaction diffusion
    grayV.setK(myGUI->violinK);
    grayV.setF(myGUI->violinF);
//    grayV.setK( presets.get(myGUI->presetIndex).kill);
//    grayV.setF(presets.get(myGUI->presetIndex).feed);
    grayV.setPasses((int) ofMap(violinEnergy, 0, 1, myGUI->minViolinSpeed, myGUI->maxViolinSpeed));
    grayV.update();
}

void XBScene4::updateViolinCelloFbo()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    auxFbo.begin();
    {
        ofPushMatrix();
        ofScale(windowScale, windowScale);

        ofBackground(0);
        //draw cello windows
        if (fakeCelloEvent || celloEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
            int windowIndex = drawWindow(celloNote, celloWindows);
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

        //draw violin windows
        if (fakeViolinEvent || violinEnergy > energyThreshold) {
            ofPushStyle();
            ofSetColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
            int windowIndex = drawWindow(violinNote, violinWindows);
            if (ofGetFrameNum() % myGUI->windowFrequency == 0) {
                if (windowIndex == 2) { // if third floor, light up two windows
                    violinOutlinesToDraw.push_back(violinOutlines[windowIndex + 1]);
                }
                else if (windowIndex == 3) // if fourth floor, increment one to skip double window
                    windowIndex++;
                violinOutlinesToDraw.push_back(violinOutlines[windowIndex]);
            }
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
}

void XBScene4::drawDirector()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    // draw directors waves
    ofPushStyle();
    ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
    ofSetLineWidth(myGUI->lineWidth);
    for (Wave w:waves)
        w.display();
    ofPopStyle();
}

void XBScene4::drawViolin()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    // draw reaction diffusion with shader to convert it to B&W with transparency
    ofPushStyle();
    convertToBW.begin();
    ofSetColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
    grayV.draw(0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    convertToBW.end();
    ofPopStyle();

}

void XBScene4::drawCello()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    ofPushStyle();
    convertToBW.begin();
    ofSetColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
    grayX.draw(0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
    convertToBW.end();
    ofPopStyle();
}

void XBScene4::drawPiano()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    // draw expanding stones from piano
    ofPushStyle();
    for (int i = 0; i < stonesToDraw.size(); i++) {
        expandingPolyLine &e = stonesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        float scale = 1 + e.amplitude * sin(myGUI->stoneFrequency * e.life + myGUI->stonePhase * PI / 2.f);
        ofScale(scale, scale);
        e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA - e.life * myGUI->stoneAlphaDecrease, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
    ofPopStyle();
    pianoEnergy *= gui->pianoDecay;
}

void XBScene4::drawViolinWindow()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    // draw expanding violin windows
    ofPushStyle();
    for (int i = 0; i < violinOutlinesToDraw.size(); i++) {
        expandingPolyLine &e = violinOutlinesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        ofScale(1 - e.life * myGUI->growFactor, 1 - e.life * myGUI->growFactor);
        e.path.setFillColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, ofClamp(myGUI->colorViolinA - e.life * myGUI->alphaFactor, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
    ofPopStyle();
}

void XBScene4::drawCelloWindow()
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;
    // draw expanding cello windows
    ofPushStyle();
    for (int i = 0; i < celloOutlinesToDraw.size(); i++) {
        expandingPolyLine &e = celloOutlinesToDraw[i];
        ofPushMatrix();
        ofTranslate(e.centroid);
        ofScale(1 - e.life * myGUI->growFactor, 1 - e.life * myGUI->growFactor);
        e.path.setFillColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, ofClamp(myGUI->colorCelloA - e.life * myGUI->alphaFactor, 0, 255)));
        e.path.draw();
        ofPopMatrix();
    }
    ofPopStyle();
}

int XBScene4::drawWindow(float note, vector<ofRectangle> &windows)
{
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;

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
//    ofDrawRectangle(window.x, y, window.width, 10);
    rectMask.draw(window.x, y - myGUI->barHeight / 2, window.width, myGUI->barHeight);
    return currentWindow;
}

//--------------------------------------------------------------
void XBScene4::keyPressed(int key)
{
    switch (key) {
        case 'c':
        case 'C': {
            if (!fakeCelloEvent) {
                fakeCelloEvent = true;
                celloNote = ofRandom(1);
            }
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

void XBScene4::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;

    switch (key) {
        case 'z':
        case 'Z': {
//            waves.clear();
//            initWaves();
            initReactionDiffusion();
            break;
        }

        case 'v':
        case 'V': {
            int index = midiToRowMapping[(int) ofRandom(127)];
            expandingPolyLine e = stones[index][(int) ofRandom(stones[index].size() - 1)];
            e.life = 1;
            e.amplitude = myGUI->stoneGrowFactor;
            stonesToDraw.push_back(e);
            break;
        }
        case 'x':
        case 'X':
            fakeViolinEvent = false;
            break;
        case 'c':
        case 'C':
            fakeCelloEvent = false;
            break;
        case 'm':
        case 'M':
            convertToBW.load("resources/shaders/convertToBW");
            break;
    }
}


void XBScene4::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn)
{
    if (!active)
        return;

    XBBaseScene::onPianoNoteOn(noteOn);
    XBScene4GUI *myGUI = (XBScene4GUI *) gui;

    if (noteOn.pitch < 0 || noteOn.pitch > MAX_MIDI_VALUE) {
        cout << "Wrong MIDI note received " << ofToString(noteOn.pitch) << endl;
        return;
    }

    int wichLine = midiToRowMapping[noteOn.pitch];
    expandingPolyLine e = stones[wichLine][(int) ofRandom(stones[wichLine].size() - 1)];
    e.life = 1;
    e.amplitude = myGUI->stoneGrowFactor;
    stonesToDraw.push_back(e);
}



void XBScene4::initReactionDiffusion()
{
    grayX.allocate(ofGetWidth(), ofGetHeight());
    grayV.allocate(ofGetWidth(), ofGetHeight());

    ofImage seed;
    seed.load("resources/img/Esc4Violin.png");
    grayV.begin();
    seed.draw(0, 0, ofGetWidth(), ofGetHeight());
    grayV.end();

    ofImage mask;
    mask.load("resources/img/Esc4Violin_mascara.png");
    grayV.begin(1);
    mask.draw(0, 0, ofGetWidth(), ofGetHeight());
    grayX.end(1);

    seed.load("resources/img/Esc4Cello.png");
    grayX.begin();
    seed.draw(0, 0, ofGetWidth(), ofGetHeight());
    grayX.end();

    mask.load("resources/img/Esc4Cello_mascara.png");
    grayX.begin(1);
    mask.draw(0, 0, ofGetWidth(), ofGetHeight());
    grayX.end(1);

    convertToBW.load("resources/shaders/convertToBW");
}

void XBScene4::initWaves()
{
    // Initialize a wave with starting point, width, amplitude, and period

    XBScene4GUI *myGUI = (XBScene4GUI *) gui;

    int spacing = 10;

    // create horzontal waves
    svg.load("resources/horizontalesv04_pocas_01.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        //        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
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

void XBScene4::initStones()
{
    stones.resize(40);
    svg.load("resources/Esc3y4Pianov02.svg");
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        string pathId = svg.getPathIdAt(i);
        if (pathId.empty())
            continue;
        pathId = pathId.substr(1, 2);
//        cout << "Path " << i << " ID: " << pathId << endl;
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
            stones[ofToInt(pathId) - 1].push_back(epl);
        }
    }
    string filePath = "resources/mapping_38_rows_midi.txt";
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

void XBScene4::initWindows(string name, vector<ofRectangle> &vectorWindows, int startIndex, int arrangFloor)
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
}

void XBScene4::arrangeWindows(int indexToMerge, vector<ofRectangle> &elements)
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

void XBScene4::initWindowsOutlines(string name, vector<expandingPolyLine> &vectorWindows)
{
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

