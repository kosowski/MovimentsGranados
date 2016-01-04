#include "ofApp.h"
#include "MathUtils.h"
#include "../../Shared/OSCSettings.h"


static const int DEFAULT_SAMPLERATE = 44100;
static const int DEFAULT_BUFFERSIZE = 512;

static const int GUI_DEV_WIDTH  = 300;
static const int GUI_AN_WIDTH   = 200;
static const int GUI_MARGIN     = 10;

static const string STR_APP_TITLE              = "AUDIO ANALYZER [CELLO]";

static const string DEVICE_SETTINGS_FILENAME   = "deviceSettings.xml";
static const string STR_DEV_TITLE              = "DEVICE SELECTOR";
static const string STR_DEV_STATUS             = "CURRENT STATUS";
static const string STR_DEV_STATUS_ON          = "ON";
static const string STR_DEV_STATUS_OFF         = "OFF";
static const string STR_DEV_START              = "START";
static const string STR_DEV_STOP               = "STOP";

static const string ANALYSIS_CELLO_FILENAME    = "analyzer.xml";
static const string STR_PITCH                  = "Pitch";
static const string STR_PITCH_MIDINOTE         = "Midi Note";
static const string STR_ENERGY                 = "Energy";
static const string STR_ENERGY_VALUE           = "Energy";
static const string STR_ENERGY_GAIN            = "Gain";
static const string STR_SILENCE                = "Silence";
static const string STR_SILENCE_THRESHOLD      = "Threshold";
static const string STR_SILENCE_LENGTH         = "Length (ms)";
static const string STR_SILENCE_STATUS         = "Current Status";
static const string STR_SILENCE_STATUS_ON      = "ON";
static const string STR_SILENCE_STATUS_OFF     = "OFF";
static const string STR_PAUSE                  = "Pause";
static const string STR_PAUSE_LENGTH           = "Length (ms)";
static const string STR_PAUSE_STATUS           = "Current Status";
static const string STR_PAUSE_STATUS_ON        = "ON";
static const string STR_PAUSE_STATUS_OFF       = "OFF";
static const string STR_ONSETS                 = "Onsets";
static const string STR_ONSETS_THRESHOLD       = "Threshold";
static const string STR_ONSETS_STATUS          = "Current Status";
static const string STR_ONSETS_STATUS_ON       = "ON";
static const string STR_ONSETS_STATUS_OFF      = "OFF";

static const float PITCH_MIN = 0, PITCH_MAX = 127;
static const float ENERGY_MIN = 0, ENERGY_MAX = 1;
static const float GAIN_MIN = 1, GAIN_MAX = 10;
static const float SILENCE_THRSHLD_MIN = 0.01, SILENCE_THRSHLD_MAX = 0.5;
static const float SILENCE_LENGTH_MIN = 0, SILENCE_LENGTH_MAX = 1000;
static const float PAUSE_LENGTH_MIN = 0.01, PAUSE_LENGTH_MAX = 10000;
static const float ONSET_THRSHLD_MIN = 0.01, ONSET_THRSHLD_MAX = 1;

