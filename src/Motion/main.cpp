// Main entry for Motion Application
#include "MotionApp.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, INT) {
	/* 
	    argv[0] [ignored] the command to run this application
	    argv[1] startSampleIndex passed to MotionApp()
	 */
    int argc = __argc;
    char** argv = __argv;
	int startSampleIndex = (argc >= 2) ? startSampleIndex = Ogre::StringConverter::parseInt(Ogre::String(argv[2]), -1) : -1;

	OgreBites::SampleBrowser app(false, startSampleIndex);
	app.go();
	return 0;
}
