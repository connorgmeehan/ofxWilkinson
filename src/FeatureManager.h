#pragma once

#include "ofMain.h"
#include "ofxCv.h"

template <class UserFollower>
class FeatureManager {
    public:
        void setup();
        void update(std::vector<cv::Point2f> & features);
        void draw(int width, int height);

        ofParameterGroup & getParameters();
        std::vector<UserFollower> & getFollowers();

        void onTrackingPersistance(float & val);
        void onTrackingMaxDistance(float & val);
        void onFeaturePredictionDistance(float & val);
        void onFeaturePredictionSmoothAmount(float & val);
        void onFeatureSmoothAmount(float & val);

        void setOutputScale(glm::vec2 sourceDimensions, glm::vec2 outputDimensions);
    private:

        ofxCv::PointTrackerFollower<UserFollower> _tracker;

        void scaleFeaturestoOutput(std::vector<cv::Point2f> & features);
        cv::Point_<float> _featurePositionScale;

        ofParameterGroup _featureManagerParams = ofParameterGroup("Feature Manager Settings");
        ofParameter<float> _trackingPersistance;
        ofParameter<float> _trackingMaxDistance;
        ofParameter<float> _featurePredictionDistance;
        ofParameter<float> _featurePredictionSmoothingAlpha;
        ofParameter<float> _featureSmoothingAlpha;

};

#include "FeatureManager.hpp"

