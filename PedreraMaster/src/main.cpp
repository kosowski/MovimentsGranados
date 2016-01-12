#include "ofMain.h"
#include "MainApp.h"
#include "GUIApp.h"
#include "Defaults.h"

int main()
{
    ofGLFWWindowSettings mainSettings;
    mainSettings.width = MAIN_WINDOW_WIDTH;
    mainSettings.height = MAIN_WINDOW_HEIGHT;
    mainSettings.windowMode = MAIN_WINDOW_MODE;
    mainSettings.resizable = MAIN_WINDOW_RESIZABLE;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(mainSettings);

    ofGLFWWindowSettings guiSettings;
    guiSettings.width = GUI_WINDOW_WIDTH;
    guiSettings.height = GUI_WINDOW_HEIGHT;
    guiSettings.windowMode = GUI_WINDOW_MODE;
    guiSettings.resizable = GUI_WINDOW_RESIZABLE;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(guiSettings);

    shared_ptr<GUIApp> guiApp(new GUIApp);
    ofRunApp(guiWindow, guiApp);

    shared_ptr<MainApp> mainApp(new MainApp);
    ofRunApp(mainWindow, mainApp);

    ofRunMainLoop();
}
