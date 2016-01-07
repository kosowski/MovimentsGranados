#include "ofApp.h"

static const string STR_APP_TITLE           = "PIANO [MIDI RECEIVER]";
static const string STR_MIDIPORTS_TITLE     = "MIDI PORTS";
static const string STR_SETTINGS_FILENAME   = "settings.xml";

static const string STR_STATUS              = "CURRENT STATUS";
static const string STR_STATUS_ON           = "ON";
static const string STR_STATUS_OFF          = "OFF";
static const string STR_START               = "START";
static const string STR_STOP                = "STOP";

static const int GUI_MARGIN     = 10;
static const int GUI_WIDTH      = 300;


void ofApp::setup()
{
    ofSetWindowTitle(STR_APP_TITLE);
    ofBackground(81, 111, 88);

#ifdef OF_DEBUG
    midiIn.listPorts();
#endif

    // GUI
    {
        gui.setup(STR_MIDIPORTS_TITLE, STR_SETTINGS_FILENAME);
        gui.setPosition(GUI_MARGIN, GUI_MARGIN);

        vector<string> portList = midiIn.getPortList();
        for (int i=0; i<portList.size(); ++i)
        {
            XBMidiPortParams params(portList[i]);
            gui.add(params.getParams());

            portParams.push_back(params);
        }

        gui.loadFromFile(STR_SETTINGS_FILENAME);

        gui.add(lblStatus.setup(STR_STATUS, STR_STATUS_OFF));
        lblStatus.setBackgroundColor(ofColor::darkRed);

        gui.add(btnStart.setup(STR_START));
        gui.add(btnStop.setup(STR_STOP));
        btnStart.addListener(this, &ofApp::startButtonPressed);
        btnStop.addListener(this, &ofApp::stopButtonPressed);


        gui.setSize(GUI_WIDTH, GUI_WIDTH);
        gui.setWidthElements(GUI_WIDTH);
    }

    vector<string> portList = midiIn.getPortList();
    for (int i=0; i<portList.size(); ++i)
        cout << portList[i] << endl;

    midiIn.openVirtualPort(portList[0]);
    bool ignoreSysex = false;
    bool ignoreTiming = true;
    bool ignoreSense = false;

    midiIn.ignoreTypes(ignoreSysex, ignoreTiming, ignoreSense);

    midiIn.addListener(this);

#ifdef OF_DEBUG
    midiIn.setVerbose(true);
#else
    midiIn.setVerbose(false);
#endif
}

void ofApp::update()
{
}

void ofApp::draw()
{
    gui.draw();
}

void ofApp::exit()
{
    gui.saveToFile(STR_SETTINGS_FILENAME);
}

void ofApp::keyReleased(int key)
{
}

void ofApp::startButtonPressed()
{
}

void ofApp::stopButtonPressed()
{
}

void ofApp::newMidiMessage(ofxMidiMessage &eventArgs)
{
    cout << eventArgs.toString() << endl;
}
