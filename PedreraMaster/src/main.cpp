#include "ofMain.h"
#include "ofApp.h"

#include "Defaults.h"

int main()
{
	ofSetupOpenGL(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, MAIN_WINDOW_MODE);
	ofRunApp(new ofApp());
}
