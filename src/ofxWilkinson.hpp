
template <class UserFollower>
void ofxWilkinson<UserFollower>::setup(){
    if(!paramsSet()){
        return;
    }

    ofSetFrameRate(ARTNET_MAX_FPS);

    // UserFollower::setScale(glm::vec2(_cameraWidth, _cameraHeight), glm::vec2(_outputWidth, _outputHeight));
    UserFollower::setScale(glm::vec2(_cameraWidth, _cameraHeight), glm::vec2(_cameraWidth, _cameraHeight));

    _updateProfiler.setGoal(1.0f / (float) ARTNET_MAX_FPS);

    _cam.setup(_cameraWidth, _cameraHeight);
    _camOut = cv::Mat(_cameraWidth, _cameraHeight, CV_8UC3);

    _roiFinder.setup(_cameraWidth, _cameraHeight);
    _pointWarper.setup(_cameraWidth, _cameraHeight);
    _featureManager.setup();

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
        // Run background diff and contours to find regions of interest
        _roiFinder.update(_camOut);
        // Warp the centers of the regions of interest to the space
        _pointWarper.warpPoints(_roiFinder.getFeatures());
        // 
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
        ofPushMatrix();
            ofTranslate(x,y);

            debugString += "_camOut channels size: " + ofToString(_camOut.channels()) + ", _camOut.type(): " + cv::type2str(_camOut.type()) + "\n";
            ofFloatImage tempCam;
            tempCam.allocate(_cameraWidth, _cameraHeight, OF_IMAGE_GRAYSCALE);
            ofxCv::toOf(_camOut, tempCam);        
            _cam.draw(0,0);

            _roiFinder.draw();
            _pointWarper.draw();
            _featureManager.draw(_cameraWidth, _cameraHeight);

        ofPopMatrix();
    ofPopStyle();

    _updateProfiler.draw(x, y-50, _cameraWidth, 50);
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::setCameraDimensions(int width, int height){
    _cameraWidth = width;
    _cameraHeight = height;
}

template <class UserFollower>
void ofxWilkinson<UserFollower>::setOutputDimensions(int width, int height, int strands, int strandWidth){
    _outputWidth = width;
    _outputHeight = height;

    _rowSkip = (strands != -1 ? _outputHeight / strands : 0);
    _colSkip = (strandWidth != -1 ? _outputWidth / strandWidth : 0);
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

