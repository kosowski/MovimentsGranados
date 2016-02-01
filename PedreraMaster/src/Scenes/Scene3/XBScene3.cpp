//
// Created by Miquel Àngel Soler on 24/12/15.
//

#include "XBScene3.h"
#include "XBScene3GUI.h"


void XBScene3::setup(XBBaseGUI *_gui)
{
    XBBaseScene::setup(_gui);

    int w = 200;
    int h = 250;
    
    // creatures init
    initPaths();
    vPathIndex = 0;//vPath.size()/2;
    xPathIndex = 0;//xPath.size()/2;
    v.setup(vPath.getPointAtIndexInterpolated(vPathIndex).x, vPath.getPointAtIndexInterpolated(vPathIndex).y);
    x.setup(xPath.getPointAtIndexInterpolated(xPathIndex).x, xPath.getPointAtIndexInterpolated(xPathIndex).y);

    initPhysics();
    initWaves();
    initParticles();
    initStones();

    blur.setup(getMainFBO().getWidth(), getMainFBO().getHeight(), 0);
}

void XBScene3::update()
{
    XBBaseScene::update();

    XBScene3GUI *myGUI = (XBScene3GUI *) gui;

    updateVioinCello();

    // update piano's stones
    for (int i = 0; i < stonesToDraw.size(); i++) {
        stonesToDraw[i].life += 1;//myGUI->stoneGrowFactor;
        stonesToDraw[i].amplitude *= myGUI->stoneDamping;
        if (stonesToDraw[i].life > ofGetFrameRate() * myGUI->stoneTime) {
            stonesToDraw.erase(stonesToDraw.begin() + i);
            i--; // keep i index valid
        }
    }

    // update waves
    if(myGUI->simulateHands){
        rightHand.pos.x = ofGetMouseX() / (float) ofGetWidth();
        rightHand.pos.y = ofGetMouseY() / (float) ofGetHeight();
        leftHand.pos.x = (rightHand.pos.x - 0.5)  + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005) - 0.5);
        leftHand.pos.y = rightHand.pos.y  + 0.5 * (ofNoise(ofGetElapsedTimeMillis() * 0.0005 + 1000) - 0.5);
    }
    for (int i = 0; i < waves.size(); i++) {
        waves[i].setAttractor(0, rightHand.pos.x * MAIN_WINDOW_WIDTH, rightHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius);
        waves[i].setAttractor(1, leftHand.pos.x * MAIN_WINDOW_WIDTH, leftHand.pos.y * MAIN_WINDOW_HEIGHT, myGUI->attractorStrength, myGUI->attractorRadius);
        waves[i].update();
    }
}

void XBScene3::drawIntoFBO()
{
    XBScene3GUI *myGUI = (XBScene3GUI *) gui;

    fbo.begin();
    {
        ofPushMatrix();
        ofScale(MAIN_WINDOW_SCALE, MAIN_WINDOW_SCALE);
        if (showFacadeImage)
            templateImage.draw(0, 0);
        else
            ofBackground(0);
        if (showTemplate) {
            ofSetColor(255);
            svg.draw();
        }

        // draw directors waves
        ofPushStyle();
        ofSetColor(myGUI->rgbColorDirectorR, myGUI->rgbColorDirectorG, myGUI->rgbColorDirectorB, myGUI->colorDirectorA);
        ofSetLineWidth(myGUI->lineWidth);
        for (Wave w:waves)
            w.display();
        ofPopStyle();

        ofPushStyle();
        // draw expanding stones from piano
        for (int i = 0; i < stonesToDraw.size(); i++) {
            expandingPolyLine &e = stonesToDraw[i];
            ofPushMatrix();
            ofTranslate(e.centroid);
//             ofScale(e.life * myGUI->stoneGrowFactor, e.life * myGUI->stoneGrowFactor);
            float scale = 0.5 + e.amplitude * sin(myGUI->stoneFrequency * e.life + myGUI->stonePhase * PI/2.f);
            ofScale(scale,scale);
            e.path.setFillColor(ofColor(myGUI->rgbColorPianoR, myGUI->rgbColorPianoG, myGUI->rgbColorPianoB, ofClamp(myGUI->colorPianoA - e.life * myGUI->stoneAlphaDecrease, 0, 255)));
            e.path.draw();
            ofPopMatrix();
        }
        ofPopStyle();

        //draw particles from violin and cello
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < circles.size(); i++) {
            ofFill();
            circles[i].get()->draw(pTex);
        }
        ofPopStyle();

        // draw violin and cello
        if (myGUI->showPath) {
            ofSetColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA));
            vPath.draw();
            ofPoint p = vPath.getPointAtIndexInterpolated(vPathIndex);
            ofSetColor(255);
            ofDrawCircle(p.x, p.y, 10);
            ofSetColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA));
            xPath.draw();
            p = xPath.getPointAtIndexInterpolated(xPathIndex);
            ofSetColor(255);
            ofDrawCircle(p.x, p.y, 10);
        }
        pTex.setAnchorPercent(0.5, 0.5);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        v.draw(pTex);
        x.draw(pTex);
        ofPopStyle();

        // mask for removing the windows
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        mask.draw(0, 0);
        ofPopStyle();
        
        ofPopMatrix();
        
        drawMusiciansWindows();
        
        //draw GUI
        drawGUI();

        drawFadeRectangle();
    }
    fbo.end();

    blur.apply(&fbo, myGUI->blurAmount, 1);
    applyPostFX();
}

