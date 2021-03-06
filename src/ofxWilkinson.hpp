#include "ofxWilkinson.h"
#include "utils.h"

template <class UserFollower>
ofxWilkinson<UserFollower>::ofxWilkinson() :
    _cameraWidth(0),
    _cameraHeight(0),
    _outputWidth(0),
    _outputHeight(0),
    _outputCols(0),
    _outputRows(0) {
    
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::setup(){
    if(!paramsSet()){
        return;
    }

    ofSetFrameRate(ARTNET_MAX_FPS);

    _updateProfiler.setGoal(1.0f / (float) ARTNET_MAX_FPS);

    _roiFinder.setup(_cameraWidth, _cameraHeight);
    _pointWarper.setup(_cameraWidth, _cameraHeight);

    _featureManager.setOutputScale( glm::vec2(_cameraWidth, _cameraHeight), glm::vec2(_outputWidth, _outputHeight));
    _featureManager.setup();

    _gui.setup("Settings", "ofxWilkinson.json",  ofGetWidth() - 200, 15);
    _globalParams.add(_hideAll.set("hide_all", false));
    _globalParams.add(_drawOscControls.set("draw_osc", true));
    _globalParams.add(_drawGui.set("draw_gui", true));
    _globalParams.add(_drawCam.set("draw_camera", true));
    _globalParams.add(_drawRoi.set("draw_roi", true));
    _globalParams.add(_drawPointWarper.set("draw_pointwarper", true));
    _globalParams.add(_drawFeatureManager.set("draw_featuremanager", true));
    _globalParams.add(_drawFollowers.set("draw_followers", true));
    _globalParams.add(_drawSceneBuilder.set("draw_scenebuilder", true));
    _gui.add(_globalParams);

    // Add parameters for modules
    _gui.add(_roiFinder.getParameters());
    _gui.add(_pointWarper.getParameters());
    _gui.add(_featureManager.getParameters());

    _gui.loadFromFile("ofxWilkinson.json");

    // setup OscController to control parameters in gui
    _oscController = OscController(_gui);
    _oscController.buildCommandList();
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::update(){
    _oscController.update();

    // if(_cam.isFrameNew()){
    _updateProfiler.start();

    if(_isCameraInit == false && ofGetElapsedTimef() > 10.0f) {
        _roiFinder.reset();
        _isCameraInit = true;
    }
    auto features = _roiFinder.getFeatures();
    // Warp the centers of the regions of interest to the output dimensions
    _pointWarper.warpPoints(features);

    // Make feature manager track newly warped features 
    _featureManager.update(_pointWarper.getWarped());
    
    _updateProfiler.stop();
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::draw(int x, int y){
    if(!_hideAll) {
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        
        if(_drawGui) {
            _gui.draw();
        }

        std::string debugString = "";
        debugString += "Do not run in production!  It costs too much.  [" + ofToString(ofGetFrameRate(),2)+"fps]\n";
        debugString += "ofxWilkinson::draw(x:"+ofToString(x)+", y:"+ofToString(y)+")\n";

        ofPushStyle();
            ofSetColor(ofColor::white);
            ofNoFill();
            ofPushMatrix();
                ofTranslate(x,y);
                ofDrawRectangle(0, _cameraHeight, _outputWidth, _outputHeight);

                if(_drawCam) {
                    _roiFinder.drawCamera(); 
                }
                if(_drawOscControls) {
                    _oscController.draw(_cameraWidth, 0);
                }

                if(_drawRoi) { _roiFinder.drawThreshold(); }
                if(_drawPointWarper) { _pointWarper.draw(); }
                if(_drawFeatureManager) {_featureManager.draw(0, _cameraHeight); }
                if(_drawFollowers) {
                    ofTranslate(_outputWidth, _cameraHeight);
                    for(auto & f : _featureManager.getFollowers()) {
                        f.draw();
                    }
                }
                if(_drawSceneBuilder) {
                    ofSetColor(ofColor::white);
                    _sceneBuilder.draw(_outputWidth, 0);
                }

            ofPopMatrix();
        ofPopStyle();

    }
    _updateProfiler.draw(x, y-50, _cameraWidth, 50);
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::drawOutputFrame(int x, int y) {
    _sceneBuilder.draw(x, y);
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::drawFollowers(){
    for( auto & f : _featureManager.getFollowers()) {
        f.draw();
    }
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::setCameraDimensions(int width, int height){
    _cameraWidth = width;
    _cameraHeight = height;
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::setLightingArrayDimensions(int count, int length){
    if(_outputWidth != 0 || _outputHeight != 0) {
        
        // if one dimension not a power of 2, use the other dimension
        int bufferWidth = checkPowerofTwo(_outputWidth) ? _outputWidth : _outputHeight;
        int bufferHeight = checkPowerofTwo(_outputHeight) ? _outputHeight : _outputHeight;

        // if one isn't power's of 2, error
        if(!checkPowerofTwo(bufferWidth) || !checkPowerofTwo(bufferHeight)) {
            ofLog(OF_LOG_ERROR) << "ofxWilkinson::setLightingArrayDimensions -> one of the output dimensions (width: " << _outputWidth << " , height: " << _outputHeight << ") is not a power of 2";
            ofExit(1);
        }

        _sceneBuilder.setup(bufferWidth, bufferHeight, count, length);
    } else {
        ofLog() << "ofxWilkinson::setLightingArrayDimensions() -> you must run setOutputDimensions before running setLightingArrayDimensions";
        ofExit();
    }
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::setOutputDimensions(int cols, int rows, int width, int height){

    if(width == -1 && height == -1) {
        width = cols;
        height = rows;
    }

    _outputWidth = width;
    _outputHeight = height;

    _outputCols = cols;
    _outputRows = rows;
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::bindFrame(){
    _sceneBuilder.bindFrame();
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::unbindFrame(){
    _sceneBuilder.unbindFrame();
}

template <class UserFollower>
bool ofxWilkinson<UserFollower>::paramsSet(){
    ofLog() << "ofxWilkinson checking parameters!";
    ofLog() << "----------------------------------";
    ofLog() << "setCameraDimensions(width, height) - " << (_cameraWidth && _cameraHeight ? "OK!" : "FAIL!");
    if(!_cameraWidth || !_cameraHeight){ return false; }

    ofLog() << "-------------COMPLETE-------------";
    return true;
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::exit(){
    _roiFinder.stopThread();
    _oscController.stop();
}

template <class UserFollower>
std::vector<UserFollower> & ofxWilkinson<UserFollower>::getFollowers(){
    return _featureManager.getFollowers();
}

template<class UserFollower>
size_t ofxWilkinson<UserFollower>::getSize() {
    return _featureManager.getFollowers.size();
}
