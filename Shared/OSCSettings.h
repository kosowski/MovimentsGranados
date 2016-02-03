//
//  OSCSettings.h
//  MovimentsGranados
//
//  Created by Miquel Angel Soler on 4/1/16.
//
//

#pragma once

#ifndef MovimentsGranados_OSCSettings_h
#define MovimentsGranados_OSCSettings_h

// Cello analyzer
static const string         OSC_CELLO_SENDER_HOST		    = "localhost";
static const int            OSC_CELLO_SENDER_PORT		    = 30301;
static const string         OSC_CELLO_ADDR_BASE             = "/analyzer/cello";

// Violin analyzer
static const string         OSC_VIOLIN_SENDER_HOST  	    = "localhost";
static const int            OSC_VIOLIN_SENDER_PORT          = 30302;
static const string         OSC_VIOLIN_ADDR_BASE            = "/analyzer/violin";

// Analyzer (shared by cello and violin)
static const string         OSC_ANALYZER_ADDR_STARTED       = "/started";
static const string         OSC_ANALYZER_ADDR_STOPPED       = "/stopped";
static const string         OSC_ANALYZER_ADDR_PITCHNOTE     = "/pitchMIDINote";
static const string         OSC_ANALYZER_ADDR_ENERGY        = "/energy";
static const string         OSC_ANALYZER_ADDR_SILENCE       = "/silence";
static const string         OSC_ANALYZER_ADDR_PAUSE         = "/pause";
static const string         OSC_ANALYZER_ADDR_ONSET         = "/onset";

// Piano
static const string         OSC_PIANO_SENDER_HOST           = "localhost";
static const int            OSC_PIANO_SENDER_PORT           = 30303;
static const string         OSC_PIANO_ADDR_BASE             = "/midi";
static const string         OSC_PIANO_ADDR_NOTEON           = "/noteOn";
static const string         OSC_PIANO_ADDR_NOTEOFF          = "/noteOff";
static const string         OSC_PIANO_ADDR_CONTROLCHANGE    = "/controlChange";
static const string         OSC_PIANO_ADDR_PROGRAMCHANGE    = "/programChange";
static const string         OSC_PIANO_ADDR_PITCHBEND        = "/pitchBend";
static const string         OSC_PIANO_ADDR_AFTERTOUCH       = "/aftertouch";
static const string         OSC_PIANO_ADDR_POLYAFTERTOUCH   = "/polyAftertouch";

// Kinect
static const string         OSC_KINECT_SENDER_HOST          = "192.168.1.126";
static const int            OSC_KINECT_SENDER_PORT          = 30304;
static const int            OSC_KINECT_SENDER_PORT_MAX      = 30305;
static const string         OSC_KINECT_ADDR_BASE            = "/motion";
static const string         OSC_KINECT_ADDR_STATE           = "/detectionStatus";
static const string         OSC_KINECT_STATE_SETUP          = "Setting up";
static const string         OSC_KINECT_STATE_DETECTING      = "Detecting";
static const string         OSC_KINECT_STATE_CAPTURING      = "Capturing";
static const string         OSC_KINECT_STATE_POSITIONED     = "Positioned";
static const string         OSC_KINECT_ADDR_LHAND           = "/leftHand";
static const string         OSC_KINECT_ADDR_RHAND           = "/rightHand";
static const string         OSC_KINECT_ADDR_POSITION        = "/position";
static const string         OSC_KINECT_ADDR_VELOCITY        = "/velocity";

#endif