void XBScene3::drawGUI()
{
    XBBaseScene::drawGUI();
}

void XBScene3::keyReleased(int key)
{
    XBBaseScene::keyReleased(key);

    XBScene3GUI *myGUI = (XBScene3GUI *) gui;

    switch (key)
    {
        case 'v':
        case 'V':
        {
            int index = midiToRowMapping[(int) ofRandom(127)];
            expandingPolyLine e = stones[index][ (int) ofRandom(stones[index].size() - 1)];
            e.life = 1;
            e.amplitude = myGUI->stoneGrowFactor;
            stonesToDraw.push_back(e);
            break;
        }
        case 'z':
        case 'Z':
        {
            waves.clear();
            initWaves();
            break;
        }
        default: break;
    }
}

//--------------------------------------------------------------
void XBScene3::onViolinPitchChanged(float &pitch)
{
    if(!active)
        return;
    violinNote = pitch;
}

void XBScene3::onViolinEnergyChanged(float &energy)
{
    if(!active)
        return;
    
    if (energy <= energyThreshold)
        violinEnergy = 0;
    else
        violinEnergy = energy;
}

void XBScene3::onCelloPitchChanged(float &pitch)
{
    if(!active)
        return;
    
    celloNote = pitch;
}

void XBScene3::onCelloEnergyChanged(float &energy)
{
    if(!active)
        return;
    
    if (energy <= energyThreshold)
        celloEnergy = 0;
    else
        celloEnergy = energy;
}

void XBScene3::onPianoNoteOn(XBOSCManager::PianoNoteOnArgs &noteOn)
{
    if(!active)
        return;
    
    XBScene3GUI *myGUI = (XBScene3GUI *) gui;
    
    if(noteOn.pitch < 0 || noteOn.pitch > MAX_MIDI_VALUE){
        cout << "Wrong MIDI note received " << ofToString(noteOn.pitch) << endl; return;
    }
    
    int wichLine = midiToRowMapping[noteOn.pitch];
    expandingPolyLine e = stones[wichLine][ (int) ofRandom(stones[wichLine].size() - 1)];
    e.life = 1;
    e.amplitude = myGUI->stoneGrowFactor;
    stonesToDraw.push_back(e);
    pianoEnergy = noteOn.velocity / MAX_MIDI_VALUE;
}

void XBScene3::onPianoNoteOff(int &noteOff)
{
//    cout << "Piano NoteOff: p=" << noteOff << endl;
    pianoEnergy = 0;
}

void XBScene3::onKinectLPositionChanged(XBOSCManager::KinectPosVelArgs &lPos) {
    leftHand.pos.set(lPos.x, lPos.y, lPos.z);
}
void XBScene3::onKinectLVelocityChanged(XBOSCManager::KinectPosVelArgs &lVel){
    leftHand.velocity.set(lVel.x, lVel.y, lVel.z);
}
void XBScene3::onKinectRPositionChanged(XBOSCManager::KinectPosVelArgs &rPos){
    rightHand.pos.set(rPos.x, rPos.y, rPos.z);
}
void XBScene3::onKinectRVelocityChanged(XBOSCManager::KinectPosVelArgs &rVel){
    rightHand.velocity.set(rVel.x, rVel.y, rVel.z);
}

void XBScene3::initPaths()
{
    svg.load("resources/caminosEscena3.svg");
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
//        cout << "Path " << i << " ID: " << svg.getPathIdAt(i) << endl;
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        if (lines.size() > 1)
            cout << "Error en formato caminos escena 3" << endl;

        ofPolyline pl = lines[0].getResampledBySpacing(1);
        if (i == 1)
            vPath = pl;
        else if (i == 2)
            xPath = pl;

    }

}

void XBScene3::initStones()
{
    stones.resize(40);
    svg.load("resources/Esc3y4Pianov02.svg");
    for (int i = 0; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        string pathId = svg.getPathIdAt(i);
        if(pathId.empty())
            continue;
        pathId = pathId.substr(1,2);
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
    
    if(!file.exists()){
        ofLogError("The file " + filePath + " is missing");
    }
    ofBuffer buffer(file);
    
    //Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        midiToRowMapping.push_back(ofToInt(line));
    }
}

void XBScene3::initParticles()
{
    XBScene3GUI *myGUI = (XBScene3GUI *) gui;

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

}

