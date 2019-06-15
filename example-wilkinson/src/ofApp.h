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

		ofxWilkinson<PresenceFollower> wilkinson;
		ofShader backgroundShader;

		ofFbo followerFbo;

		int inputWidth = 640;
		int inputHeight = 360;

		int outputWidth = 64;
		int outputHeight = 50;

		float shaderBackgroundStrength = 1.0f;

		void exit();
};

