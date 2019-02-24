#include "RoiFinder.h"

void RoiFinder::setup(int width, int height) {
    _width = width;
    _height = height;

    _thresholded = cv::Mat(width, height, CV_8UC1);

    _roiParams.add(_bgReset.set("Reset Background", false));
    _roiParams.add(_bgLearningTime.set("Learning Time", LEARNING_TIME, 0.0f, 300.0f));
    _roiParams.add(_bgThresholdCutoff.set("Threshold Cutoff", THRESHOLD_VALUE, 0.0f, 20.0f));
    _roiParams.add(_threshBlurAmount.set("Threshold Blur", 32, 1, 80));

    _roiParams.add(_contourMinArea.set("Contour Min Area", 1, 0, 100));
    _roiParams.add(_contourMaxArea.set("Contour Max Area", 100, 0, 100));

    _roiParams.add(_trackerPersistance.set("Tracker Persistance", 15, 5, 30));
    _roiParams.add(_trackerMaxDistance.set("Tracker Max Distance", 2, 0, 10));

    _bgReset.addListener(this, &RoiFinder::onBgReset);
    _bgLearningTime.addListener(this, &RoiFinder::onLearningTime);
    _bgThresholdCutoff.addListener(this, &RoiFinder::onBgThreshold);
    _contourMinArea.addListener(this, &RoiFinder::onContourMin);
    _contourMaxArea.addListener(this, &RoiFinder::onContourMax);
    _trackerPersistance.addListener(this, &RoiFinder::onTrackerPersistance);
    _trackerMaxDistance.addListener(this, &RoiFinder::onTrackerMaxDist);

    _background.setLearningTime(_bgLearningTime);
    _background.setThresholdValue(_bgThresholdCutoff);
    _contourFinder.setMinAreaRadius(_contourMinArea);
    _contourFinder.setMaxAreaRadius(_contourMaxArea);
    _contourFinder.getTracker().setPersistence(_trackerPersistance);
    _contourFinder.getTracker().setMaximumDistance(_trackerMaxDistance);
}

void RoiFinder::update(cv::Mat & mat) {
    _background.update(mat, _thresholded);

    cv::blur(_thresholded, _thresholded, cv::Size(_threshBlurAmount, _threshBlurAmount), cv::Point(-1, -1));

    _contourFinder.findContours(_thresholded);

    _features.resize(_contourFinder.size() );
    for(int i = 0; i < _contourFinder.size(); i++) {
        _features[i] = _contourFinder.getCenter(i);
    }
}

void RoiFinder::draw() {

        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255,255,255,50);

        ofPixels thresholdedPixels;
        ofxCv::toOf(_thresholded, thresholdedPixels);
        ofImage thresholdedImg(thresholdedPixels);
        thresholdedImg.draw(0,0);

        _contourFinder.draw();

        for(auto & p : _features) {
            ofDrawCircle(p.x, p.y, 2);
        }
}

std::vector<cv::Point2f> & RoiFinder::getFeatures() {
    return _features;
}

ofParameterGroup & RoiFinder::getParameters(){
    return _roiParams;
}

void RoiFinder::reset() {
    _bgReset = false;
    _background.reset();
}

void RoiFinder::onBgReset(bool & val){
    if(_bgReset){
        _bgReset = false;
        _background.reset();
    }
}

void RoiFinder::onLearningTime(float & val){
    _background.setLearningTime(val);
}

void RoiFinder::onBgThreshold(float & val){
    _background.setThresholdValue(val);
}

void RoiFinder::onContourMin(int & val){
    _contourFinder.setMinAreaRadius(val);
}

void RoiFinder::onContourMax(int & val){
    _contourFinder.setMaxAreaRadius(val);
}

void RoiFinder::onTrackerPersistance(int & val){
    _contourFinder.getTracker().setPersistence(val);
}

void RoiFinder::onTrackerMaxDist(int & val){
    _contourFinder.getTracker().setMaximumDistance(val);
}
