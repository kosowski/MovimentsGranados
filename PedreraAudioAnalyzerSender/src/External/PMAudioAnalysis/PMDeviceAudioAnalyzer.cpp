//
//  PMDeviceAudioAnalyzer.cpp
//  ConcertParaules
//
//  Created by Miquel Àngel Soler on 25/9/15.
//
//

#include "PMDeviceAudioAnalyzer.hpp"

#define USE_RECORDER false

#if USE_RECORDER == true
#include "PMRecorder.hpp"
#endif

// TODO: Should be able to change to a custom number
static const int NUM_MELBANDS = 40;

PMDeviceAudioAnalyzer::PMDeviceAudioAnalyzer(int _deviceID, int _inChannels, int _outChannels, int _sampleRate, int _bufferSize)
{
    deviceID = _deviceID;
    inChannels = _inChannels;
    outChannels = _outChannels;
    sampleRate = _sampleRate;
    bufferSize = _bufferSize;
    numBuffers = bufferSize / 64;

    soundStream.setDeviceID(deviceID);
    soundStream.printDeviceList();

    isSetup = false;
}

PMDeviceAudioAnalyzer::~PMDeviceAudioAnalyzer()
{
}

void PMDeviceAudioAnalyzer::setup(unsigned int _audioInputIndex, vector<unsigned int> _channelNumbers,
        float _silenceThreshold, unsigned int silenceQueueLength,
        float _onsetsThreshold, int _ascDescAnalysisSize)
{

    if (isSetup) return;

    audioInputIndex = _audioInputIndex;

    // Channels
    channelNumbers = _channelNumbers;

    // Silence & Pause
    digitalGain = 1.0f;
    wasSilent = false;
    silenceThreshold = _silenceThreshold;
    silenceTimeTreshold = silenceQueueLength;
    pauseTimeTreshold = 1000;
    oldTimeOfSilence = 0;

    // Shhht!
    shtTimeTreshold = 150;

    // Onsets
    onsetsThreshold = _onsetsThreshold;
    oldOnsetState = false;

    // Pitch
    ascDescAnalysisSize = _ascDescAnalysisSize;

    // Aubio Setup
    aubioPitch = new ofxAubioPitch();
    aubioPitch->setup();
    aubioOnset = new ofxAubioOnset();
    aubioOnset->setup();
    aubioMelBands = new ofxAubioMelBands();
    aubioMelBands->setup();

    isSetup = true;
}

void PMDeviceAudioAnalyzer::start()
{
    soundStream.stop();

    soundStream.setup(outChannels, inChannels, sampleRate, bufferSize, numBuffers);
    soundStream.setInput(this);
}

void PMDeviceAudioAnalyzer::stop()
{
    soundStream.stop();
}

void PMDeviceAudioAnalyzer::clear()
{
    stop();
    //TODO: Delete aubio
}

