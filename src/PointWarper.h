#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class PointWarper {
    public:
        void setup(int width, int height);
        std::vector<cv::Point2f> & warpPoints(std::vector<cv::Point2f> & toWarp);
        void draw();

        ofParameterGroup & getParameters();

        void onTopLeft(glm::vec2 & val);
        void onTopRight(glm::vec2 & val);
        void onBottomRight(glm::vec2 & val);
        void onBottomLeft(glm::vec2 & val);

        std::vector<cv::Point2f> & getWarped();
    private:
        int _width, _height;
        std::vector<cv::Point2f> _warpedPoints;

        ofParameterGroup _pointWarpParams = ofParameterGroup("Point Warp Settings");
        ofParameter<glm::vec2> _pTopLeft, _pTopRight, _pBottomRight, _pBottomLeft;
        cv::Point2f _srcPoints[4];
        cv::Point2f _destPoints[4];
};