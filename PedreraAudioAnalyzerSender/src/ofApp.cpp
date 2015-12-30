#include "ofApp.h"

#include "PMAudioAnalyzer.hpp"

const string SETTINGS_FILENAME = "settings.xml";
const int GUI_WIDTH = 300;
const string STR_STATUS = "CURRENT STATUS";
const string STR_STATUS_ON = "On";
const string STR_STATUS_OFF = "Off";

void ofApp::setup()
{
    vector<ofSoundDevice> devices = PMAudioAnalyzer::getInstance().getInputDevices();

    guiAudioDevices.setup("Device selector");

    for (int i=0; i<devices.size(); ++i)
    {
        if (devices[i].inputChannels < 2) continue;

        XBDeviceParams devParams(devices[i].deviceID, devices[i].name, devices[i].inputChannels);
        guiAudioDevices.add(devParams.getParams());

        deviceParams.push_back(devParams);
    }

    guiAudioDevices.loadFromFile(SETTINGS_FILENAME);

    guiAudioDevices.add(lblStatus.setup(STR_STATUS, STR_STATUS_OFF));
    guiAudioDevices.add(btnStartAnalysis.setup("START"));
    guiAudioDevices.add(btnStopAnalysis.setup("STOP"));

    btnStartAnalysis.addListener(this, &ofApp::startButtonPressed);
    btnStopAnalysis.addListener(this, &ofApp::stopButtonPressed);

    lblStatus.setDefaultWidth(GUI_WIDTH);

    guiAudioDevices.setSize(GUI_WIDTH, GUI_WIDTH);
    guiAudioDevices.setWidthElements(GUI_WIDTH);
}

void ofApp::update()
{
}

void ofApp::draw()
{
    guiAudioDevices.draw();
}

void ofApp::exit()
{
    guiAudioDevices.saveToFile(SETTINGS_FILENAME);
}

void ofApp::keyReleased(int key)
{
}

void ofApp::startButtonPressed()
{
    unsigned int deviceID;
    vector<unsigned int> enabledChannels;

    bool found = false;
    for (int i=0; i<deviceParams.size() && !found; ++i)
    {
        deviceID = deviceParams[i].getDeviceID();
        found = (deviceParams[i].getIsEnabled()) && (deviceParams[i].getEnabledChannels().size() == 2);
        if (found) enabledChannels = deviceParams[i].getEnabledChannels();
    }

    if (found) lblStatus.setup(STR_STATUS, STR_STATUS_ON);
}

void ofApp::stopButtonPressed()
{
    lblStatus.setup(STR_STATUS, STR_STATUS_OFF);
}