void ofApp::setup()
{
    ofSetWindowTitle(STR_APP_TITLE);
    ofBackground(81, 88, 111);

    float silenceThreshold = 0;
    unsigned int silenceLength = 0;
    float onsetsThreshold = 0;
    int ascDescAnalysisSize = 20;
    PMAudioAnalyzer::getInstance().init(silenceThreshold, silenceLength, onsetsThreshold, ascDescAnalysisSize);

    buildDevicesPanel();
    buildCelloAnalysisPanel();

    oscSender.setup(OSC_CELLO_SENDER_HOST, OSC_CELLO_SENDER_PORT);
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
    guiAnalysis.saveToFile(ANALYSIS_CELLO_FILENAME);
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

void ofApp::buildCelloAnalysisPanel()
{
    guiAnalysis.setup("CELLO", ANALYSIS_CELLO_FILENAME);
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

    guiAnalysis.loadFromFile(ANALYSIS_CELLO_FILENAME);

    guiAnalysis.setSize(GUI_AN_WIDTH, GUI_AN_WIDTH);
    guiAnalysis.setWidthElements(GUI_AN_WIDTH);
}

void ofApp::startButtonPressed()
{
    unsigned int deviceID = 0;
    vector<unsigned int> enabledChannels;

    bool deviceIsValid = false;
    for (int i=0; i<deviceParams.size() && !deviceIsValid; ++i)
    {
        deviceID = deviceParams[i].getDeviceID();
        deviceIsValid = (deviceParams[i].getIsEnabled()) && (deviceParams[i].getEnabledChannels().size() == 1);
        if (deviceIsValid) enabledChannels = deviceParams[i].getEnabledChannels();
    }

    if (deviceIsValid)
    {
        // Create Audio Analyzer for the selected device

        vector<ofSoundDevice> devices = PMAudioAnalyzer::getInstance().getInputDevices();
        bool deviceFound = false;
        int deviceIndex = 0;

        for (int i=0; i<devices.size() && !deviceFound; ++i)
        {
            deviceFound = (devices[i].deviceID == deviceID);
            if (deviceFound) deviceIndex = i;
        }

        unsigned int audioInputIndex = 0;
        PMDeviceAudioAnalyzer *deviceAudioAnalyzer = PMAudioAnalyzer::getInstance().addDeviceAnalyzer(audioInputIndex,
                devices[deviceIndex].deviceID,
                enabledChannels[0],
                devices[deviceIndex].outputChannels,
                DEFAULT_SAMPLERATE, DEFAULT_BUFFERSIZE,
                enabledChannels,
                devices[deviceIndex].inputChannels);

        ofAddListener(deviceAudioAnalyzer->eventPitchChanged, this, &ofApp::analyzerPitchChanged);
        ofAddListener(deviceAudioAnalyzer->eventEnergyChanged, this, &ofApp::analyzerEnergyChanged);
        ofAddListener(deviceAudioAnalyzer->eventSilenceStateChanged, this, &ofApp::analyzerSilenceStateChanged);
        ofAddListener(deviceAudioAnalyzer->eventPauseStateChanged, this, &ofApp::analyzerPauseStateChanged);
        ofAddListener(deviceAudioAnalyzer->eventOnsetStateChanged, this, &ofApp::analyzerOnsetDetected);

        audioAnalyzers = PMAudioAnalyzer::getInstance().getAudioAnalyzers();

        // Send "start" OSC message
        {
            ofxOscMessage m;
            m.setAddress("/start");
            oscSender.sendMessage(m, false);
        }

        // Register GUI events
        {
            silenceThreshold.addListener(this, &ofApp::guiSilenceThresholdChanged);
            silenceLength.addListener(this, &ofApp::guiSilenceLengthChanged);
            pauseLength.addListener(this, &ofApp::guiPauseLengthChanged);
            onsetsThreshold.addListener(this, &ofApp::guiOnsetsThresholdChanged);
        }

        // Force initial audio analyzer values setup
        {
            float tmpSilenceThrshld0 = silenceThreshold.get();
            guiSilenceThresholdChanged(tmpSilenceThrshld0);
            float tmpSilenceLength0 = silenceLength.get();
            guiSilenceLengthChanged(tmpSilenceLength0);
            float tmpPauseLength0 = pauseLength.get();
            guiPauseLengthChanged(tmpPauseLength0);
            float tmpOnsetsThrshld0 = onsetsThreshold.get();
            guiOnsetsThresholdChanged(tmpOnsetsThrshld0);
        }

        PMAudioAnalyzer::getInstance().start();

        lblStatus.setup(STR_DEV_STATUS, STR_DEV_STATUS_ON);
        lblStatus.setBackgroundColor(ofColor::darkGreen);
    }
}

void ofApp::stopButtonPressed()
{
    PMAudioAnalyzer::getInstance().stop();

    lblStatus.setup(STR_DEV_STATUS, STR_DEV_STATUS_OFF);
    lblStatus.setBackgroundColor(ofColor::darkRed);

    // Send "stop" OSC message
    {
        ofxOscMessage m;
        m.setAddress("/stop");
        oscSender.sendMessage(m, false);
    }
}

void ofApp::guiSilenceThresholdChanged(float &threshold) {
    (*audioAnalyzers)[0]->setSilenceThreshold(threshold);
}

void ofApp::guiSilenceLengthChanged(float &length) {
    (*audioAnalyzers)[0]->setSilenceQueueLength(length);
}

void ofApp::guiPauseLengthChanged(float &length) {
    (*audioAnalyzers)[0]->setPauseTimeTreshold(length);
}

void ofApp::guiOnsetsThresholdChanged(float &threshold) {
    (*audioAnalyzers)[0]->setOnsetsThreshold(threshold);
}

void ofApp::analyzerPitchChanged(pitchParams &pitchParams)
{
    pitchMidiNote = truncateFloat(pitchParams.midiNote, 2);

    // Send pitch MIDI note OSC message
    {
        ofxOscMessage m;
        m.setAddress("/pitchMIDINote");
        m.addFloatArg(pitchParams.midiNote);
        oscSender.sendMessage(m, false);
    }
}

void ofApp::analyzerEnergyChanged(energyParams &energyParams)
{
    float gainEnergy = energyParams.energy * energyGain;
    energyEnergy = truncateFloat(gainEnergy, 2);

    // Send energy OSC message
    {
        ofxOscMessage m;
        m.setAddress("/energy");
        m.addFloatArg(gainEnergy);
        oscSender.sendMessage(m, false);
    }
}

void ofApp::analyzerSilenceStateChanged(silenceParams &silenceParams)
{
    silenceOn = silenceParams.isSilent;

    // Send silence change OSC message
    {
        ofxOscMessage m;
        m.setAddress("/silence");
        m.addBoolArg(silenceOn);
        oscSender.sendMessage(m, false);
    }
}

void ofApp::analyzerPauseStateChanged(pauseParams &pauseParams)
{
    pauseOn = pauseParams.isPaused;

    // Send pause change OSC message
    {
        ofxOscMessage m;
        m.setAddress("/pause");
        m.addBoolArg(pauseOn);
        oscSender.sendMessage(m, false);
    }
}

void ofApp::analyzerOnsetDetected(onsetParams &onsetParams)
{
    onsetsOn = onsetParams.isOnset;

    // Send onset detected OSC message
    {
        ofxOscMessage m;
        m.setAddress("/onset");
        oscSender.sendMessage(m, false);
    }
}
