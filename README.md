# Moviments Granados
Moviments Granados project by [Xavi Bové](http://xavibove.com).

## Basic Requirements
- openframeworks 0.9
- Xcode 7 (tested) or Linux (untested, but projects have been generated also using Linux ARMv6 and ARMv7 Makefiles)

## Required addons for Subprojects

### PedreraMaster
- ofxOSC (already part of OF)
- [ofxTweenzor](https://github.com/NickHardeman/ofxTweenzor)
- ofxGUI (already part of OF)
- ofxXmlSettings (already part of OF)
- [ofxTraerhysics](https://github.com/martinlindelof/ofxTraerPhysics)
- [ofxColorPicker](https://github.com/julapy/ofxColorPicker)
- [ofxMSAInteractiveObject-PM](https://github.com/kosowski/ofxMSAInteractiveObject-PM)
- [ofxParticles](https://github.com/timscaffidi/ofxParticles)
- [ofxSvg-PM](https://github.com/miquelsoler/ofxSvg-PM)
- [ofxBox2d](https://github.com/vanderlin/ofxBox2d)
- [ofxSyphon](https://github.com/astellato/ofxSyphon)
- [ofxFX](https://github.com/jeonghopark/ofxFX)

### PedreraKinectSender
- ofxGUI (already part of OF)
- [ofxOpenNI-PM](https://github.com/eduardfrigola/ofxOpenNI-PM) 
- ofxOSC (already part of OF)

### PedreraKinectV2Sender_Win
- ofxGUI (already part of OF)
- ofxOSC (already part of OF)
- [ofxKinectForWindows2] (https://github.com/elliotwoods/ofxKinectForWindows2)

### PedreraKinectReceiver
- ofxOSC (already part of OF)

### PedreraMIDISender
- [ofxMidi](https://github.com/danomatika/ofxMidi)
- ofxGUI (already part of OF)
- ofxOSC (already part of OF)

### PedreraAudioAnalyzers
- [ofxAubio](https://github.com/aubio/ofxAubio)
- ofxGUI (already part of OF)
- ofxOSC (already part of OF)
- 

For using ip video for sending a receiving the stream of the kinect you will need:
- of 0.9.1+ (last openframeworks version updated on 30/01/16) on Windows.
- These addons for windows Sender:
  - [ofxHTTP] (https://github.com/bakercp/ofxHTTP) and it's dependencies

- These addons for Mac Receiver:
  - [ofxIpVideoGrabber] (https://github.com/bakercp/ofxIpVideoGrabber)
  
