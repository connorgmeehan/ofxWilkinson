#pragma once

#include "ofMain.h"

#include "ofxWilkinson.h"
#include "PresenceFollower.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofxWilkinson<PresenceFollower> wilkinson;

		void exit();
};

