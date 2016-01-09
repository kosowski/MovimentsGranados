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
- [ofxColorPicker](https://github.com/julapy/ofxColorPicker) This addon requires [ofxMSAInteractiveObject] but not the current version. The correct version in included in ofxColorPicker, inside the folder  'examples/addons'. It must be copied to openFrameworks addon folder.

### PedreraKinectSender
- ofxGUI (already part of OF)
- [ofxKinectFeatures-PM](https://github.com/eduardfrigola/ofxKinectFeatures-PM)
- [ofxOpenNI-PM](https://github.com/eduardfrigola/ofxOpenNI-PM) 
- ofxOSC (already part of OF)

### PedreraMIDISender
- [ofxMidi](https://github.com/danomatika/ofxMidi)
- ofxGUI (already part of OF)
- ofxOSC (already part of OF)

### PedreraCelloAnalyzer and PedreraViolinAnalyzer
- [ofxAubio](https://github.com/aubio/ofxAubio)
- ofxGUI (already part of OF)
- ofxOSC (already part of OF)

### PedreraGUIController
- ofxGUI (already part of OF)
- ofxOSC (already part of OF)
