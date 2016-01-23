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
    
    initSystem();
    initWindows("resources/Esc2Cello.svg", celloWindows, 2, 2);
    initWindows("resources/Esc2Piano.svg", pianoWindows, 2, 2);
    initWindows("resources/Esc2Violinv02.svg", violinWindows, 1, 10);
    
    windowMask.load("resources/Esc2_fade_Completo.png");
    }



void XBScene2::update()
{
    XBBaseScene::update();

    p_mouse->position.set(ofGetMouseX(), ofGetMouseY(), 0);
    //physics->setGravity( gravityDir.x, gravityDir.y, gravityDir.z );
    physics->tick();
}

void XBScene2::drawIntoFBO()
{
    XBScene2GUI *myGUI = (XBScene2GUI *)gui;

    auxFbo.begin();
    {
        ofPushMatrix();
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        
        ofBackground(0);
        
        //draw cello windows
        if(fakeCelloEvent || celloEnergy > energyThreshold){
            ofPushStyle();
            ofSetColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA);
            drawWindow(celloNote, celloWindows);
            ofPopStyle();
        }
        
        //draw piano windows
        if(fakePianoEvent || pianoEnergy > energyThreshold){
            ofPushStyle();
            ofSetColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, myGUI->colorPianoA);
            drawWindow(pianoNote, pianoWindows);
            ofPopStyle();
        }
        
        //draw violin windows
        if(fakeViolinEvent || violinEnergy > energyThreshold){
            ofPushStyle();
            ofSetColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA);
            drawWindow(violinNote, violinWindows);
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

        if(showFacadeImage)
            templateImage.draw(0,0);
        else
            ofBackground(0);
        // draw springs
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorB, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        for (int i = 0; i < visibleSprings.size(); ++i) {
            Spring *e = visibleSprings[i];
            Particle *a = e->getOneEnd();
            Particle *b = e->getTheOtherEnd();
            ofDrawLine(a->position.x, a->position.y, b->position.x, b->position.y);
        }
        
        // apply mask to remove windows interiors
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();
        ofPopMatrix();
//
//        // add fbo with windows contents
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        auxFbo.draw(0,0);
        ofPopStyle();
        
        drawGUI();
        drawFadeRectangle();
        
        
    }
    fbo.end();
}

void XBScene2::drawWindow(float note, vector<ofRectangle>& windows){
    int currentWindow = 0;
    float mappedPitch;
    if(note < 0.25){
        currentWindow = 0;
        mappedPitch = ofMap(note, 0, 0.25, 0, 1);
    }
    else if(note >= 0.25 && note < 0.5){
        currentWindow = 1;
        mappedPitch = ofMap(note, 0.25, 0.5, 0, 1);
    }
    else if(note >= 0.5 && note < 0.75){
        currentWindow = 2;
        mappedPitch = ofMap(note, 0.5, 0.75, 0, 1);
    }
    else if(note >= 0.75 ){
        currentWindow = 3;
        mappedPitch = ofMap(note, 0.75, 1, 0, 1);
    }
    ofRectangle window = windows[currentWindow];
//    ofNoFill();
//    ofDrawRectangle(window);
    ofFill();
    float y = ofMap(mappedPitch, 0, 1, window.getMaxY(), window.getMinY());
    ofDrawRectangle(window.x, y, window.width, 10);
}

//--------------------------------------------------------------

void XBScene2::onViolinPitchChanged(float &pitch) {
    violinNote = pitch;
}
void XBScene2::onViolinEnergyChanged(float &energy) {
    if(energy <= energyThreshold)
        violinEnergy = 0;
    else
        violinEnergy = energy;
}

void XBScene2::onCelloPitchChanged(float &pitch) {
    celloNote = pitch;
}
void XBScene2::onCelloEnergyChanged(float &energy) {
    if(energy <= energyThreshold)
        celloEnergy = 0;
    else
        celloEnergy = energy;
}

void XBScene2::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn) {
//    cout << "Piano NoteOn:  p=" << noteOn.pitch << " v=" << noteOn.velocity << endl;
    pianoNote = noteOn.pitch;
    pianoEnergy = noteOn.velocity;
}

