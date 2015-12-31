#include "ofApp.h"
#include "MathUtils.h"

static const int DEFAULT_SAMPLERATE = 44100;
static const int DEFAULT_BUFFERSIZE = 512;

static const int GUI_DEV_WIDTH  = 300;
static const int GUI_AN_WIDTH   = 200;
static const int GUI_MARGIN     = 10;

static const string DEVICE_SETTINGS_FILENAME   = "deviceSettings.xml";
static const string STR_DEV_TITLE              = "DEVICE SELECTOR";
static const string STR_DEV_STATUS             = "CURRENT STATUS";
static const string STR_DEV_STATUS_ON          = "ON";
static const string STR_DEV_STATUS_OFF         = "OFF";
static const string STR_DEV_START              = "START";
static const string STR_DEV_STOP               = "STOP";

static const string ANALYSIS_FILENAME          = "audioAnalyzer.xml";
static const string STR_AN_TITLE               = "AUDIO ANALYZER";
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
static const float SILENCE_THRSHLD_MIN = 0, SILENCE_THRSHLD_MAX = 0.5;
static const float SILENCE_LENGTH_MIN = 0, SILENCE_LENGTH_MAX = 1000;
static const float PAUSE_LENGTH_MIN = 0, PAUSE_LENGTH_MAX = 10000;
static const float ONSET_THRSHLD_MIN = 0, ONSET_THRSHLD_MAX = 1;

void ofApp::setup()
{
    ofSetWindowTitle("AUDIO ANALYZER");
    ofBackground(81, 88, 111);

    int ascDescAnalysisSize = 20;
    PMAudioAnalyzer::getInstance().init(silenceThreshold, (unsigned int)silenceLength, onsetsThreshold, ascDescAnalysisSize);

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
    unsigned int deviceID = 0;
    vector<unsigned int> enabledChannels;

    bool deviceIsValid = false;
    for (int i=0; i<deviceParams.size() && !deviceIsValid; ++i)
    {
        deviceID = deviceParams[i].getDeviceID();
        deviceIsValid = (deviceParams[i].getIsEnabled()) && (deviceParams[i].getEnabledChannels().size() == 2);
        if (deviceIsValid) enabledChannels = deviceParams[i].getEnabledChannels();
    }

    if (deviceIsValid)
    {
        vector<ofSoundDevice> devices = PMAudioAnalyzer::getInstance().getInputDevices();
        bool deviceFound = false;
        int deviceIndex = 0;

        for (int i=0; i<devices.size() && !deviceFound; ++i)
        {
            deviceFound = (devices[i].deviceID == deviceID);
            if (deviceFound) deviceIndex = i;
        }

        // Create audio analyzers
        for (unsigned int i=0; i<1; ++i)
        {
            vector<unsigned int> analyzerEnabledChannels;
            analyzerEnabledChannels.push_back(enabledChannels[i]);

            unsigned int audioInputIndex = i;
            PMDeviceAudioAnalyzer *deviceAudioAnalyzer = PMAudioAnalyzer::getInstance().addDeviceAnalyzer(i,
                    devices[deviceIndex].deviceID,
                    devices[deviceIndex].inputChannels,
                    devices[deviceIndex].outputChannels,
                    DEFAULT_SAMPLERATE,
                    DEFAULT_BUFFERSIZE,
                    analyzerEnabledChannels);

            ofAddListener(deviceAudioAnalyzer->eventPitchChanged, this, &ofApp::pitchChanged);
            ofAddListener(deviceAudioAnalyzer->eventEnergyChanged, this, &ofApp::energyChanged);
            ofAddListener(deviceAudioAnalyzer->eventSilenceStateChanged, this, &ofApp::silenceStateChanged);
            ofAddListener(deviceAudioAnalyzer->eventPauseStateChanged, this, &ofApp::pauseStateChanged);
            ofAddListener(deviceAudioAnalyzer->eventOnsetStateChanged, this, &ofApp::onsetDetected);
        }

        unsigned int audioInputIndex = 0;

        audioAnalyzers = PMAudioAnalyzer::getInstance().getAudioAnalyzers();

        // Register GUI events
        {
            silenceThreshold.addListener(this, &ofApp::silenceThresholdChanged);
            silenceLength.addListener(this, &ofApp::silenceLengthChanged);
            pauseLength.addListener(this, &ofApp::pauseLengthChanged);
            onsetsThreshold.addListener(this, &ofApp::onsetsThresholdChanged);
        }

        // Force initial audio analyzer values setup
        {
            float tmpSilenceThrshld = silenceThreshold.get();
            silenceThresholdChanged(tmpSilenceThrshld);
            float tmpSilenceLength = silenceLength.get();
            silenceLengthChanged(tmpSilenceLength);
            float tmpPauseLength = pauseLength.get();
            pauseLengthChanged(tmpPauseLength);
            float tmpOnsetsThrshld = onsetsThreshold.get();
            onsetsThresholdChanged(tmpOnsetsThrshld);
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
}

void ofApp::silenceThresholdChanged(float &threshold)
{
    vector<PMDeviceAudioAnalyzer *>::iterator itAudioAnalyzer;
    for (itAudioAnalyzer = audioAnalyzers->begin(); itAudioAnalyzer != audioAnalyzers->end(); ++itAudioAnalyzer)
    {
        if ((*itAudioAnalyzer)->getInputIndex() != 0) continue;
        (*itAudioAnalyzer)->setSilenceThreshold(threshold);
    }
}

void ofApp::silenceLengthChanged(float &length)
{
    vector<PMDeviceAudioAnalyzer *>::iterator itAudioAnalyzer;
    for (itAudioAnalyzer = audioAnalyzers->begin(); itAudioAnalyzer != audioAnalyzers->end(); ++itAudioAnalyzer)
    {
        if ((*itAudioAnalyzer)->getInputIndex() != 0) continue;
        (*itAudioAnalyzer)->setSilenceQueueLength(length);
    }
}

void ofApp::pauseLengthChanged(float &length)
{
    vector<PMDeviceAudioAnalyzer *>::iterator itAudioAnalyzer;
    for (itAudioAnalyzer = audioAnalyzers->begin(); itAudioAnalyzer != audioAnalyzers->end(); ++itAudioAnalyzer)
    {
        if ((*itAudioAnalyzer)->getInputIndex() != 0) continue;
        (*itAudioAnalyzer)->setPauseTimeTreshold(length);
    }
}

void ofApp::onsetsThresholdChanged(float &threshold)
{
    vector<PMDeviceAudioAnalyzer *>::iterator itAudioAnalyzer;
    for (itAudioAnalyzer = audioAnalyzers->begin(); itAudioAnalyzer != audioAnalyzers->end(); ++itAudioAnalyzer)
    {
        if ((*itAudioAnalyzer)->getInputIndex() != 0) continue;
        (*itAudioAnalyzer)->setOnsetsThreshold(threshold);
    }
}

void ofApp::pitchChanged(pitchParams &pitchParams)
{
    pitchMidiNote = truncateFloat(pitchParams.midiNote, 2);
}

void ofApp::energyChanged(energyParams &energyParams)
{
    energyEnergy = truncateFloat(energyParams.energy * energyGain, 2);
}

void ofApp::silenceStateChanged(silenceParams &silenceParams)
{
    silenceOn = silenceParams.isSilent;
}

void ofApp::pauseStateChanged(pauseParams &pauseParams)
{
    pauseOn = pauseParams.isPaused;
}

void ofApp::onsetDetected(onsetParams &onsetParams)
{
    onsetsOn = onsetParams.isOnset;
}
