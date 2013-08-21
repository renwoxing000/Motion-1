// Main entry for Motion Application
//#include "MotionApp.h"
#include "SampleBrowser.h"

//INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, INT) {
int main(){
	/* 
	    argv[0] [ignored] the command to run this application
	    argv[1] startSampleIndex passed to MotionApp()
	 */
 //   int argc = __argc;
 //   char** argv = __argv;
	//int startSampleIndex = (argc >= 2) ? startSampleIndex = Ogre::StringConverter::parseInt(Ogre::String(argv[2]), -1) : -1;

	//MotionApp app(startSampleIndex);
	OgreBites::SampleBrowser app;
	app.go();
	return 0;
}
