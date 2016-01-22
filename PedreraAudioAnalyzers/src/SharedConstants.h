//
// Created by Miquel Àngel Soler on 21/1/16.
//

#ifndef PEDRERACELLOANALYZER_SHAREDCONSTANTS_H
#define PEDRERACELLOANALYZER_SHAREDCONSTANTS_H

#include "ofMain.h"

static const int DEFAULT_SAMPLERATE = 44100;
static const int DEFAULT_BUFFERSIZE = 512;

static const int GUI_DEV_WIDTH  = 300;
static const int GUI_AN_WIDTH   = 200;
static const int GUI_MARGIN     = 10;

static const string STR_DEV_TITLE              = "DEVICE SELECTOR";
static const string STR_DEV_STATUS             = "CURRENT STATUS";
static const string STR_DEV_STATUS_ON          = "ON";
static const string STR_DEV_STATUS_OFF         = "OFF";
static const string STR_DEV_START              = "START";
static const string STR_DEV_STOP               = "STOP";

static const string STR_PITCH                  = "Pitch";
static const string STR_PITCH_CURRENT_NOTE     = "Current Note";
static const string STR_PITCH_SMOOTHED_NOTE    = "Smoothed Note";
static const string STR_PITCH_SMOOTH_AMOUNT    = "Smooth Amount";
static const string STR_PITCH_MINMIDINOTE      = "Min. Note";
static const string STR_PITCH_MAXMIDINOTE      = "Max. Note";
static const string STR_ENERGY                 = "Energy";
static const string STR_ENERGY_VALUE           = "Current Energy";
static const string STR_ENERGY_SMOOTHED_VALUE  = "Smoothed Energy";
static const string STR_ENERGY_SMOOTH_AMOUNT   = "Smooth Amount";
static const string STR_ENERGY_MIN             = "Min";
static const string STR_ENERGY_MAX             = "Max";
static const string STR_ENERGY_GAIN            = "Gain";
static const string STR_SILENCE                = "Silence";
static const string STR_SILENCE_THRESHOLD      = "Threshold";
static const string STR_SILENCE_LENGTH         = "Length (ms)";
static const string STR_SILENCE_STATUS         = "Current Status";
static const string STR_ONSETS                 = "Onsets";
static const string STR_ONSETS_THRESHOLD       = "Threshold";
static const string STR_ONSETS_STATUS          = "Current Status";

static const float PITCH_MIN = 0, PITCH_MAX = 127;
static const float PITCH_SMOOTH_MIN = 0.1, PITCH_SMOOTH_MAX = 1;
static const float ENERGY_MIN = 0.01, ENERGY_MAX = 10;
static const float ENERGY_SMOOTH_MIN = 0.1, ENERGY_SMOOTH_MAX = 1;
static const float GAIN_MIN = 1, GAIN_MAX = 10;
static const float SILENCE_THRSHLD_MIN = 0.01, SILENCE_THRSHLD_MAX = 0.5;
static const float SILENCE_LENGTH_MIN = 0, SILENCE_LENGTH_MAX = 1000;
static const float ONSET_THRSHLD_MIN = 0.01, ONSET_THRSHLD_MAX = 1;

#endif //PEDRERACELLOANALYZER_SHAREDCONSTANTS_H
