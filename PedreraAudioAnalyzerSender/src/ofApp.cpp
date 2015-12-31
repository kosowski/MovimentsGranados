#include "ofApp.h"

#include "PMAudioAnalyzer.hpp"

const int GUI_DEV_WIDTH = 300;
const int GUI_AN_WIDTH = 200;
const int GUI_MARGIN = 10;

const string DEVICE_SETTINGS_FILENAME   = "deviceSettings.xml";
const string STR_DEV_TITLE              = "DEVICE SELECTOR";
const string STR_DEV_STATUS             = "CURRENT STATUS";
const string STR_DEV_STATUS_ON          = "ON";
const string STR_DEV_STATUS_OFF         = "OFF";
const string STR_DEV_START              = "START";
const string STR_DEV_STOP               = "STOP";

const string ANALYSIS_FILENAME          = "audioAnalyzer.xml";
const string STR_AN_TITLE               = "AUDIO ANALYZER";
const string STR_PITCH                  = "Pitch";
const string STR_PITCH_MIDINOTE         = "Midi Note";
const string STR_ENERGY                 = "Energy";
const string STR_ENERGY_VALUE           = "Energy";
const string STR_ENERGY_GAIN            = "Gain";
const string STR_SILENCE                = "Silence";
const string STR_SILENCE_THRESHOLD      = "Threshold";
const string STR_SILENCE_LENGTH         = "Length (ms)";
const string STR_SILENCE_STATUS         = "Current Status";
const string STR_SILENCE_STATUS_ON      = "ON";
const string STR_SILENCE_STATUS_OFF     = "OFF";
const string STR_PAUSE                  = "Pause";
const string STR_PAUSE_LENGTH           = "Length (ms)";
const string STR_PAUSE_STATUS           = "Current Status";
const string STR_PAUSE_STATUS_ON        = "ON";
const string STR_PAUSE_STATUS_OFF       = "OFF";
const string STR_ONSETS                 = "Onsets";
const string STR_ONSETS_THRESHOLD       = "Threshold";
const string STR_ONSETS_STATUS          = "Current Status";
const string STR_ONSETS_STATUS_ON       = "ON";
const string STR_ONSETS_STATUS_OFF      = "OFF";

const float PITCH_MIN = 0;
const float PITCH_MAX = 127;
const float ENERGY_MIN = 0;
const float ENERGY_MAX = 1;
const float GAIN_MIN = 1;
const float GAIN_MAX = 10;
const float SILENCE_THRSHLD_MIN = 0;
const float SILENCE_THRSHLD_MAX = 0.5;
const float SILENCE_LENGTH_MIN = 0;
const float SILENCE_LENGTH_MAX = 1000;
const float PAUSE_LENGTH_MIN = 0;
const float PAUSE_LENGTH_MAX = 10000;
const float ONSET_THRSHLD_MIN = 0;
const float ONSET_THRSHLD_MAX = 1;

void ofApp::setup()
{
    ofSetWindowTitle("AUDIO ANALYZER");
    ofBackground(81, 88, 111);

    buildDevicesPanel();
    buildAnalysisPanel();
}

void ofApp::update()
{
}

void ofApp::draw()
{
    guiDevices.draw();
    guiAnalysis.draw();
}

void ofApp::exit()
{
    guiDevices.saveToFile(DEVICE_SETTINGS_FILENAME);
    guiAnalysis.saveToFile(ANALYSIS_FILENAME);
}

void ofApp::keyReleased(int key)
{
}

