//
//  PMAudioOutParams.h
//  PMConcertParaules
//
//  Created by Miquel Àngel Soler on 13/10/15.
//

#ifndef PMAudioOutParams_h
#define PMAudioOutParams_h

#pragma once

struct baseAudioInParams {
    unsigned int audioInputIndex;
    int deviceID;
    int channelID;
};

struct pitchParams : public baseAudioInParams {
    float midiNote;
    float min;
    float max;
    float deltaPitch;
    float smoothedPitch;
    float confidence;
    float minConfidence;
};

struct energyParams : public baseAudioInParams {
    float energy;
    float min;
    float max;
    float deltaEnergy;
    float smoothedEnergy;
};

struct silenceParams : public baseAudioInParams {
    bool isSilent;
};

struct freqBandsParams : public baseAudioInParams {
    float *melBands;
    int numBands;
};

struct onsetParams : public baseAudioInParams {
    bool isOnset;
};

struct shtParams : public baseAudioInParams {
    bool isSht;
    float time;
};

struct pauseParams : public baseAudioInParams {
    bool isPaused;
};

struct melodyDirectionParams : public baseAudioInParams {
    float direction;
};


#endif /* PMAudioOutParams_h */
