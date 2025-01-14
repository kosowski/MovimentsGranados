#include "MainApp.h"
#include "XBScene1.h"
#include "XBScene2.h"
#include "XBScene3.h"
#include "XBScene4.h"
#include "XBScene5.h"
#include "XBScene6.h"
#include "../../Shared/OSCSettings.h"
#include "XBSettingsManager.h"

#ifdef OF_DEBUG
static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS (DEBUG)";
#else
static const string STR_WINDOW_TITLE = "MOVIMENTS GRANADOS";
#endif

static const string STR_APPSETTINGS_FILENAME = "settings/AppSettings.xml";


void MainApp::setup()
{
    ofSetWindowTitle(STR_WINDOW_TITLE);
    ofBackground(0, 0, 0);

    XBSettingsManager::getInstance().loadFile(STR_APPSETTINGS_FILENAME);
    float windowScale = XBSettingsManager::getInstance().getWindowScale();

    if (MAIN_WINDOW_MODE == OF_WINDOW)
    {
        int windowWidths = int(MAIN_WINDOW_WIDTH * windowScale) + GUI_WINDOW_WIDTH;
        int winX = ofGetScreenWidth()/2 - windowWidths/2;
        int winY = ofGetScreenHeight()/2 - int(MAIN_WINDOW_HEIGHT * windowScale)/2;
        ofSetWindowShape(int(MAIN_WINDOW_WIDTH * windowScale), int(MAIN_WINDOW_HEIGHT * windowScale));
        ofSetWindowPosition(winX, winY);
    }

    ofSetFrameRate(60);
    ofSetVerticalSync(true);

#ifdef OF_DEBUG
    showFPS = true;
#else
    showFPS = false;
#endif

    XBOSCManager::getInstance().init(OSC_CELLO_SENDER_PORT, OSC_VIOLIN_SENDER_PORT, OSC_PIANO_SENDER_PORT, OSC_KINECT_SENDER_PORT);

    int sceneIndex = 0;
    XBScene1 *scene1 = new XBScene1("Scene 1");
    scene1->setup(guiApp->getGuiForSceneIndex(sceneIndex++));
    sceneManager.addScene(scene1);
    XBScene2 *scene2 = new XBScene2("Scene 2");
    scene2->setup(guiApp->getGuiForSceneIndex(sceneIndex++));
    sceneManager.addScene(scene2);
    XBScene3 *scene3 = new XBScene3("Scene 3");
    scene3->setup(guiApp->getGuiForSceneIndex(sceneIndex++));
    sceneManager.addScene(scene3);
    XBScene4 *scene4 = new XBScene4("Scene 4");
    scene4->setup(guiApp->getGuiForSceneIndex(sceneIndex++));
    sceneManager.addScene(scene4);
    XBScene5 *scene5 = new XBScene5("Scene 5");
    scene5->setup(guiApp->getGuiForSceneIndex(sceneIndex++));
    sceneManager.addScene(scene5);
    XBScene6 *scene6 = new XBScene6("Scene 6");
    scene6->setup(guiApp->getGuiForSceneIndex(sceneIndex++));
    sceneManager.addScene(scene6);

    // Start with scene 0
    sceneManager.setup(0);
    
    //setup syphon server
    syphonServer.setName("MovimentsGranados");
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
        ofDrawBitmapString(ofToString(roundf(ofGetFrameRate())) + "fps", 15, 15);
    }
    
    syphonServer.publishScreen();
}

void MainApp::exit()
{
    sceneManager.exit();
}

void MainApp::keyPressed(int key)
{
    sceneManager.keyPressed(key);
}

void MainApp::keyReleased(int key)
{
    float fadeOutTime = XBSettingsManager::getInstance().getFadeOutTime();
    float fadeInTime = XBSettingsManager::getInstance().getFadeInTime();

    switch (key)
    {
        case OF_KEY_LEFT:
        case OF_KEY_DOWN:
            sceneManager.goToPrevScene(fadeOutTime, fadeInTime);
            break;
        case OF_KEY_RIGHT:
        case OF_KEY_UP:
            sceneManager.goToNextScene(fadeOutTime, fadeInTime);
            break;
        case 'q':
        case 'Q':
            showFPS = !showFPS; break;
        case '1':
            sceneManager.goToScene(0, fadeOutTime, fadeInTime); break;
        case '2':
            sceneManager.goToScene(1, fadeOutTime, fadeInTime); break;
        case '3':
            sceneManager.goToScene(2, fadeOutTime, fadeInTime); break;
        case '4':
            sceneManager.goToScene(3, fadeOutTime, fadeInTime); break;
        case '5':
            sceneManager.goToScene(4, fadeOutTime, fadeInTime); break;
        case '6':
            sceneManager.goToScene(5, fadeOutTime, fadeInTime); break;
        default: break;
    }
    sceneManager.keyReleased(key);
}
