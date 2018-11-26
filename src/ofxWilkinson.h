#pragma once

#include "ofxCv.h"

#include "FeatureManager.h"
#include "ThreadedWebcam.h"
#include "FlowDetector.h"
#include "FlowTracker.h"

class ofxWilkinson {
    public:
        void setup();
        void update();
        void draw(int x, int y);

        void setDimensions(int width, int height);
        void setPadding(glm::vec2 padding);
        void setPadding(int x, int y);

        void exit();
    protected:
        int _width, _height;
        glm::vec2 _padding = glm::vec2(-1, -1);
        ThreadedWebcam _cam;
        cv::Mat _camOut;
        FeatureManager _featureManager;
        FlowDetector _flow;
        FlowTracker _tracker;

        bool paramsSet();      

        std::string type2str(int type);
};