#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "utils.h"

constexpr float LEARNING_TIME = 0.5f;
constexpr float THRESHOLD_VALUE = 7.0f;

class RoiFinder {
    public:
        void setup(int width, int height);
        void update(cv::Mat & mat);
        void draw(int x, int y);

        ofParameterGroup & getParameters();
    private:
        int _width, _height;
        ofxCv::RunningBackground _background;
        cv::Mat _thresholded;

        ofParameterGroup _roiParams = ofParameterGroup("Roi Settings");
        ofParameter<bool> _bgReset;
        ofParameter<float> _bgLearningTime;
        ofParameter<float> _bgThresholdCutoff;
};