#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include <boost/circular_buffer.hpp>
using namespace ofxCv;

constexpr float DYING_TIME = 1.0f;
constexpr int FOLLOWER_HISTORY = 3;

class BaseFollower : public PointFollower {
    protected:
        ofColor color;
        glm::vec2 cur, smoothed, lastSmoothed, velocity, pos;

        float startedDying;

        static float _predictionDistance;
        static float _predictionSmoothingAlpha;
        static float _positionSmoothingAlpha;
    public:
        static void setPredictionDistance(float delta);
        static void setPredictionSmoothingAlpha(float alpha);
        static void setSmoothingAlpha(float alpha);
        
        BaseFollower()
            :startedDying(0) {
        }

        void setup(const cv::Point2f & track);
        void update(const cv::Point2f & track);
        void draw();
        void kill();

        virtual void _setup(const cv::Point2f & track) {}
        virtual void _update(const cv::Point2f & track) {}
        virtual void _draw() {}
        virtual void _kill() {
            float curTime = ofGetElapsedTimef();
            if(startedDying == 0) {
                startedDying = curTime;
            } else if(curTime - startedDying > DYING_TIME) {
                this->PointFollower::kill();
            }
        }
        void drawDebug();
};
