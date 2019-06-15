#pragma once

#include "ofxCv.h"
#include "ofxGui.h"

#include "utils.h"

#include "Profiler.h"
#include "OscController.h"

#include "ThreadedWebcam.h"
#include "RoiFinder.h"
#include "PointWarper.h"
#include "FeatureManager.h"
#include "SceneBuilder.h"

#include "BaseFollower.h"


#define ARTNET_MAX_FPS 44

template <class UserFollower>
class ofxWilkinson {
    public:
        ofxWilkinson();
        void setup();
        void update();
        void draw(int x, int y);
        void drawFollowers();

        void setCameraDimensions(int width, int height);
        void setOutputDimensions(int cols, int rows, int width = -1, int height = -1);

        void bindFrame();
        void unbindFrame();

        void exit();

        std::vector<UserFollower> & getFollowers();
        size_t getSize();
    protected:
        bool _isCameraInit = false;
        int _cameraWidth, _cameraHeight;
        int _outputWidth, _outputHeight;
        int _outputCols, _outputRows;
        ThreadedWebcam _cam;
        cv::Mat _camOut;

        Profiler _updateProfiler;

        RoiFinder _roiFinder;
        PointWarper _pointWarper;
        FeatureManager<UserFollower> _featureManager;
        SceneBuilder _sceneBuilder;

        bool paramsSet();      

        ofxPanel _gui;
        ofParameterGroup _globalParams = ofParameterGroup("general");
        ofParameter<bool> _hideAll;
        ofParameter<bool> _drawOscControls;
        ofParameter<bool> _drawGui;
        ofParameter<bool> _drawCam;
        ofParameter<bool> _drawRoi;
        ofParameter<bool> _drawPointWarper;
        ofParameter<bool> _drawFeatureManager;
        ofParameter<bool> _drawFollowers;
        ofParameter<bool> _drawSceneBuilder;
        OscController _oscController;
};

#include "ofxWilkinson.hpp"

typedef ofxWilkinson<BaseFollower> ofxWilkinsonBasic;