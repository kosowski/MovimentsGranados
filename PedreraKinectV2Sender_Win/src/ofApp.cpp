#include "ofApp.h"
#include "XBSettingsManager.h"
#include "../../Shared/OSCSettings.h"
#include "Defaults.h"

static const string STR_APP_TITLE           = "KINECT DETECTOR";
static const string STR_GUI_TITLE           = "Kinect Status";

static const string STR_STATE               = "STATUS";
static const string STR_STATE_DETECTING     = "Detecting Person...";
static const string STR_STATE_CAPTURING     = "Capturing";
static const string STR_BUTTON_RESTART      = "RESTART";
static const string STR_BUTTON_SENDVIDEO	= "Send Video over IP";
static const string STR_TOGGLE_SHOW_KINECT  = "Draw Kinect Output";
static const string STR_TOGGLE_SHOW_HANDS   = "Draw Hand Detection";

static const string SETTINGS_FILENAME       = "settings.xml";
static const string STR_APPSETTINGS_FILENAME= "AppSettings.xml";

static const int GUI_POSX = 10;
static const int GUI_POSY = 10;
static const int GUI_WIDTH = 350;

void ofApp::setup()
{
    ofSetWindowTitle(STR_APP_TITLE);
    ofBackground(ofColor::black);
	ofSetFrameRate(60);

    currState = prevState = STATE_SETUP;

    // GUI
    {
        gui.setup(STR_GUI_TITLE, SETTINGS_FILENAME);
        gui.setPosition(GUI_POSX, GUI_POSY);
        gui.add(showImage.setup(STR_TOGGLE_SHOW_KINECT, true));
        gui.add(showHands.setup(STR_TOGGLE_SHOW_HANDS, true));
		gui.add(sendVideo.setup(STR_BUTTON_SENDVIDEO, true));
        gui.loadFromFile(SETTINGS_FILENAME);

        gui.add(guiStatusLbl.setup(STR_STATE, STR_STATE_DETECTING));
        guiStatusLbl.setBackgroundColor(ofColor::darkRed);
        guiStatusLbl.setDefaultWidth(GUI_WIDTH);
        gui.add(guiRestartBtn.setup(STR_BUTTON_RESTART));
		
        
        guiRestartBtn.addListener(this, &ofApp::resetKinect);

        gui.setSize(GUI_WIDTH, GUI_WIDTH);
        gui.setWidthElements(GUI_WIDTH);
    }

    // OSC
    {
		XBSettingsManager::getInstance().loadFile(STR_APPSETTINGS_FILENAME);
		string oscHost = XBSettingsManager::getInstance().getOSCHost();
		cout << oscHost << endl;
		oscSender.setup(oscHost, OSC_KINECT_SENDER_PORT);
		oscSender_Max.setup(oscHost, OSC_KINECT_SENDER_PORT_MAX);
		oscSender_Receiver.setup(oscHost, OSC_KINECT_SENDER_PORT_RECEIVER);

		oscReceiver.setup(OSC_KINECT_SENDER_PORT_RECEIVER);
    }

    // KINECT / MOTION
    {
        motionExtractor = &PMMotionExtractor::getInstance();
        kinectAvailable = motionExtractor->setup();
        if (kinectAvailable) {
            currState = STATE_DETECTING;
        }
    }

	// POSITIONING
	{
		userPositionedTimeStamp = 0;
		userRemovedTimeStamp = 0;
	}

	// VIDEO Server
	{
		ofx::HTTP::SimpleIPVideoServerSettings settings;

		// Many other settings are available.
		settings.setPort(7890);

		// The default maximum number of client connections is 5.
		settings.ipVideoRouteSettings.setMaxClientConnections(2);

		// Apply the settings.
		server.setup(settings);

		// Start the server.
		server.start();
		
		//Get URL
		serverURL = server.getURL();
		cout << serverURL << endl;

		// Launch a browser with the address of the server.
		//ofLaunchBrowser(server.getURL());

		
	}

    ofAddListener(motionExtractor->eventUserDetection, this, &ofApp::userDetection);
	ofAddListener(motionExtractor->eventUserPositioned, this, &ofApp::userPositioned);


}

void ofApp::update()
{
    handleStateChanges();
    motionExtractor->update();


	if (sendVideo) {
		colorPixels.setFromPixels(motionExtractor->getColorPixels()); //Get Color Stream
		colorPixels.resize(480, 270); //Resize color stream to improve speed
		colorPixels.setImageType(OF_IMAGE_GRAYSCALE);
		server.send(colorPixels.getPixels()); //send resized color stream
	}


    if(currState == STATE_CAPTURING){
        handsInfo = motionExtractor->getHandsInfo();
        sendHandInfo();
    }

	//OSC
	while (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		if (m.getAddress() == "/receiver/reset")
			resetKinect();
		else if (m.getAddress() == "/receiver/ipvideo")
			sendVideo = m.getArgAsBool(0);
		else if (m.getAddress() == "/receiver/positionedThreshold")
			motionExtractor->setPositionedThreshold(m.getArgAsFloat(0));
		else if (m.getAddress() == "/receiver/meanSize")
			motionExtractor->setMeanSize(m.getArgAsInt32(0));
	}
}

