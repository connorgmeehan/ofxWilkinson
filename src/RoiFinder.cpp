#include "RoiFinder.h"

RoiFinder::~RoiFinder() {
    stopThread();
    waitForThread(false);
}

void RoiFinder::setup(int width, int height) {
    _width = width;
    _height = height;

    _cam.setup(width, height);

    _thresholded = cv::Mat(width, height, CV_8UC1);

    _roiParams.add(_param_bgReset.set("reset_background", false));
    _roiParams.add(_param_bgLearningTime.set("learning_time", LEARNING_TIME, 0.0f, 300.0f));
    _roiParams.add(_param_bgThresholdCutoff.set("threshold_cutoff", THRESHOLD_VALUE, 0.0f, 20.0f));
    _roiParams.add(_param_threshBlurAmount.set("threshold_blur", 32, 1, 80));
    _roiParams.add(_param_contourMinArea.set("contour_area_min", 1, 0, 100));
    _roiParams.add(_param_contourMaxArea.set("contour_area_max", 200, 0, 100));
    _roiParams.add(_param_trackerPersistance.set("tracker_persistance", 15, 5, 30));
    _roiParams.add(_param_trackerMaxDistance.set("tracker_distance_max", 2, 0, 10));

    _param_bgReset.addListener(this, &RoiFinder::onBgReset);
    _param_bgLearningTime.addListener(this, &RoiFinder::onLearningTime);
    _param_bgThresholdCutoff.addListener(this, &RoiFinder::onBgThreshold);
    _param_threshBlurAmount.addListener(this, &RoiFinder::onThreshBlurAmount);
    _param_contourMinArea.addListener(this, &RoiFinder::onContourMin);
    _param_contourMaxArea.addListener(this, &RoiFinder::onContourMax);
    _param_trackerPersistance.addListener(this, &RoiFinder::onTrackerPersistance);
    _param_trackerMaxDistance.addListener(this, &RoiFinder::onTrackerMaxDist);

    _threshBlurAmount = _param_threshBlurAmount;

    _background.setLearningTime(_param_bgLearningTime);
    _background.setThresholdValue(_param_bgThresholdCutoff);
    _contourFinder.setMinAreaRadius(_param_contourMinArea);
    _contourFinder.setMaxAreaRadius(_param_contourMaxArea);
    _contourFinder.getTracker().setPersistence(_param_trackerPersistance);
    _contourFinder.getTracker().setMaximumDistance(_param_trackerMaxDistance);

    startThread();
}

void RoiFinder::threadedFunction() {
    while(isThreadRunning()) {
        _cam.update();
        if(_cam.isFrameNew()) {

            cv::Mat frame = ofxCv::toCv(_cam.getPixels());

            _background.update(frame, _thresholded);
            if(_threshBlurAmount > 0) {
                cv::blur(_thresholded, _thresholded, cv::Size(_threshBlurAmount, _threshBlurAmount), cv::Point(-1, -1));
            }

            _contourFinder.findContours(_thresholded);

            lock();
            _features.resize(_contourFinder.size() );
            for(int i = 0; i < _contourFinder.size(); i++) {
                _features[i] = _contourFinder.getCenter(i);
            }
            unlock();
        }
    }
}

void RoiFinder::drawCamera() {
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    ofSetColor(255,255,255,255);
    lock();
    auto temp = ofImage(_cam.getPixels());
    unlock();
    temp.draw(0,0);
}

void RoiFinder::drawThreshold() {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255,255,255,150);
    lock();

    ofPixels thresholdedPixels;
    ofxCv::toOf(_thresholded, thresholdedPixels);
    ofImage thresholdedImg(thresholdedPixels);
    thresholdedImg.draw(0,0);

    _contourFinder.draw();

    for(auto & p : _features) {
        ofDrawCircle(p.x, p.y, 2);
    }
    unlock();
}

void RoiFinder::stop() {
    stopThread();
}

std::vector<cv::Point2f> RoiFinder::getFeatures() {
    return _features;
}

ofParameterGroup & RoiFinder::getParameters(){
    return _roiParams;
}

void RoiFinder::reset() {
    lock();
    _param_bgReset = false;
    _background.reset();
    unlock();
}

void RoiFinder::onBgReset(bool & val){
    if(_param_bgReset){
        reset();
    }
}

void RoiFinder::onLearningTime(float & val){
    lock();
    _background.setLearningTime(val);
    unlock();
}

void RoiFinder::onBgThreshold(float & val){
    lock();
    _background.setThresholdValue(val);
    unlock();
}

void RoiFinder::onThreshBlurAmount(int & val){
    lock();
    _threshBlurAmount = val;
    unlock();
}

void RoiFinder::onContourMin(int & val){
    lock();
    _contourFinder.setMinAreaRadius(val);
    unlock();
}

void RoiFinder::onContourMax(int & val){
    lock();
    _contourFinder.setMaxAreaRadius(val);
    unlock();
}

void RoiFinder::onTrackerPersistance(int & val){
    lock();
    _contourFinder.getTracker().setPersistence(val);
    unlock();
}

void RoiFinder::onTrackerMaxDist(int & val){
    lock();
    _contourFinder.getTracker().setMaximumDistance(val);
    unlock();
}
