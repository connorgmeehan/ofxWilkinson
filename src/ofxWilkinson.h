#pragma once

#include "ofxCv.h"
#include "ofxGui.h"

#include "utils.h"

#include "Profiler.h"

#include "ThreadedWebcam.h"
#include "RoiFinder.h"
#include "PointWarper.h"
#include "FeatureManager.h"
#include "BaseFollower.h"

#define ARTNET_MAX_FPS 44


template <class UserFollower>
class ofxWilkinson {
    public:
        void setup();
        void update();
        void draw(int x, int y);

        void setCameraDimensions(int width, int height);
        void setOutputDimensions(int width, int height, int strands = -1, int strandWidth = -1);

        void exit();

        std::vector<UserFollower> & getFollowers();
    protected:
        int _cameraWidth, _cameraHeight;
        int _outputWidth, _outputHeight;
        int _colSkip, _rowSkip;
        ThreadedWebcam _cam;
        cv::Mat _camOut;

        Profiler _updateProfiler;
        RoiFinder _roiFinder;
        PointWarper _pointWarper;
        FeatureManager<UserFollower> _featureManager;

        bool paramsSet();      

        ofxPanel _gui;
        ofParameterGroup _globalParams = ofParameterGroup("App Settings");
        ofParameter<bool> _drawCam;
        ofParameter<bool> _drawRoi;
};

#include "ofxWilkinson.hpp"

typedef ofxWilkinson<BaseFollower> ofxWilkinsonBasic;