void PMDeviceAudioAnalyzer::audioIn(float *input, int bufferSize, int nChannels)
{
    // Init of audio event params struct
    pitchParams pitchParams;
    pitchParams.deviceID = deviceID;
    pitchParams.audioInputIndex = audioInputIndex;
    silenceParams silenceParams;
    silenceParams.deviceID = deviceID;
    silenceParams.audioInputIndex = audioInputIndex;
    energyParams energyParams;
    energyParams.deviceID = deviceID;
    energyParams.audioInputIndex = audioInputIndex;
    onsetParams onsetParams;
    onsetParams.deviceID = deviceID;
    onsetParams.audioInputIndex = audioInputIndex;
    freqBandsParams freqBandsParams;
    freqBandsParams.deviceID = deviceID;
    freqBandsParams.audioInputIndex = audioInputIndex;

    aubioOnset->setThreshold(onsetsThreshold);

    aubioPitch->audioIn(input, bufferSize, nChannels);
    aubioOnset->audioIn(input, bufferSize, nChannels);
    aubioMelBands->audioIn(input, bufferSize, nChannels);

    float currentMidiNote = aubioPitch->latestPitch;
//    cout << "Pitch confidence: " << aubioPitch->pitchConfidence << endl;

    // Silence
    //cout << " sil.thr : " << silenceThreshold << endl;
    bool isSilent = (getAbsMean(input, bufferSize) < silenceThreshold);
    {
        if (wasSilent != isSilent) // Changes in silence (ON>OFF or OFF>ON)
        {
            wasSilent = isSilent;
            if (isSilent) {
                silenceStarted();
            } else {
                silenceEnded();

            }
        }

        if (isInSilence)
            updateSilenceTime();
    }

//    if(isSilent) cout << "in silence..." << endl;
//    else cout << "not silent" << endl;
    
    // Pitch
    {
        if (currentMidiNote)
        {
            pitchParams.midiNote = currentMidiNote;
            ofNotifyEvent(eventPitchChanged, pitchParams, this);

            midiNoteHistory.push_front(currentMidiNote);

            if (midiNoteHistory.size() > ascDescAnalysisSize)
                midiNoteHistory.pop_back();

            // MELODY DIRECTION
            checkMelodyDirection();
        } else {
            if (midiNoteHistory.size() > 0)
                midiNoteHistory.pop_back();
        }
    }

    // Mel bands
    {
        energyParams.energy = getAbsMean(input, bufferSize);
        ofNotifyEvent(eventEnergyChanged, energyParams, this);
    }

    // Shhhht
    {
        if (!isSilent) checkShtSound();
    }

    // Onsets
    {
        bool isOnset = aubioOnset->received();
        if (oldOnsetState != isOnset)
        {
            oldOnsetState = isOnset;
            onsetParams.isOnset = isOnset;
            ofNotifyEvent(eventOnsetStateChanged, onsetParams, this);
        }
    }

#if USE_RECORDER == true
    //Call to the Recorder
    // FIXME: Això no hauria d'estar a l'analitzador d'àudio, sino fora!!!
    // FIXME: Now records all channels, better to chose how many chanels to record
    if (PMRecorder::getInstance().isRecording()) {
        PMRecorder::getInstance().addAudioBuffer(input, bufferSize, inChannels);
    }
#endif
}

float PMDeviceAudioAnalyzer::getEnergy()
{
    float *energies = aubioMelBands->energies;

    float result = 0.0f;

    for (int i = 0; i < NUM_MELBANDS; i++) {
        result += energies[i];
    }

    result /= NUM_MELBANDS; //Applied vector aritmetic mean https://en.wikipedia.org/wiki/Weighted_arithmetic_mean
    return result;
}

float PMDeviceAudioAnalyzer::getRms(float *input, int bufferSize)
{
    float rms = 0.0f;
    for (int i = 0; i < bufferSize * inChannels; i += inChannels) {
        rms += pow(input[i], 2);
    }
    rms = rms / bufferSize;
    rms = sqrt(rms);
    return rms;
}

float PMDeviceAudioAnalyzer::getAbsMean(float *input, int bufferSize)
{
    float sum = 0.0f;
    for (int i=0; i<bufferSize; ++i)
    {
        for (int j=0; j<channelNumbers.size(); ++j)
        {
            sum += abs(input[(i * inChannels) + channelNumbers[j]]*digitalGain);
        }
    }

    return (sum / (bufferSize * channelNumbers.size()));
}

void PMDeviceAudioAnalyzer::silenceStarted()
{
    silenceBeginTime = ofGetElapsedTimeMillis();
    isInSilence = true;
}

void PMDeviceAudioAnalyzer::updateSilenceTime()
{
    float timeOfSilence = ofGetElapsedTimeMillis() - silenceBeginTime;

    bool sendSilenceEvent = ((timeOfSilence > silenceTimeTreshold) && (oldTimeOfSilence <= silenceTimeTreshold));
    if (sendSilenceEvent)
    {
        silenceParams silenceParams;
        silenceParams.deviceID = deviceID;
        silenceParams.audioInputIndex = audioInputIndex;
        silenceParams.isSilent = isInSilence;
        silenceParams.silenceTime = 0;
        ofNotifyEvent(eventSilenceStateChanged, silenceParams, this);
    }

    oldTimeOfSilence = timeOfSilence;

    bool sendPauseEvent = false;

    if (timeOfSilence > pauseTimeTreshold) {
        sendPauseEvent = !isInPause;
        isInPause = true;
    } else {
        sendPauseEvent = isInPause;
        isInPause = false;
    }

    if (sendPauseEvent) {
        pauseParams pauseParams;
        pauseParams.deviceID = deviceID;
        pauseParams.audioInputIndex = audioInputIndex;
        pauseParams.isPaused = isInPause;
        pauseParams.pauseTime = 0;
        ofNotifyEvent(eventPauseStateChanged, pauseParams, this);
    }
}

