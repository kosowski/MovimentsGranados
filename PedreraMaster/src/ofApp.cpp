#include "ofApp.h"
#include "XBOSCManager.h"
#include "XBScene1.h"
#include "XBScene2.h"
#include "XBScene3.h"
#include "../../Shared/OSCSettings.h"

void ofApp::setup()
{
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

#ifdef OF_DEBUG
    showFPS = true;
#else
    showFPS = false;
#endif

    ofBackground(0, 0, 0);

    XBOSCManager::getInstance().init(OSC_CELLO_SENDER_PORT, OSC_VIOLIN_SENDER_PORT, OSC_PIANO_SENDER_PORT, OSC_KINECT_SENDER_PORT);

    XBScene1 *scene1 = new XBScene1("Scene 1");
    scene1->setup();
    sceneManager.addScene(scene1);
    XBScene2 *scene2 = new XBScene2("Scene 2");
    scene2->setup();
    sceneManager.addScene(scene2);
    XBScene3 *scene3 = new XBScene3("Scene 3");
    scene3->setup();
    sceneManager.addScene(scene3);

    // Start with scene 0
    sceneManager.setup(0);
}

void ofApp::update()
{
    XBOSCManager::getInstance().update();
    sceneManager.update();
}

void ofApp::draw()
{
    sceneManager.draw();

    if (showFPS) {
        ofSetColor(ofColor::white);
        ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", 15, ofGetHeight() - 15);
    }
}

void ofApp::exit()
{
    sceneManager.exit();
}

void ofApp::keyReleased(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
        case OF_KEY_DOWN:
            sceneManager.goToPrevScene(SCENETRANSITION_Fade, 0.5f);
            break;
        case OF_KEY_RIGHT:
        case OF_KEY_UP:
            sceneManager.goToNextScene(SCENETRANSITION_Fade, 0.5f);
            break;
        case 'f':
        case 'F':
            showFPS = !showFPS; break;
        case '1':
            sceneManager.goToScene(0); break;
        case '2':
            sceneManager.goToScene(1); break;
        case '3':
            sceneManager.goToScene(2); break;
        default: break;
    }
    sceneManager.keyReleased(key);
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
}
