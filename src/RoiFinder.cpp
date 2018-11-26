#include "RoiFinder.h"

void RoiFinder::setup(int width, int height) {
    _width = width;
    _height = height;

    _background.setLearningTime(LEARNING_TIME);
    _background.setThresholdValue(THRESHOLD_VALUE);

    _thresholded = cv::Mat(width, height, CV_8UC1);

    _roiParams.add(_bgReset.set("Reset Background", false));
    _roiParams.add(_bgLearningTime.set("Learning Time", LEARNING_TIME, 0.0f, 5.0f));
    _roiParams.add(_bgThresholdCutoff.set("Learning Time", THRESHOLD_VALUE, 0.0f, 20.0f));
}

void RoiFinder::update(cv::Mat & mat) {
    _background.update(mat, _thresholded);
    
}

void RoiFinder::draw(int x, int y) {

    ofPushMatrix();
    ofTranslate(x,y);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255,255,255,125);

        ofPixels thresholdedPixels;
        ofxCv::toOf(_thresholded, thresholdedPixels);
        ofImage thresholdedImg(thresholdedPixels);
        thresholdedImg.draw(0,0);
    ofPopMatrix();
}

ofParameterGroup & RoiFinder::getParameters(){
    return _roiParams;
}