void PMDeviceAudioAnalyzer::silenceEnded()
{
    float timeOfSilence = ofGetElapsedTimeMillis() - silenceBeginTime;

    if (isInSilence) {
        silenceParams silenceParams;
        silenceParams.deviceID = deviceID;
        silenceParams.audioInputIndex = audioInputIndex;
        silenceParams.isSilent = false;
        silenceParams.silenceTime = timeOfSilence;
        ofNotifyEvent(eventSilenceStateChanged, silenceParams, this);
    }

    if (isInPause) {
        pauseParams pauseParams;
        pauseParams.deviceID = deviceID;
        pauseParams.audioInputIndex = audioInputIndex;
        pauseParams.isPaused = false;
        pauseParams.pauseTime = timeOfSilence;
        ofNotifyEvent(eventPauseStateChanged, pauseParams, this);
    }
    isInPause = false;
    isInSilence = false;
}

void PMDeviceAudioAnalyzer::checkShtSound()
{
    float *melBands = aubioMelBands->energies;
    float lowBands = 0.0f;
    float highBands = 0.0f;
    int high_low_limit = NUM_MELBANDS * 2 / 3;
    for (int i = 0; i < high_low_limit; i++) {
        lowBands += melBands[i];
    }
    lowBands /= high_low_limit;
    for (int i = high_low_limit; i < NUM_MELBANDS; i++) {
        highBands += melBands[i];
    }
    highBands /= (NUM_MELBANDS - high_low_limit);


    if (highBands > 3 * lowBands && !isShtSounding) {
        shtBeginTime = ofGetElapsedTimeMillis();
        isShtSounding = true;
        isShtTrueSent = false;
    } else if (highBands < 3 * lowBands && isShtSounding) {
        isShtSounding = false;
        isShtFalseSent = false;
    }

    float timeOfSht = ofGetElapsedTimeMillis() - shtBeginTime;
    if (isShtSounding && timeOfSht > shtTimeTreshold && !isShtTrueSent) {
        shtParams shtParams;
        shtParams.deviceID = deviceID;
        shtParams.audioInputIndex = audioInputIndex;
        shtParams.time = timeOfSht;
        shtParams.isSht = true;
        ofNotifyEvent(eventShtStateChanged, shtParams, this);
        isShtTrueSent = true;
    }
    else if (!isShtSounding && !isShtFalseSent) {
        shtParams shtParams;
        shtParams.deviceID = deviceID;
        shtParams.audioInputIndex = audioInputIndex;
        shtParams.time = timeOfSht;
        shtParams.isSht = false;
        ofNotifyEvent(eventShtStateChanged, shtParams, this);
        isShtFalseSent = true;
    }
}

void PMDeviceAudioAnalyzer::checkMelodyDirection()
{
    melodyDirectionParams melodyDirectionParams;
    melodyDirectionParams.deviceID = deviceID;
    melodyDirectionParams.audioInputIndex = audioInputIndex;

    float diferenceSum = 0;
    for (int i = 0; i < midiNoteHistory.size() - 1; i++) {
        diferenceSum += midiNoteHistory[i + 1] - midiNoteHistory[i];
    }
    diferenceSum /= ascDescAnalysisSize;
    if (diferenceSum != 0 && midiNoteHistory.size() == ascDescAnalysisSize) {
        melodyDirectionParams.direction = diferenceSum;
        ofNotifyEvent(eventMelodyDirection, melodyDirectionParams, this);
    }
}

void PMDeviceAudioAnalyzer::setOnsetsThreshold(float value)
{
    onsetsThreshold = value;
}
