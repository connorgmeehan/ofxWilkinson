#pragma once

#include "ofxCv.h"
#include "utils.h"

using namespace ofxCv;
using namespace cv;
using namespace glm;

class FlowDetector {
    public:
        void setup(int width, int height, vector<glm::vec2> features);
        void update(cv::Mat & cam);
        void draw(int x, int y);

        std::vector<cv::Point2f> getFeatures();
        void updateFeatures(std::vector<glm::vec2> & dirtyFeatures, std::vector<glm::vec2> & newFeatures);
    protected:
        ofxCv::FlowPyrLK _flow;
        std::vector<cv::Point2f> _featuresToKill;
};