//
// Created by Miquel Àngel Soler on 5/1/16.
//

#include "XBOSCManager.h"
#include "../../Shared/OSCSettings.h"

void XBOSCManager::init(int celloPort, int violinPort, int pianoPort, int kinectPort)
{
    celloReceiver.setup(celloPort);
    buildCelloAddresses();

    violinReceiver.setup(violinPort);
    buildViolinAddresses();

    pianoReceiver.setup(pianoPort);
    buildPianoReceiverAddresses();

    kinectReceiver.setup(kinectPort);
    buildKinectReceiverAddresses();
}

void XBOSCManager::update()
{
    handleCelloMessages();
    handleViolinMessages();
    handlePianoMessages();
    handleKinectMessages();
}

void XBOSCManager::handleCelloMessages()
{
    while (celloReceiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        celloReceiver.getNextMessage(&msg);
        string address = msg.getAddress();

        if (address == celloAddrStarted) {
        }
        else if (address == celloAddrStopped) {
        }
        else if (address == celloAddrPitchNote) {
        }
        else if (address == celloAddrEnergy) {
        }
        else if (address == celloAddrSilence) {
        }
        else if (address == celloAddrPause) {
        }
        else if (address == celloAddrOnset) {
        }
    }
}

void XBOSCManager::handleViolinMessages()
{
    while (violinReceiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        violinReceiver.getNextMessage(&msg);
        string address = msg.getAddress();

        if (address == violinAddrStarted) {
        }
        else if (address == violinAddrStopped) {
        }
        else if (address == violinAddrPitchNote) {
        }
        else if (address == violinAddrEnergy) {
        }
        else if (address == violinAddrSilence) {
        }
        else if (address == violinAddrPause) {
        }
        else if (address == violinAddrOnset) {
        }
    }
}

void XBOSCManager::handlePianoMessages()
{
}

void XBOSCManager::handleKinectMessages()
{
}

void XBOSCManager::buildCelloAddresses()
{
    stringstream ssCelloAddrStarted;
    ssCelloAddrStarted << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_STARTED;
    celloAddrStarted = ssCelloAddrStarted.str();

    stringstream ssCelloAddrStopped;
    ssCelloAddrStopped << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_STOPPED;
    celloAddrStopped = ssCelloAddrStopped.str();

    stringstream ssCelloAddrPitchNote;
    ssCelloAddrPitchNote << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_PITCHNOTE;
    celloAddrPitchNote = ssCelloAddrPitchNote.str();

    stringstream ssCelloAddrEnergy;
    ssCelloAddrEnergy << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_ENERGY;
    celloAddrEnergy = ssCelloAddrEnergy.str();

    stringstream ssCelloAddrSilence;
    ssCelloAddrSilence << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_SILENCE;
    celloAddrSilence = ssCelloAddrSilence.str();

    stringstream ssCelloAddrPause;
    ssCelloAddrPause << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_PAUSE;
    celloAddrPause = ssCelloAddrPause.str();

    stringstream ssCelloAddrOnset;
    ssCelloAddrOnset << OSC_CELLO_ADDR_BASE << OSC_ANALYZER_ADDR_ONSET;
    celloAddrOnset = ssCelloAddrOnset.str();
}

void XBOSCManager::buildViolinAddresses()
{
    stringstream ssViolinAddrStarted;
    ssViolinAddrStarted << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_STARTED;
    violinAddrStarted = ssViolinAddrStarted.str();

    stringstream ssViolinAddrStopped;
    ssViolinAddrStopped << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_STOPPED;
    violinAddrStopped = ssViolinAddrStopped.str();

    stringstream ssViolinAddrPitchNote;
    ssViolinAddrPitchNote << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_PITCHNOTE;
    violinAddrPitchNote = ssViolinAddrPitchNote.str();

    stringstream ssViolinAddrEnergy;
    ssViolinAddrEnergy << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_ENERGY;
    violinAddrEnergy = ssViolinAddrEnergy.str();

    stringstream ssViolinAddrSilence;
    ssViolinAddrSilence << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_SILENCE;
    violinAddrSilence = ssViolinAddrSilence.str();

    stringstream ssViolinAddrPause;
    ssViolinAddrPause << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_PAUSE;
    violinAddrPause = ssViolinAddrPause.str();

    stringstream ssViolinAddrOnset;
    ssViolinAddrOnset << OSC_VIOLIN_ADDR_BASE << OSC_ANALYZER_ADDR_ONSET;
    violinAddrOnset = ssViolinAddrOnset.str();
}

void XBOSCManager::buildPianoReceiverAddresses()
{
}

void XBOSCManager::buildKinectReceiverAddresses()
{
}
