#include "ofApp.h"
#include "../../Shared/OSCSettings.h"

#ifdef OF_DEBUG
static const string STR_APP_TITLE              = "PIANO [MIDI RECEIVER] (DEBUG)";
#else
static const string STR_APP_TITLE              = "PIANO [MIDI RECEIVER]";
#endif

static const string STR_MIDIPORTS_TITLE     = "MIDI PORTS";
static const string STR_SETTINGS_FILENAME   = "settings.xml";

static const string STR_STATUS              = "CURRENT STATUS";
static const string STR_STATUS_ON           = "ON";
static const string STR_STATUS_OFF          = "OFF";
static const string STR_START               = "START";
static const string STR_STOP                = "STOP";

static const int GUI_MARGIN     = 10;
static const int GUI_WIDTH      = 300;

static const int MAX_NUM_STRMESSAGES = 40;


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


        lblStatus.setDefaultWidth(GUI_WIDTH);
        gui.setSize(GUI_WIDTH, GUI_WIDTH);
        gui.setWidthElements(GUI_WIDTH);
    }

    // OSC
    {
        oscSender.setup(OSC_PIANO_SENDER_HOST, OSC_PIANO_SENDER_PORT);
    }
}

void ofApp::update()
{
}

void ofApp::draw()
{
    gui.draw();

    ofDrawBitmapString(strMessage.str(), GUI_MARGIN + GUI_WIDTH + GUI_MARGIN*2, GUI_MARGIN*2);
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
    vector<string> portList = midiIn.getPortList();

    int portIndex = 0;
    bool found = false;
    bool isVirtualPort = false;
    for (int i=0; i<portParams.size() && !found; ++i)
    {
        found = portParams[i].getIsEnabled();
        if (found)
        {
            portIndex = i;
            isVirtualPort = portParams[i].getIsVirtualPort();
        }
    }

    if (!found) return;

    if (isVirtualPort) cout << endl << "Opening virtual port " << portList[portIndex] << endl;
    else cout << "Is virtual? " << isVirtualPort << endl;

    if (isVirtualPort)
        midiIn.openVirtualPort(portList[portIndex]);
    else
        midiIn.openPort(portList[portIndex]);

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

    lblStatus.setBackgroundColor(ofColor::darkGreen);
    lblStatus.setup(STR_STATUS, STR_STATUS_ON);

    numStrMessages = 0;
    strMessage.str("");
}

void ofApp::stopButtonPressed()
{
    midiIn.removeListener(this);
    midiIn.closePort();
    lblStatus.setBackgroundColor(ofColor::darkRed);
    lblStatus.setup(STR_STATUS, STR_STATUS_OFF);
}

void ofApp::newMidiMessage(ofxMidiMessage &midiMessage)
{
    if (numStrMessages > MAX_NUM_STRMESSAGES)
    {
        strMessage.str("");
        numStrMessages = 0;
    }

    switch(midiMessage.status)
    {
        case MIDI_NOTE_ON:
        {
            ofxOscMessage m;
            stringstream address;
            address << OSC_PIANO_ADDR_BASE << OSC_PIANO_ADDR_NOTEON;
            m.setAddress(address.str());
            m.addIntArg(midiMessage.pitch);
            m.addIntArg(midiMessage.velocity);
            oscSender.sendMessage(m);

            strMessage << "Note On  - p:" << midiMessage.pitch << " v:" << midiMessage.velocity << endl;
            numStrMessages++;
            break;
        }
        case MIDI_NOTE_OFF:
        {
            ofxOscMessage m;
            stringstream address;
            address << OSC_PIANO_ADDR_BASE << OSC_PIANO_ADDR_NOTEOFF;
            m.setAddress(address.str());
            m.addIntArg(midiMessage.pitch);
            oscSender.sendMessage(m);

            strMessage << "Note Off - p:" << midiMessage.pitch << endl;
            numStrMessages++;
            break;
        }
        default: break;
    }

//    cout << midiMessage.toString() << endl;
}
