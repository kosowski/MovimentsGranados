#include "ofMain.h"
#include "ofApp.h"

#include "Defaults.h"

int main()
{
	ofGLFWWindowSettings settings;
	settings.width = int(KINECT_STD_COLOR_WIDTH * WINDOW_SCALE_FACTOR);
	settings.height = int(KINECT_STD_COLOR_HEIGHT * WINDOW_SCALE_FACTOR);
	settings.windowMode = OF_WINDOW;
	settings.resizable = false;

	shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    shared_ptr<ofApp> app(new ofApp);

    ofRunApp(window, app);
    ofRunMainLoop();
}
