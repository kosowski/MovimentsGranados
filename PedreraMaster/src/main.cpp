#include "ofMain.h"
#include "MainApp.h"

#include "Defaults.h"

int main()
{
    ofGLFWWindowSettings mainSettings;
    mainSettings.width = MAIN_WINDOW_WIDTH;
    mainSettings.height = MAIN_WINDOW_HEIGHT;
    mainSettings.windowMode = MAIN_WINDOW_MODE;
    mainSettings.resizable = MAIN_WINDOW_RESIZABLE;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(mainSettings);

    shared_ptr<MainApp> mainApp(new MainApp);
    ofRunApp(mainWindow, mainApp);

    ofRunMainLoop();
}