void ofApp::draw()
{
    motionExtractor->draw(showImage, (showHands && currState==STATE_CAPTURING));

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
        case STATE_DETECTING: {
            stateDescr = STR_STATE_DETECTING;
            guiStatusLbl.setBackgroundColor(ofColor::darkBlue);

            ofxOscMessage m;
            stringstream address;
            address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_STATE;
            m.setAddress(address.str());
            m.addStringArg(OSC_KINECT_STATE_DETECTING);
            oscSender.sendMessage(m, false);
			oscSender_Max.sendMessage(m, false);
			oscSender_Receiver.sendMessage(m, false);

            break;
        }
        case STATE_CAPTURING: {
            stateDescr = STR_STATE_CAPTURING;
            guiStatusLbl.setBackgroundColor(ofColor::darkGreen);

            ofxOscMessage m;
            stringstream address;
            address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_STATE;
            m.setAddress(address.str());
            m.addStringArg(OSC_KINECT_STATE_CAPTURING);
            oscSender.sendMessage(m, false);
			oscSender_Max.sendMessage(m, false);
			oscSender_Receiver.sendMessage(m, false);

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
	else {
		currState = STATE_DETECTING;
		userRemovedTimeStamp = ofGetElapsedTimef();
	}
}

void ofApp::userPositioned(bool &hasPositioned)
{
	userPositionedTimeStamp = ofGetElapsedTimef();
	ofxOscMessage userPositioned;
	stringstream userPositioned_address;
	userPositioned_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_STATE;
	userPositioned.setAddress(userPositioned_address.str());
	userPositioned.addStringArg(OSC_KINECT_STATE_POSITIONED);
	oscSender.sendMessage(userPositioned, false);
	oscSender_Max.sendMessage(userPositioned, false);
	oscSender_Receiver.sendMessage(userPositioned, false);
}

void ofApp::sendHandInfo()
{
    //LEFT HAND POSITION
    ofxOscMessage lhandpos;
    stringstream lhandpos_address;
    lhandpos_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_LHAND << OSC_KINECT_ADDR_POSITION;
    lhandpos.setAddress(lhandpos_address.str());
    lhandpos.addFloatArg(handsInfo.leftHand.pos.x);
    lhandpos.addFloatArg(handsInfo.leftHand.pos.y);
    lhandpos.addFloatArg(handsInfo.leftHand.pos.z);
    oscSender.sendMessage(lhandpos, false);
	oscSender_Max.sendMessage(lhandpos, false);
	oscSender_Receiver.sendMessage(lhandpos, false);

    //RIGHT HAND POSITION
    ofxOscMessage rhandpos;
    stringstream rhandpos_address;
    rhandpos_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_RHAND << OSC_KINECT_ADDR_POSITION;
    rhandpos.setAddress(rhandpos_address.str());
    rhandpos.addFloatArg(handsInfo.rightHand.pos.x);
    rhandpos.addFloatArg(handsInfo.rightHand.pos.y);
    rhandpos.addFloatArg(handsInfo.rightHand.pos.z);
    oscSender.sendMessage(rhandpos, false);
	oscSender_Max.sendMessage(rhandpos, false);
	oscSender_Receiver.sendMessage(rhandpos, false);

    //LEFT HAND VELOCITY
    ofxOscMessage lhandvel;
    stringstream lhandvel_address;
    lhandvel_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_LHAND << OSC_KINECT_ADDR_VELOCITY;
    lhandvel.setAddress(lhandvel_address.str());
    lhandvel.addFloatArg(handsInfo.leftHand.v.x);
    lhandvel.addFloatArg(handsInfo.leftHand.v.y);
    lhandvel.addFloatArg(handsInfo.leftHand.v.z);
    oscSender.sendMessage(lhandvel, false);
	oscSender_Max.sendMessage(lhandvel, false);
	oscSender_Receiver.sendMessage(lhandvel, false);

    //RIGHT HAND VELOCITY
    ofxOscMessage rhandvel;
    stringstream rhandvel_address;
    rhandvel_address << OSC_KINECT_ADDR_BASE << OSC_KINECT_ADDR_RHAND << OSC_KINECT_ADDR_VELOCITY;
    rhandvel.setAddress(rhandvel_address.str());
    rhandvel.addFloatArg(handsInfo.rightHand.v.x);
    rhandvel.addFloatArg(handsInfo.rightHand.v.y);
    rhandvel.addFloatArg(handsInfo.rightHand.v.z);
    oscSender.sendMessage(rhandvel, false);
	oscSender_Max.sendMessage(rhandvel, false);
	oscSender_Receiver.sendMessage(rhandvel, false);
}

void ofApp::resetKinect(){
    kinectAvailable = motionExtractor->reset(kinectAvailable);
}

void ofApp::windowResized(int w, int h)
{
	float kinect_aspectRatio = (float)KINECT_STD_COLOR_WIDTH / (float)KINECT_STD_COLOR_HEIGHT;
	ofSetWindowShape((float)h*kinect_aspectRatio, h);
}