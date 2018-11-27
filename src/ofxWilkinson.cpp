#include "./ofxWilkinson.h"

void ofxWilkinson::setup(){
    if(!paramsSet()){
        return;
    }

    _cam.setup(_width, _height);
    _camOut = cv::Mat(_width, _height, CV_8UC3);

    _roiFinder.setup(_width, _height);
    _pointWarper.setup(_width, _height);

    _gui.setup("Settings");
    _globalParams.add(_drawCam.set("Draw Camera", true));
    _globalParams.add(_drawRoi.set("Draw Region of Interest Detection", true));
    _gui.add(_globalParams);

    // Add parameters for modules
    _gui.add(_roiFinder.getParameters());
    _gui.add(_pointWarper.getParameters());
}

void ofxWilkinson::update(){
    if(_cam.isFrameNew()){
        _camOut = ofxCv::toCv(_cam.getPixels());
        _roiFinder.update(_camOut);
        _pointWarper.warpPoints(_roiFinder.getFeatures());
    }
}

void ofxWilkinson::draw(int x, int y){

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
            tempCam.allocate(_width, _height, OF_IMAGE_GRAYSCALE);
            ofxCv::toOf(_camOut, tempCam);        
            _cam.draw(0,0);

            _roiFinder.draw();
            _pointWarper.draw();

        ofPopMatrix();
    ofPopStyle();
}

void ofxWilkinson::setDimensions(int width, int height){
    _width = width;
    _height = height;
}

bool ofxWilkinson::paramsSet(){
    ofLog() << "ofxWilkinson checking parameters!";
    ofLog() << "----------------------------------";
    ofLog() << "setDimensions(width, height) - " << (_width && _height ? "OK!" : "FAIL!");
    if(!_width || !_height){ return false; }

    ofLog() << "-------------COMPLETE-------------";
    return true;
}

void ofxWilkinson::exit(){
    _cam.stopThread();
}