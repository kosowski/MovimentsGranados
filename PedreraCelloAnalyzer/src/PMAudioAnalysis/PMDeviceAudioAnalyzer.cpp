//
//  PMDeviceAudioAnalyzer.cpp
//  ConcertParaules
//
//  Created by Miquel Àngel Soler on 25/9/15.
//
//

#include "PMDeviceAudioAnalyzer.hpp"


// TODO: Should be able to change to a custom number
static const int NUM_MELBANDS = 40;

PMDeviceAudioAnalyzer::PMDeviceAudioAnalyzer(int _deviceID, int _channelID, int _outChannels, int _sampleRate, int _bufferSize, int _inChannels)
{
    deviceID = _deviceID;
    channelID = _channelID;
    inChannels = _inChannels;
    outChannels = _outChannels;
    sampleRate = _sampleRate;
    bufferSize = _bufferSize;
    numBuffers = bufferSize / 64;

    soundStream.setDeviceID(deviceID);
    soundStream.printDeviceList();

    isSetup = false;
    oldPitch = 0.0f;
    oldEnergy = 0.0f;

    /**/
    channelInput = new float[bufferSize];
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
    for (int k=0; k<channelNumbers.size(); ++k)
    {
        /**/
        for (int i = 0; i < bufferSize; i++) {
            channelInput[i] = input[(i * nChannels) + channelID];
        }

        // Init of audio event params struct
        pParams.deviceID = deviceID;
        pParams.audioInputIndex = audioInputIndex;
        silenceParams silenceParams;
        silenceParams.deviceID = deviceID;
        silenceParams.audioInputIndex = audioInputIndex;
        eParams.deviceID = deviceID;
        eParams.audioInputIndex = audioInputIndex;
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

        /// PITCH
        ///////////
        float currentMidiNote = aubioPitch->latestPitch;
        float pitchConfidence = aubioPitch->pitchConfidence;
        pParams.confidence = pitchConfidence;
        pParams.minConfidence = 0.5;
        if(pitchConfidence<pParams.minConfidence)
        {
            currentMidiNote = pParams.min + (pParams.max-pParams.min) / 2.0;
        }
        //cout << "Pitch Confidence " << aubioPitch->pitchConfidence << endl;

        // SILENCE
        ////////////

        float absMean = getAbsMean(input,bufferSize);

        bool nowIsSilent = (eParams.smoothedEnergy < silenceThreshold);

        if(nowIsSilent && !alreadyInSilence)
        {
            silenceStarted();
//        cout << "SILENCE STARTED" << endl;
        }
        else if(nowIsSilent && ( (ofGetElapsedTimeMillis() - notSilenceBeginTime) > silenceTimeTreshold ) )
        {
            notifySilence(true);
//        cout << "SILENCE BROKEN DURATION DONE  ... Sent TRUE!! Mean : " << absMean << " __ thrs." << silenceThreshold  << endl;

        }
        else if(!nowIsSilent)
        {
            notifySilence(false);
            silenceStopped();
//        cout << "SILENCE ENDED ... Sent FALSE!" << absMean << " __ thrs." << silenceThreshold  << endl;
        }
        else
        {
//        cout << "ELSE " << (ofGetElapsedTimeMillis() - notSilenceBeginTime) << " .... " << silenceTimeTreshold << endl;
        }







//    {
//        if (wasSilent != isSilent) // Changes in silence (ON>OFF or OFF>ON)
//        {
//            if (isSilent)
//            {
//                silenceStarted();
//            } else
//            {
//                silenceEnded();
//            }
//            wasSilent = isSilent;
//        }
//
//        if (isInSilence)
//            sendSilenceEvent();
//    }

//    if(isSilent) cout << "in silence..." << endl;
//    else cout << "not silent" << endl;

        // Pitch
        {
            if (currentMidiNote)
            {
                pParams.midiNote = currentMidiNote;
                float pitchDelted = (pParams.deltaPitch)*pParams.midiNote + (1.0 - pParams.deltaPitch)*oldPitch;
                pParams.smoothedPitch = ofMap(pitchDelted,pParams.min,pParams.max,0,1,true);
                oldPitch = pitchDelted;
                ofNotifyEvent(eventPitchChanged, pParams, this);

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

        // Energy
        {
            // Raw Energy
            eParams.energy = absMean;

            // Smoothed and Mapped Energy = energySmoothed
            float energyDelted =(eParams.deltaEnergy)*eParams.energy + (1.0 - eParams.deltaEnergy)*oldEnergy;
            eParams.smoothedEnergy = ofMap(energyDelted*digitalGain,eParams.min,eParams.max,0,1,true);

            oldEnergy = energyDelted;

            ofNotifyEvent(eventEnergyChanged, eParams, this);
        }

        // Shhhht
        {
//        if (!isSilent) checkShtSound();
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

//    cout << "g:" << digitalGain << " st:" << silenceThreshold << " sl:" << silenceTimeTreshold << endl;
    }
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
    float actual =0.0f;
    for (int i=0; i<bufferSize; ++i)
    {
        for (int j=0; j<channelNumbers.size(); ++j)
        {
            actual = abs(input[(i * inChannels) + channelNumbers[j]]*digitalGain);
            //actual = ofMap(actual,eParams.min,eParams.max,0,1,true);
            sum = sum + actual;
        }
    }
//    for (int j=0; j<channelNumbers.size(); ++j)
//    {
//        //float energyDelted =(eParams.deltaEnergy)*eParams.energy + (1.0 - eParams.deltaEnergy)*oldEnergy;
//        actual = abs(input[(i * inChannels) + channelNumbers[j]]);
//        actual = ofMap(actual*digitalGain,eParams.min,eParams.max,0,1,true);
//        sum = sum + actual;
//    }

//    cout << " .......... " << endl;
//    cout <<"SUM " << sum << endl << " __ bufferSize " << (bufferSize) << " __ ch.num " << channelNumbers.size()  << " __ In.Channels : " << inChannels <<" __ Result : "  <<  (sum / (bufferSize * channelNumbers.size())) << endl;

    return (sum / (bufferSize * channelNumbers.size()));
}

void PMDeviceAudioAnalyzer::silenceStopped()
{
    notSilenceBeginTime = ofGetElapsedTimeMillis();
    alreadyInSilence = false;
}

void PMDeviceAudioAnalyzer::notifySilence(bool b)
{
    float timeOfSilence = ofGetElapsedTimeMillis() - notSilenceBeginTime;

    bool sendSilenceEvent = ((timeOfSilence > silenceTimeTreshold) && (oldTimeOfSilence <= silenceTimeTreshold));
//    if (sendSilenceEvent)
    {
        silenceParams silenceParams;
        silenceParams.deviceID = deviceID;
        silenceParams.audioInputIndex = audioInputIndex;
        silenceParams.isSilent = b;
        ofNotifyEvent(eventSilenceStateChanged, silenceParams, this);
    }

    oldTimeOfSilence = timeOfSilence;

    bool sendPauseEvent = false;

    if (timeOfSilence > pauseTimeTreshold)
    {
        sendPauseEvent = !isInPause;
        isInPause = true;
    } else
    {
        sendPauseEvent = isInPause;
        isInPause = false;
    }

    if (sendPauseEvent)
    {
        pauseParams pauseParams;
        pauseParams.deviceID = deviceID;
        pauseParams.audioInputIndex = audioInputIndex;
        pauseParams.isPaused = isInPause;
        ofNotifyEvent(eventPauseStateChanged, pauseParams, this);
    }
}

void PMDeviceAudioAnalyzer::silenceStarted()
{
    float timeOfSilence = ofGetElapsedTimeMillis() - notSilenceBeginTime;

//    if (isInSilence) {
//        silenceParams silenceParams;
//        silenceParams.deviceID = deviceID;
//        silenceParams.audioInputIndex = audioInputIndex;
//        silenceParams.isSilent = false;
//        ofNotifyEvent(eventSilenceStateChanged, silenceParams, this);
//    }

    if (isInPause) {
        pauseParams pauseParams;
        pauseParams.deviceID = deviceID;
        pauseParams.audioInputIndex = audioInputIndex;
        pauseParams.isPaused = false;
        ofNotifyEvent(eventPauseStateChanged, pauseParams, this);
    }
    isInPause = false;
    alreadyInSilence = true;
    notSilenceBeginTime = ofGetElapsedTimeMillis();
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
