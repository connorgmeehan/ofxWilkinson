#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "utils.h"

constexpr float LEARNING_TIME = 300.0f;
constexpr float THRESHOLD_VALUE = 8.0f;

class RoiFinder {
    public:
        void setup(int width, int height);
        void update(cv::Mat & mat);
        void draw();

        ofParameterGroup & getParameters();
        std::vector<cv::Point2f> & getFeatures();

        void onBgReset(bool & val);
        void onLearningTime(float & val);
        void onBgThreshold(float & val);
        void onContourMin(int & val);
        void onContourMax(int & val);
        void onTrackerPersistance(int & val);
        void onTrackerMaxDist(int & val);
    private:
        int _width, _height;
        ofxCv::RunningBackground _background;
        cv::Mat _thresholded;

        ofxCv::ContourFinder _contourFinder;
        std::vector<cv::Point2f> _features;

        ofParameterGroup _roiParams = ofParameterGroup("ROI Settings");
        ofParameter<bool> _bgReset;
        ofParameter<float> _bgLearningTime;
        ofParameter<float> _bgThresholdCutoff;
        ofParameter<int> _threshBlurAmount;

        ofParameter<int> _contourMinArea, _contourMaxArea;
        ofParameter<int> _trackerPersistance, _trackerMaxDistance;
};