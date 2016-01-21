#include "ofMain.h"
#include "ViolinApp.h"

#include "Defaults.h"

int main()
{
	ofSetupOpenGL(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_MODE);
	ofRunApp(new ViolinApp());
}
