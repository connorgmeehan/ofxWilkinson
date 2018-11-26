#include "./ofxWilkinson.h"

void ofxWilkinson::setup(){
    if(!paramsSet()){
        return;
    }

    _padding = ( _padding == glm::vec2(-1, -1) ? glm::vec2(_width/15, _height/15) : _padding );

    _featureManager.setPadding(_padding);
    _featureManager.setup( 5, 5, _width, _height);
    _cam.setup(_width, _height);
    _camOut = cv::Mat(_width, _height, CV_8UC3);
    _flow.setup(_width, _height, _featureManager.getInitialFeatures());

}

void ofxWilkinson::update(){
    if(_cam.isFrameNew()){
        _camOut = ofxCv::toCv(_cam.getPixels());
        _flow.update(_camOut);
        _tracker.update( _flow.getFeatures() );
    }
}

void ofxWilkinson::draw(int x, int y){
    
    std::string arrayString = "";

    for(auto & f : _flow.getFeatures()) {
        arrayString += "feature: " + to_string(f.x) + ", " + to_string(f.y) + " \n";
    }

    ofDrawBitmapStringHighlight(arrayString, 100 + 640, 0);

    std::string debugString = "";
    debugString += "Do not run in production!  It costs too much.  [" + ofToString(ofGetFrameRate(),2)+"fps]\n";
    debugString += "ofxWilkinson::draw(x:"+ofToString(x)+", y:"+ofToString(y)+")\n";

    ofPushStyle();
        ofSetColor(ofColor::white);
        
        debugString += "_camOut channels size: " + ofToString(_camOut.channels()) + ", _camOut.type(): " + type2str(_camOut.type()) + "\n";
        ofFloatImage tempCam;
        tempCam.allocate(_width, _height, OF_IMAGE_GRAYSCALE);
        ofxCv::toOf(_camOut, tempCam);        
        _cam.draw(x,y);


        
        ofSetColor(ofColor::red);
        _flow.draw(x, y);
        
        ofSetColor(ofColor::green);
        ofNoFill();
        debugString += "_tracker follower size: " + ofToString(_tracker.size()) + "\n";
        _tracker.draw(x, y);

        ofSetColor(ofColor::blue);
        ofNoFill();
        _featureManager.draw(x, y);

        ofDrawBitmapStringHighlight(debugString,0,12);
    ofPopStyle();
}

void ofxWilkinson::setDimensions(int width, int height){
    _width = width;
    _height = height;
}

void ofxWilkinson::setPadding(glm::vec2 padding) {
    _padding = padding;
}

void ofxWilkinson::setPadding(int x, int y) {
    setPadding(glm::vec2(x, y));
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

// Converts open CV types to readable string i.e. CV_8UC3
// Credit to Octopus on stack overflow
// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv 

std::string ofxWilkinson::type2str(int type){
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}