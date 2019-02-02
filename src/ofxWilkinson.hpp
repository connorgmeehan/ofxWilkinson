
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

    _cam.setup(_cameraWidth, _cameraHeight);
    _camOut = cv::Mat(_cameraWidth, _cameraHeight, CV_8UC3);

    _roiFinder.setup(_cameraWidth, _cameraHeight);
    _pointWarper.setup(_cameraWidth, _cameraHeight);

    _featureManager.setOutputScale( glm::vec2(_cameraWidth, _cameraHeight), glm::vec2(_outputWidth, _outputHeight));
    _featureManager.setup();

    _sceneBuilder.setup(_outputWidth, _outputHeight);

    _gui.setup("Settings", "backend_settings.xml",  ofGetWidth() - 200, 15);
    _globalParams.add(_drawCam.set("Draw Camera", true));
    _globalParams.add(_drawRoi.set("Draw Region of Interest Detection", true));
    _gui.add(_globalParams);

    // Add parameters for modules
    _gui.add(_roiFinder.getParameters());
    _gui.add(_pointWarper.getParameters());
    _gui.add(_featureManager.getParameters());
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::update(){
    if(_cam.isFrameNew()){
        _updateProfiler.start();

        // Get the camera output
        _camOut = ofxCv::toCv(_cam.getPixels());

        if(_isCameraInit == false && ofGetElapsedTimef() > 1.0f) {
            _roiFinder.reset();
            _isCameraInit = true;
        }    

        // Run background diff and contours to find regions of interest
        _roiFinder.update(_camOut);

        // Warp the centers of the regions of interest to the output dimensions
        _pointWarper.warpPoints(_roiFinder.getFeatures());

        // Make feature manager track newly warped features 
        _featureManager.update(_pointWarper.getWarped());
        
        _updateProfiler.stop();
    }
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::draw(int x, int y){

    ofEnableBlendMode(OF_BLENDMODE_DISABLED);

    _gui.draw();

    std::string debugString = "";
    debugString += "Do not run in production!  It costs too much.  [" + ofToString(ofGetFrameRate(),2)+"fps]\n";
    debugString += "ofxWilkinson::draw(x:"+ofToString(x)+", y:"+ofToString(y)+")\n";

    ofPushStyle();
        ofSetColor(ofColor::white);
        ofNoFill();
        ofPushMatrix();
            ofTranslate(x,y);
            ofDrawRectangle(0, _cameraHeight, _outputWidth, _outputHeight);

            debugString += "_camOut channels size: " + ofToString(_camOut.channels()) + ", _camOut.type(): " + cv::type2str(_camOut.type()) + "\n";
            ofFloatImage tempCam;
            tempCam.allocate(_cameraWidth, _cameraHeight, OF_IMAGE_GRAYSCALE);
            ofxCv::toOf(_camOut, tempCam);        
            _cam.draw(0,0);

            _roiFinder.draw();
            _pointWarper.draw();
            _featureManager.draw(_cameraWidth, _cameraHeight);
            
            ofTranslate(_outputWidth, _cameraHeight);
            for(auto & f : _featureManager.getFollowers()) {
                f.draw();
            }

        ofPopMatrix();
    ofPopStyle();

    _updateProfiler.draw(x, y-50, _cameraWidth, 50);
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
ofFbo & ofxWilkinson<UserFollower>::getOutputFbo(){
    _sceneBuilder.getOutputFbo();
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
    _cam.stopThread();
}

template <class UserFollower>
std::vector<UserFollower> & ofxWilkinson<UserFollower>::getFollowers(){
    return _featureManager.getFollowers();
}