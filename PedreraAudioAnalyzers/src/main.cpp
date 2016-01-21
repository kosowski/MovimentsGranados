#include "ofMain.h"
#include "Defaults.h"

#if defined(INSTRUMENT_CELLO)
#include "CelloApp.h"
#elif defined(INSTRUMENT_VIOLIN)
#include "ViolinApp.h"
#endif

int main()

{
    ofSetupOpenGL(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_MODE);

#if defined(INSTRUMENT_CELLO)
    ofRunApp(new CelloApp());
#elif defined(INSTRUMENT_VIOLIN)
    ofRunApp(new ViolinApp());
#endif
}
