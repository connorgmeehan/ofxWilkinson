#pragma once

#include "ofMain.h"

class Profiler {
    public:
        void setGoal(float goalTime) {
            _goalTime = goalTime;
        }

        void start() { _startTime = ofGetElapsedTimef(); }
        void stop() { _stopTime = ofGetElapsedTimef(); }
        void draw(int x, int y, int width, int height){
            ofSetColor(ofColor::white);
            ofNoFill();
            ofDrawBitmapString( ofToString(_goalTime) + "ms", x,y );
            ofDrawRectangle(x,y,width,height);

            ofSetColor(ofColor::red);
            ofDrawRectangle(x,y,(_stopTime-_startTime)/_goalTime * width, height);
        }

    private:
        float _goalTime;
        float  _startTime, _stopTime;
};