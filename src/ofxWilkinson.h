#pragma once

#include "ofxCv.h"
#include "ofxGui.h"

#include "utils.h"

#include "ThreadedWebcam.h"
#include "RoiFinder.h"

class ofxWilkinson {
    public:
        void setup();
        void update();
        void draw(int x, int y);

        void setDimensions(int width, int height);

        void exit();
    protected:
        int _width, _height;
        ThreadedWebcam _cam;
        cv::Mat _camOut;

        RoiFinder _roiFinder;

        bool paramsSet();      

        ofxPanel _gui;
        ofParameterGroup _globalParams = ofParameterGroup("App Settings");
        ofParameter<bool> _drawCam;
        ofParameter<bool> _drawRoi;
};