#include "ofApp.h"
#include "../../Shared/OSCSettings.h"

static const string STR_APP_TITLE = "KINECT DETECTOR";
static const string STR_GUI_TITLE = "Kinect Status";

static const string SETTINGS_FILENAME = "settings.xml";

static const string STR_STATE               = "STATUS";
static const string STR_STATE_SETUP         = "Initializing Kinect...";
static const string STR_STATE_DETECTING     = "Detecting Person...";
static const string STR_STATE_CAPTURING     = "Capturing";
static const string STR_BUTTON_RESTART      = "RESTART";

static const int GUI_POSX = 10;
static const int GUI_POSY = 10;
static const int GUI_WIDTH = 350;

void ofApp::setup()
{
    ofSetWindowTitle(STR_APP_TITLE);
    ofBackground(ofColor::black);

    currState = prevState = STATE_SETUP;

    // GUI
    {
        gui.setup(STR_GUI_TITLE, SETTINGS_FILENAME);
        gui.setPosition(GUI_POSX, GUI_POSY);
        gui.loadFromFile(SETTINGS_FILENAME);

        gui.add(guiStatusLbl.setup(STR_STATE, STR_STATE_SETUP));
        guiStatusLbl.setBackgroundColor(ofColor::darkRed);
        guiStatusLbl.setDefaultWidth(GUI_WIDTH);
        gui.add(guiRestartBtn.setup(STR_BUTTON_RESTART));

        gui.setSize(GUI_WIDTH, GUI_WIDTH);
        gui.setWidthElements(GUI_WIDTH);
    }

    // OSC
    {
        oscSender.setup(OSC_KINECT_SENDER_HOST, OSC_KINECT_SENDER_PORT);
    }
}

void ofApp::update()
{
    handleStateChanges();

    switch(currState)
    {
        case STATE_SETUP:
        {
            break;
        }
        case STATE_DETECTING:
        {
            break;
        }
        case STATE_CAPTURING:
        {
            break;
        }
    }
}

void ofApp::draw()
{
    gui.draw();
}

void ofApp::exit()
{
    gui.saveToFile(SETTINGS_FILENAME);
}

void ofApp::handleStateChanges()
{
    if (currState == prevState) return;

    string stateDescr;
    switch(currState)
    {
        case STATE_SETUP:
        {
            stateDescr = STR_STATE_SETUP;
            guiStatusLbl.setBackgroundColor(ofColor::darkRed);

            ofxOscMessage m;
            stringstream address;
            address << OSC_KINECT_ADDR_BASE << OSC_KINECT_STATE;
            m.setAddress(address.str());
            m.addStringArg(OSC_KINECT_STATE_SETUP);
            oscSender.sendMessage(m, false);

            break;
        }
        case STATE_DETECTING:
        {
            stateDescr = STR_STATE_DETECTING;
            guiStatusLbl.setBackgroundColor(ofColor::darkBlue);

            ofxOscMessage m;
            stringstream address;
            address << OSC_KINECT_ADDR_BASE << OSC_KINECT_STATE;
            m.setAddress(address.str());
            m.addStringArg(OSC_KINECT_STATE_DETECTING);
            oscSender.sendMessage(m, false);

            break;
        }
        case STATE_CAPTURING:
        {
            stateDescr = STR_STATE_CAPTURING;
            guiStatusLbl.setBackgroundColor(ofColor::darkGreen);

            ofxOscMessage m;
            stringstream address;
            address << OSC_KINECT_ADDR_BASE << OSC_KINECT_STATE;
            m.setAddress(address.str());
            m.addStringArg(OSC_KINECT_STATE_CAPTURING);
            oscSender.sendMessage(m, false);

            break;
        }
    }
    guiStatusLbl.setup(STR_STATE, stateDescr);

    prevState = currState;
}

void ofApp::keyReleased(int key)
{
    // TODO: Remove after Kinect is working. The code below is just to simulate state changes.
    switch(key)
    {
        case '1': currState = STATE_SETUP; break;
        case '2': currState = STATE_DETECTING; break;
        case '3': currState = STATE_CAPTURING; break;
        default: break;
    }
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
