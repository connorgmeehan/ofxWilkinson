#pragma once

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;

constexpr int FOLLOWER_HISTORY = 3;

class BaseFollower : public PointFollower {
    protected:
        ofColor color;
        glm::vec2 cur, smoothed, lastSmoothed, velocity, pos;

        float startedDying;

        static float _dyingTime;
        static float _predictionDistance;
        static float _predictionSmoothingAlpha;
        static float _positionSmoothingAlpha;
    public:
        static void setDyingTime(float dyingTime);
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

        virtual void _setup(const glm::vec2 & position) {}
        virtual void _update(const glm::vec2 & position) {}
        virtual void _draw() {}
        virtual void _kill() {
            float curTime = ofGetElapsedTimef();
            if(startedDying == 0) {
                startedDying = curTime;
            } else if(curTime - startedDying > _dyingTime) {
                this->PointFollower::kill();
            }
        }
        void drawDebug();
};
