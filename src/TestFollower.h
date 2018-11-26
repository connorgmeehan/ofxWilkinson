#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "utils.h" // cv::mix

constexpr float SMOOTH_AMOUNT = 0.5f;
constexpr float SMOOTH_VALUE = 1.0f - SMOOTH_AMOUNT;

class TestFollower : public ofxCv::PointFollower {
    public:
        void setup(const cv::Point2f & track);
        void update(const cv::Point2f& track);
        void kill();
        void draw();

        static void setOnDeath(std::function<void(cv::Point2f &)> onDeath);
        static void setOnLife(std::function<void(cv::Point2f &)> onLife);
    private:
        cv::Point2f _curpos, _smoothed;
        float _totalDistance = 0.0f;
        
        static std::function<void(cv::Point2f &)> _onDeath;
        static std::function<void(cv::Point2f &)> _onLife;

};