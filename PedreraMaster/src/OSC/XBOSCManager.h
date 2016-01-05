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

    ofxOscReceiver kinectReceiver;
};


#endif //PEDRERAMASTER_XBOSCMANAGER_H
