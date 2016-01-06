//
// Created by Miquel Àngel Soler on 6/1/16.
//

#ifndef PEDRERAMASTER_XBOSCEVENTS_H
#define PEDRERAMASTER_XBOSCEVENTS_H


// Cello audio analysis
ofEvent<void>   eventCelloStarted;
ofEvent<void>   eventCelloStopped;
ofEvent<float>  eventCelloPitchChanged;
ofEvent<float>  eventCelloEnergyChanged;
ofEvent<bool>   eventCelloSilenceChanged;
ofEvent<bool>   eventCelloPauseChanged;
ofEvent<void>   eventCelloOnsetDetected;

// Violin audio analysis
ofEvent<void>   eventViolinStarted;
ofEvent<void>   eventViolinStopped;
ofEvent<float>  eventViolinPitchChanged;
ofEvent<float>  eventViolinEnergyChanged;
ofEvent<bool>   eventViolinSilenceChanged;
ofEvent<bool>   eventViolinPauseChanged;
ofEvent<void>   eventViolinOnsetDetected;


#endif //PEDRERAMASTER_XBOSCEVENTS_H
