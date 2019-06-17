#pragma once

#include "ofMain.h"

// #define OLA_INSTALLED

#include "ofxWilkinson.h"
#include "PresenceFollower.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofCamera cam;

		ofxWilkinson<PresenceFollower> wilkinson;
		ofShader backgroundShader;
		ofPlanePrimitive plane;

		ofFbo followerFbo;

		int inputWidth = 160;
		int inputHeight = 90;

		int outputWidth = 64;
		int outputHeight = 50;
		int outputFboWidth, outputFboHeight;

		int strandCount = 15;
		int strandLength = 50;
		
		float shaderBackgroundStrength = 1.0f;

		void exit();
};

