#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "utils.h"

constexpr float LEARNING_TIME = 300.0f;
constexpr float THRESHOLD_VALUE = 8.0f;

class RoiFinder : public ofThread {
    public:
        ~RoiFinder();
        void setup(int width, int height);
        void threadedFunction();
        void drawCamera();
        void drawThreshold();
        void stop();

        ofParameterGroup & getParameters();
        std::vector<cv::Point2f> getFeatures();

        void reset();

        void onBgReset(bool & val);
        void onLearningTime(float & val);
        void onBgThreshold(float & val);
        void onThreshBlurAmount(int & val);
        void onContourMin(int & val);
        void onContourMax(int & val);
        void onTrackerPersistance(int & val);
        void onTrackerMaxDist(int & val);
    private:
        int _width, _height;

        ofVideoGrabber _cam;

        ofxCv::RunningBackground _background;
        cv::Mat _thresholded;

        ofxCv::ContourFinder _contourFinder;
        std::vector<cv::Point2f> _features;

        int _threshBlurAmount;

        ofParameterGroup _roiParams = ofParameterGroup("regions_of_interest");
        ofParameter<bool> _param_bgReset;
        ofParameter<float> _param_bgLearningTime;
        ofParameter<float> _param_bgThresholdCutoff;
        ofParameter<int> _param_threshBlurAmount;
        ofParameter<int> _param_contourMinArea, _param_contourMaxArea;
        ofParameter<int> _param_trackerPersistance, _param_trackerMaxDistance;
};