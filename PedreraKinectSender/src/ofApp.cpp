#include "ofApp.h"
#include "../../Shared/OSCSettings.h"

static const string STR_APP_TITLE           = "KINECT DETECTOR";
static const string STR_GUI_TITLE           = "Kinect Status";

static const string STR_STATE               = "STATUS";
static const string STR_STATE_SETUP         = "Initializing Kinect...";
static const string STR_STATE_DETECTING     = "Detecting Person...";
static const string STR_STATE_CAPTURING     = "Capturing";
static const string STR_BUTTON_RESTART      = "RESTART";
static const string STR_TOGGLE_SHOW_KINECT  = "Draw Kinect Output";
static const string STR_TOGGLE_SHOW_HANDS   = "Draw Hand Detection";

static const string SETTINGS_FILENAME       = "settings.xml";

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
        gui.add(showImage.setup(STR_TOGGLE_SHOW_KINECT, true));
        gui.add(showHands.setup(STR_TOGGLE_SHOW_HANDS, true));

        gui.setSize(GUI_WIDTH, GUI_WIDTH);
        gui.setWidthElements(GUI_WIDTH);
    }

    // OSC
    {
        oscSender.setup(OSC_KINECT_SENDER_HOST, OSC_KINECT_SENDER_PORT);
    }

    // KINECT / MOTION
    {
        motionExtractor = &PMMotionExtractor::getInstance();
        kinectAvailable = motionExtractor->setup();
        if (kinectAvailable) {
            currState = STATE_DETECTING;
        }
    }

    ofAddListener(motionExtractor->eventUserDetection, this, &ofApp::userDetection);
}

void ofApp::update()
{
    handleStateChanges();

    if (kinectAvailable)
    {
        switch (currState)
        {
            case STATE_SETUP: {
                break;
            }
            case STATE_DETECTING: {
                motionExtractor->update();
                break;
            }
            case STATE_CAPTURING: {
                motionExtractor->update();
                KinectInfo hands = motionExtractor->gethandsPosition();
                sendHandInfo(hands);
                break;
            }
        }
    }
    else if (ofGetMousePressed())
    {
        currState = STATE_CAPTURING; //change state of fake detection
        KinectInfo fakeHands;
        fakeHands.leftHand.pos.x = ofGetMouseX() / ofGetWidth();
        fakeHands.leftHand.pos.y = ofGetMouseY() / ofGetHeight();
        fakeHands.rightHand.pos.x = ofMap(fakeHands.leftHand.pos.x, 0, 1, 1, 0);
//        fakeHands.rightHand.pos.y=ofMap(fakeHands.leftHand.pos.y, 0, 1, 1, 0);
        fakeHands.rightHand.pos.y = fakeHands.leftHand.pos.y;
        sendHandInfo(fakeHands); //send fake info by osc
    } else
    {
        currState = STATE_DETECTING;
    }
}

void ofApp::draw()
{
    if (showImage && kinectAvailable) //draw only if is detecting and if kinect is open
        motionExtractor->draw(showHands);
    gui.draw();
}

void ofApp::exit()
{
    motionExtractor->exit();
    gui.saveToFile(SETTINGS_FILENAME);
}

void ofApp::handleStateChanges()
{
    if (currState == prevState) return;

    string stateDescr;
    switch (currState) {
        case STATE_SETUP: {
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
        case STATE_DETECTING: {
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
        case STATE_CAPTURING: {
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

void ofApp::userDetection(bool &hasUser)
{
    if (hasUser)
        currState = STATE_CAPTURING;
    else
        currState = STATE_DETECTING;
}

void ofApp::sendHandInfo(KinectInfo hands)
{
    //LEFT HAND POSITION
    ofxOscMessage lhandpos;
    stringstream lhandpos_address;
    lhandpos_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_LHAND << OSC_KINECT_ADDR_POSITION;
    lhandpos.setAddress(lhandpos_address.str());
    lhandpos.addFloatArg(hands.leftHand.pos.x);
    lhandpos.addFloatArg(hands.leftHand.pos.y);
    lhandpos.addFloatArg(hands.leftHand.pos.z);
    oscSender.sendMessage(lhandpos, false);

    //RIGHT HAND POSITION
    ofxOscMessage rhandpos;
    stringstream rhandpos_address;
    rhandpos_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_RHAND << OSC_KINECT_ADDR_POSITION;
    rhandpos.setAddress(rhandpos_address.str());
    rhandpos.addFloatArg(hands.rightHand.pos.x);
    rhandpos.addFloatArg(hands.rightHand.pos.y);
    rhandpos.addFloatArg(hands.rightHand.pos.z);
    oscSender.sendMessage(rhandpos, false);

    //LEFT HAND VELOCITY
    ofxOscMessage lhandvel;
    stringstream lhandvel_address;
    lhandvel_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_LHAND << OSC_KINECT_ADDR_VELOCITY;
    lhandvel.setAddress(lhandvel_address.str());
    lhandvel.addFloatArg(hands.leftHand.v.x);
    lhandvel.addFloatArg(hands.leftHand.v.y);
    lhandvel.addFloatArg(hands.leftHand.v.z);
    oscSender.sendMessage(lhandvel, false);

    //RIGHT HAND VELOCITY
    ofxOscMessage rhandvel;
    stringstream rhandvel_address;
    rhandvel_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_LHAND << OSC_KINECT_ADDR_VELOCITY;
    rhandvel.setAddress(rhandvel_address.str());
    rhandvel.addFloatArg(hands.rightHand.v.x);
    rhandvel.addFloatArg(hands.rightHand.v.y);
    rhandvel.addFloatArg(hands.rightHand.v.z);
    oscSender.sendMessage(rhandvel, false);
}

void ofApp::keyReleased(int key)
{
    // TODO: Remove after Kinect is working. The code below is just to simulate state changes.
    switch (key) {
        case '1':
            currState = STATE_SETUP;
            break;
        case '2':
            currState = STATE_DETECTING;
            break;
        case '3':
            currState = STATE_CAPTURING;
            break;
        default:
            break;
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
