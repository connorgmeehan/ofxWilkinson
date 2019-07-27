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

		int outputWidth = 50;
		int outputHeight = 64;

		int strandCount = 15;
		int strandLength = 50;
		
		// application variables
		ofPlanePrimitive plane;
		ofShader backgroundShader;
		ofFbo followerFbo;
		
		float curTime = 0.0f;
		float triggerTime = 0.0f;
		float triggerInterval = 0.1f;
		float bgStrength = 0;
		float bgFadeTime = 10.0f; // in seconds
		float timeOfLastBackgroundReset = 0;

		ofFloatColor bgColor = ofColor::white;
		
		void exit();
};