void XBScene2::onPianoNoteOff(int &noteOff) {
    cout << "Piano NoteOff: p=" << noteOff << endl;
    pianoEnergy = 0;
}



//--------------------------------------------------------------
void XBScene2::keyPressed(int key){
    if(key == 'c'){
        if(fakeCelloEvent == false){
            fakeCelloEvent = true;
            celloNote = ofRandom(1);
        }
    }
    else if(key == 'v'){
        if(fakePianoEvent == false){
            fakePianoEvent = true;
            pianoNote = ofRandom(1);
        }
    }
    else if(key == 'x'){
        if(fakeViolinEvent == false){
            fakeViolinEvent = true;
            violinNote = ofRandom(1);
        }
    }
}

void XBScene2::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    switch (key) {
        case 'z':
        case 'Z': {
            particles.clear();
            visibleSprings.clear();
            delete physics;
            initSystem();
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


void XBScene2::initWindows(string name,  vector<ofRectangle>& vectorWindows, int startIndex, int arrangFloor){
    svg.load(name);
//    int startIndex = 2; //skip full frame and first balcony
    for (int i = startIndex; i < svg.getNumPath(); i++){
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero win/Users/nacho/Downloads/Esc2Cello.svgding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
        
        for(int j=0;j<(int)lines.size();j++){
            ofPolyline pl = lines[j].getResampledBySpacing(6);
            pl.close();
            vectorWindows.push_back(pl.getBoundingBox());
        }
    }
    arrangeWindows(arrangFloor, vectorWindows);
}

void XBScene2::arrangeWindows(int indexToMerge, vector<ofRectangle>& elements){
    vector<ofRectangle> arranged;
    for(int i=0;i< elements.size();i++){
        if(i == indexToMerge){
            ofRectangle r3 = elements[i];
            ofRectangle r4 = elements[i+1];
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

void XBScene2::initSystem()
{
    XBScene2GUI *myGUI = (XBScene2GUI *)gui;
    
    physics = new ParticleSystem(myGUI->gravity, myGUI->drag);
    physics->clear();
    
    p_mouse = physics->makeParticle(1.0, 0.0, 0.0, 0.0);
    p_mouse->setFixed(true);
    
    
    float gridStepX = (float) (ofGetWindowWidth() / MAIN_WINDOW_SCALE / GRID_X_RES);
    float gridStepY = (float) (ofGetWindowHeight() /MAIN_WINDOW_SCALE / GRID_Y_RES);
    
    for (int y = 0; y < GRID_Y_RES; y++)
    {
        for (int x = 0; x < GRID_X_RES; x++)
        {
            ofVec3f pos = ofVec3f(x * gridStepX, y * gridStepY, 0.0f);
            Particle *p = physics->makeParticle(myGUI->particleMass, pos.x, pos.y, pos.z);
            
            //fixed particle to which free particle is attached through a spring
            Particle *fixed = physics->makeParticle(myGUI->particleMass, pos.x, pos.y, pos.z);
            fixed->setFixed(true);
            physics->makeSpring(fixed, p, myGUI->fixedStrength, myGUI->fixedDamping, 2.0);
            
            physics->makeAttraction(p_mouse, p, myGUI->mouseStrength, myGUI->mouseSlope);
            
            particles.push_back(p);
            if (x > 0) {
                Spring *s = physics->makeSpring(particles[y * GRID_X_RES + x - 1], particles[y * GRID_X_RES + x], myGUI->springStrength, myGUI->springDamping, gridStepX);
                visibleSprings.push_back(s);
            }
        }
    }
    
    
    for (int y = 1; y < GRID_Y_RES; y++) {
        for (int x = 0; x < GRID_X_RES; x++) {
            Spring *s = physics->makeSpring(particles[(y - 1) * GRID_X_RES + x], particles[y * GRID_X_RES + x], myGUI->springStrength, myGUI->springDamping, gridStepY);
            visibleSprings.push_back(s);
        }
    }
}