void ofApp::buildDevicesPanel()
{
    vector<ofSoundDevice> devices = PMAudioAnalyzer::getInstance().getInputDevices();

    guiDevices.setup(STR_DEV_TITLE, DEVICE_SETTINGS_FILENAME);
    guiDevices.setPosition(GUI_MARGIN, GUI_MARGIN);

    for (int i=0; i<devices.size(); ++i)
    {
        if (devices[i].inputChannels < 2) continue;

        XBDeviceParams devParams(devices[i].deviceID, devices[i].name, devices[i].inputChannels);
        guiDevices.add(devParams.getParams());

        deviceParams.push_back(devParams);
    }

    guiDevices.loadFromFile(DEVICE_SETTINGS_FILENAME);

    guiDevices.add(lblStatus.setup(STR_DEV_STATUS, STR_DEV_STATUS_OFF));
    lblStatus.setBackgroundColor(ofColor::darkRed);
    guiDevices.add(btnStartAnalysis.setup(STR_DEV_START));
    guiDevices.add(btnStopAnalysis.setup(STR_DEV_STOP));

    btnStartAnalysis.addListener(this, &ofApp::startButtonPressed);
    btnStopAnalysis.addListener(this, &ofApp::stopButtonPressed);

    lblStatus.setDefaultWidth(GUI_DEV_WIDTH);

    guiDevices.setSize(GUI_DEV_WIDTH, GUI_DEV_WIDTH);
    guiDevices.setWidthElements(GUI_DEV_WIDTH);
}

void ofApp::buildAnalysisPanel()
{
    guiAnalysis.setup(STR_AN_TITLE, ANALYSIS_FILENAME);
    guiAnalysis.setPosition(ofGetWidth() - GUI_AN_WIDTH - GUI_MARGIN - 1, GUI_MARGIN);

    paramsPitch.setName(STR_PITCH);
    paramsPitch.add(pitchMidiNote.set(STR_PITCH_MIDINOTE, PITCH_MIN, PITCH_MIN, PITCH_MAX));
    guiAnalysis.add(paramsPitch);

    paramsEnergy.setName(STR_ENERGY);
    paramsEnergy.add(energyEnergy.set(STR_ENERGY_VALUE, ENERGY_MIN, ENERGY_MIN, ENERGY_MAX));
    paramsEnergy.add(energyGain.set(STR_ENERGY_GAIN, GAIN_MIN, GAIN_MIN, GAIN_MAX));
    guiAnalysis.add(paramsEnergy);

    paramsSilence.setName(STR_SILENCE);
    paramsSilence.add(silenceThreshold.set(STR_SILENCE_THRESHOLD, SILENCE_THRSHLD_MIN, SILENCE_THRSHLD_MIN, SILENCE_THRSHLD_MAX));
    paramsSilence.add(silenceLength.set(STR_SILENCE_LENGTH, SILENCE_LENGTH_MIN, SILENCE_LENGTH_MIN, SILENCE_LENGTH_MAX));
    paramsSilence.add(silenceOn.set(STR_SILENCE_STATUS, false));
    guiAnalysis.add(paramsSilence);

    paramsPause.setName(STR_PAUSE);
    paramsPause.add(pauseLength.set(STR_PAUSE_LENGTH, PAUSE_LENGTH_MIN, PAUSE_LENGTH_MIN, PAUSE_LENGTH_MAX));
    paramsPause.add(pauseOn.set(STR_PAUSE_STATUS, false));
    guiAnalysis.add(paramsPause);

    paramsOnsets.setName(STR_ONSETS);
    paramsOnsets.add(onsetsThreshold.set(STR_ONSETS_THRESHOLD, ONSET_THRSHLD_MIN, ONSET_THRSHLD_MIN, ONSET_THRSHLD_MAX));
    paramsOnsets.add(onsetsOn.set(STR_ONSETS_STATUS, false));
    guiAnalysis.add(paramsOnsets);

    guiAnalysis.loadFromFile(ANALYSIS_FILENAME);

    guiAnalysis.setSize(GUI_AN_WIDTH, GUI_AN_WIDTH);
    guiAnalysis.setWidthElements(GUI_AN_WIDTH);
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

    if (found)
    {
        lblStatus.setup(STR_DEV_STATUS, STR_DEV_STATUS_ON);
        lblStatus.setBackgroundColor(ofColor::darkGreen);
    }
}

void ofApp::stopButtonPressed()
{
    lblStatus.setup(STR_DEV_STATUS, STR_DEV_STATUS_OFF);
    lblStatus.setBackgroundColor(ofColor::darkRed);
}
