#pragma once

#include "ofMain.h"

// #define OLA_INSTALLED

#include "ofxWilkinson.h"
#include "RibbonFollower.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofCamera cam;

		ofxWilkinson<RibbonFollower> wilkinson;

		// ofxWilkinson variables

		int inputWidth = 160;
		int inputHeight = 120;

		int outputWidth = 64;
		int outputHeight = 50;
		int outputFboWidth, outputFboHeight;

		int strandCount = 15;
		int strandLength = 50;
		
		// application variables

		float curTime = 0.0f;
		float triggerTime = 0.0f;
		float triggerInterval = 0.1f;

		ofColor bgColor = ofColor::white;
		
		void exit();
};

