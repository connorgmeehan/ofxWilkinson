#pragma once

#include "ofMain.h"
#include "ofThread.h";

class ThreadedWebcam : public ofThread {
    public:
        void setup(int width, int height);
        void stop();

        void threadedFunction();
        void draw(int x, int y);

        bool isFrameNew();
        ofPixels getPixels();
    protected:
        int _width, _height;
        bool _isFrameNew = false;
        ofVideoGrabber _cam;
        ofPixels _camOut;
};