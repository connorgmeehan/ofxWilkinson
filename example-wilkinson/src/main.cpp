#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	// #ifdef __arm__
	#ifdef TARGET_LINUX_ARM 
		ofGLESWindowSettings settings;
		settings.glesVersion = 2;
	#else
		ofGLWindowSettings settings;
		settings.setGLVersion(3,2);
	#endif

	settings.setSize(1024,768);
	ofCreateWindow(settings);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
