#include "MainApp.h"
#include "XBScene1.h"
#include "XBScene2.h"
#include "XBScene3.h"
#include "XBScene4.h"
#include "../../Shared/OSCSettings.h"

static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS";


void MainApp::setup()
{
    ofSetWindowTitle(STR_WINDOW_TITLE);

    ofSetFrameRate(60);
    ofSetVerticalSync(true);

#ifdef OF_DEBUG
    showFPS = true;
#else
    showFPS = false;
#endif

    ofBackground(0, 0, 0);

    XBOSCManager::getInstance().init(OSC_CELLO_SENDER_PORT, OSC_VIOLIN_SENDER_PORT, OSC_PIANO_SENDER_PORT, OSC_KINECT_SENDER_PORT);

    ofAddListener(sceneManager.eventSceneChanged, this, &MainApp::sceneChanged);

    XBScene1 *scene1 = new XBScene1("Scene 1");
    scene1->setup();
    sceneManager.addScene(scene1);
    XBScene2 *scene2 = new XBScene2("Scene 2");
    scene2->setup();
    sceneManager.addScene(scene2);
    XBScene3 *scene3 = new XBScene3("Scene 3");
    scene3->setup();
    sceneManager.addScene(scene3);
    XBScene4 *scene4 = new XBScene4("Scene 4");
    scene4->setup();
    sceneManager.addScene(scene4);
    // Start with scene 0
    sceneManager.setup(0);
}

void MainApp::update()
{
    XBOSCManager::getInstance().update();
    sceneManager.update();
}

void MainApp::draw()
{
    sceneManager.draw();

    if (showFPS) {
        ofSetColor(ofColor::white);
        ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", 15, ofGetHeight() - 15);
    }
}

void MainApp::exit()
{
    sceneManager.exit();
}

void MainApp::keyReleased(int key)
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
        case '4':
            sceneManager.goToScene(3); break;
        default: break;
    }
    sceneManager.keyReleased(key);
}

void MainApp::mouseMoved(int x, int y)
{
}

void MainApp::mouseDragged(int x, int y, int button)
{
}

void MainApp::mousePressed(int x, int y, int button)
{
}

void MainApp::mouseReleased(int x, int y, int button)
{
}

void MainApp::mouseEntered(int x, int y)
{
}

void MainApp::mouseExited(int x, int y)
{
}
