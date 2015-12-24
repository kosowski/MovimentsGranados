#include "ofApp.h"
#include "XBScene1.h"
#include "XBScene2.h"
#include "XBScene3.h"

void ofApp::setup()
{
    sceneManager.addScene(new XBScene1("Scene 1"));
    sceneManager.addScene(new XBScene2("Scene 2"));
    sceneManager.addScene(new XBScene3("Scene 3"));

    sceneManager.goToScene(0);
}

void ofApp::update()
{
    sceneManager.update();
}

void ofApp::draw()
{
    sceneManager.draw();
}

void ofApp::keyPressed(int key)
{
}

void ofApp::keyReleased(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
        case OF_KEY_DOWN:
            sceneManager.goToPrevScene();
            break;
        case OF_KEY_RIGHT:
        case OF_KEY_UP:
            sceneManager.goToNextScene();
            break;
        case '1':
            sceneManager.goToScene(0);
            break;
        case '2':
            sceneManager.goToScene(1);
            break;
        case '3':
            sceneManager.goToScene(2);
            break;
        default: break;
    }
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