void XBScene3::initWaves()
{
    // Initialize a wave with starting point, width, amplitude, and period

    XBScene3GUI *myGUI = (XBScene3GUI *) gui;

    int spacing = 10;

    // create horzontal waves
    svg.load("resources/horizontalesv02.svg");
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
    svg.load("resources/verticalesv06.svg");
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

void XBScene3::initPhysics()
{
    ofLoadImage(pTex, "resources/particle.png");
    pTex.setAnchorPercent(0.5f, 0.5f);
    // Box2d
    box2d.init();
    box2d.setGravity(0, 2);
    //box2d.createGround();
    box2d.setFPS(60.0);
//    box2d.registerGrabbing();

    int spacing = 40;
    // create horzontal waves
    svg.load("resources/ventanas_v02.svg");
    // start at index 1, as first path uses to be a rectangle with the full frame size
    for (int i = 1; i < svg.getNumPath(); i++) {
        ofPath p = svg.getPathAt(i);
        // svg defaults to non zero winding which doesn't look so good as contours
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> &lines = const_cast<vector<ofPolyline> &>(p.getOutline());

        for (int j = 0; j < (int) lines.size(); j++) {
            ofPolyline l = lines[j].getResampledBySpacing(spacing);
            shared_ptr<ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
            edge.get()->addVertexes(l);
            edge.get()->create(box2d.getWorld());
            edges.push_back(edge);
        }
    }
}

void XBScene3::updateVioinCello(){
    XBScene3GUI *myGUI = (XBScene3GUI *) gui;

    v.glowRadius = myGUI->glowRadius * violinEnergy;
    x.glowRadius = myGUI->glowRadius * celloEnergy;
    v.glowAmount = myGUI->glowAmount;
    x.glowAmount = myGUI->glowAmount;
    
    // update violin and cello positions and colours
    vPathIndex += myGUI->pathSpeed;
    v.setSize(myGUI->size);
    v.setMaxForce(myGUI->maxForce);
    v.setMaxSpeed(myGUI->maxSpeed);
    v.setTailStiffness(myGUI->stiffness);
    v.setTailDamping(myGUI->damping);
    v.setMass(myGUI->mass);
    ofPoint vTarget = vPath.getPointAtIndexInterpolated(vPathIndex);
    v.seek(vTarget);
    v.update();
    
    xPathIndex += myGUI->pathSpeed;
    x.setSize(myGUI->size);
    x.setMaxForce(myGUI->maxForce);
    x.setMaxSpeed(myGUI->maxSpeed);
    x.setTailStiffness(myGUI->stiffness);
    x.setTailDamping(myGUI->damping);
    x.setMass(myGUI->mass);
    ofPoint xTarget = xPath.getPointAtIndexInterpolated(xPathIndex);
    x.seek(xTarget);
    x.update();
    
    if(myGUI->linkAudio){
        v.setColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA * violinEnergy));
        x.setColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA * celloEnergy));
    }
    else{
        v.setColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA ));
        x.setColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA ));
    }
        
    //update particles emitters
    float dist = x.getLocation().distance(v.getLocation());

    if (dist < myGUI->maxDistance) {
        float numParticles = ofMap(dist, 0, myGUI->maxDistance, myGUI->maxParticles, 0);
        if(numParticles < 1.f && numParticles > 0.f)
            numParticles = ofGetFrameNum() % 2;
        numParticles = floor(numParticles);
        for(int i = 0; i< numParticles; i++){
            shared_ptr<CustomBox2dParticle> c = shared_ptr<CustomBox2dParticle>(new CustomBox2dParticle);
            c.get()->setPhysics(0.2, 0.2, 0.002);
            c.get()->setup(box2d.getWorld(), v.getLocation().x, v.getLocation().y, myGUI->particleSize, myGUI->particleLife);
            ofPoint vel = myGUI->particleVelocity;
            ofPoint spread = myGUI->particleSpread;
            c.get()->setVelocity(vel.x + ofRandom(-spread.x, spread.x), vel.y + ofRandom(-spread.y, spread.y));
            c.get()->setColor(ofColor(myGUI->rgbColorViolinR, myGUI->rgbColorViolinG, myGUI->rgbColorViolinB, myGUI->colorViolinA* violinEnergy));
            circles.push_back(c);
            
            shared_ptr<CustomBox2dParticle> c2 = shared_ptr<CustomBox2dParticle>(new CustomBox2dParticle);
            c2.get()->setPhysics(0.2, 0.2, 0.002);
            c2.get()->setup(box2d.getWorld(), x.getLocation().x, x.getLocation().y, myGUI->particleSize, myGUI->particleLife);
            c2.get()->setVelocity(vel.x + ofRandom(-spread.x, spread.x), vel.y + ofRandom(-spread.y, spread.y));
            c2.get()->setColor(ofColor(myGUI->rgbColorCelloR, myGUI->rgbColorCelloG, myGUI->rgbColorCelloB, myGUI->colorCelloA * celloEnergy));
            circles.push_back(c2);
        }
    }
    // update particles
    ofRemove(circles, CustomBox2dParticle::shouldRemove);
    for (int i = 0; i < circles.size(); i++)
        circles[i].get()->update();
    box2d.update();
}

