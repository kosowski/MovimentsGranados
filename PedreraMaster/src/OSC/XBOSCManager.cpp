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
            ofNotifyEvent(eventCelloStarted, this);

//            ofNotifyEvent(eventCelloStarted);
        }
        else if (address == celloAddrStopped) {
            ofNotifyEvent(eventCelloStopped);
        }
        else if (address == celloAddrPitchNote) {
            float pitchMidiNote = msg.getArgAsFloat(0);
            ofNotifyEvent(eventCelloPitchChanged, pitchMidiNote, this);
        }
        else if (address == celloAddrEnergy) {
            float energy = msg.getArgAsFloat(0);
            ofNotifyEvent(eventCelloEnergyChanged, energy);
        }
        else if (address == celloAddrSilence) {
            bool isSilent = msg.getArgAsBool(0);
            ofNotifyEvent(eventCelloSilenceChanged, isSilent);
        }
        else if (address == celloAddrPause) {
            bool isPaused = msg.getArgAsBool(0);
            ofNotifyEvent(eventCelloPauseChanged, isPaused);
        }
        else if (address == celloAddrOnset) {
            ofNotifyEvent(eventCelloOnsetDetected);
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
            ofNotifyEvent(eventViolinStarted);
        }
        else if (address == violinAddrStopped) {
            ofNotifyEvent(eventViolinStopped);
        }
        else if (address == violinAddrPitchNote) {
            float pitchMidiNote = msg.getArgAsFloat(0);
            ofNotifyEvent(eventViolinPitchChanged, pitchMidiNote);
        }
        else if (address == violinAddrEnergy) {
            float energy = msg.getArgAsFloat(0);
            ofNotifyEvent(eventViolinEnergyChanged, energy);
        }
        else if (address == violinAddrSilence) {
            bool isSilent = msg.getArgAsBool(0);
            ofNotifyEvent(eventViolinSilenceChanged, isSilent);
        }
        else if (address == violinAddrPause) {
            bool isPaused = msg.getArgAsBool(0);
            ofNotifyEvent(eventViolinPauseChanged, isPaused);
        }
        else if (address == violinAddrOnset) {
            ofNotifyEvent(eventViolinOnsetDetected);
        }
    }
}

void XBOSCManager::handlePianoMessages()
{
    while (pianoReceiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        pianoReceiver.getNextMessage(&msg);
        string address = msg.getAddress();

        if (address == pianoAddrNoteOn) {
            PianoNoteOnArgs args;
            args.pitch = msg.getArgAsInt(0);
            args.velocity = msg.getArgAsInt(1);
            ofNotifyEvent(eventPianoNoteOn, args);
        }
        else if (address == pianoAddrNoteOff) {
            int pitch = msg.getArgAsInt(0);
            ofNotifyEvent(eventPianoNoteOff, pitch);
        }
    }
}

void XBOSCManager::handleKinectMessages()
{
    while (kinectReceiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        kinectReceiver.getNextMessage(&msg);
        string address = msg.getAddress();
    }
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
    stringstream ssPianoAddrNoteOn;
    ssPianoAddrNoteOn << OSC_PIANO_ADDR_BASE << OSC_PIANO_ADDR_NOTEON;
    pianoAddrNoteOn = ssPianoAddrNoteOn.str();

    stringstream ssPianoAddrNoteOff;
    ssPianoAddrNoteOff << OSC_PIANO_ADDR_BASE << OSC_PIANO_ADDR_NOTEOFF;
    pianoAddrNoteOff = ssPianoAddrNoteOff.str();
}

void XBOSCManager::buildKinectReceiverAddresses()
{
}
