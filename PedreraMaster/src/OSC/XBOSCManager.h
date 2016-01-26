//
// Created by Miquel Àngel Soler on 5/1/16.
//

#ifndef PEDRERAMASTER_XBOSCMANAGER_H
#define PEDRERAMASTER_XBOSCMANAGER_H

#include "ofxOSC.h"

class XBOSCManager
{
public:

    static XBOSCManager &getInstance() {
        static XBOSCManager instance;
        return instance;
    }

    void init(int celloPort, int violinPort, int pianoPort, int kinectPort);
    void update();

    // Cello events
    ofEvent<void>   eventCelloStarted;
    ofEvent<void>   eventCelloStopped;
    ofEvent<float>  eventCelloPitchChanged;
    ofEvent<float>  eventCelloEnergyChanged;
    ofEvent<bool>   eventCelloSilenceChanged;
    ofEvent<bool>   eventCelloPauseChanged;
    ofEvent<void>   eventCelloOnsetDetected;

    // Violin events
    ofEvent<void>   eventViolinStarted;
    ofEvent<void>   eventViolinStopped;
    ofEvent<float>  eventViolinPitchChanged;
    ofEvent<float>  eventViolinEnergyChanged;
    ofEvent<bool>   eventViolinSilenceChanged;
    ofEvent<bool>   eventViolinPauseChanged;
    ofEvent<void>   eventViolinOnsetDetected;

    // Piano events
    class PianoNoteOnArgs : public ofEventArgs
    {
    public:
        int pitch;
        int velocity;
    };
    ofEvent<PianoNoteOnArgs>    eventPianoNoteOn;
    ofEvent<int>                eventPianoNoteOff;

private:

    XBOSCManager() {};

    void handleCelloMessages();
    void handleViolinMessages();
    void handlePianoMessages();
    void handleKinectMessages();

    void buildCelloAddresses();
    void buildViolinAddresses();
    void buildPianoReceiverAddresses();
    void buildKinectReceiverAddresses();

    ofxOscReceiver celloReceiver;
    string celloAddrStarted;
    string celloAddrStopped;
    string celloAddrPitchNote;
    string celloAddrEnergy;
    string celloAddrSilence;
    string celloAddrPause;
    string celloAddrOnset;

    ofxOscReceiver violinReceiver;
    string violinAddrStarted;
    string violinAddrStopped;
    string violinAddrPitchNote;
    string violinAddrEnergy;
    string violinAddrSilence;
    string violinAddrPause;
    string violinAddrOnset;

    ofxOscReceiver pianoReceiver;
    string pianoAddrNoteOn;
    string pianoAddrNoteOff;

    ofxOscReceiver kinectReceiver;
    string kinectAddrLeftPosition;
    string kinectAddrLeftVelocity;
    string kinectAddrRightPosition;
    string kinectAddrRightVelocity;
};


#endif //PEDRERAMASTER_XBOSCMANAGER_H
