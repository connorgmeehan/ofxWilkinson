
#include "BaseFollower.h"

float BaseFollower::_dyingTime = 1.0f;
float BaseFollower::_predictionDistance;
float BaseFollower::_predictionSmoothingAlpha;
float BaseFollower::_positionSmoothingAlpha;

void BaseFollower::setDyingTime(float dyingTime) {
	_dyingTime = dyingTime;
}

void BaseFollower::setPredictionDistance(float delta) {
	_predictionDistance = delta;
}

void BaseFollower::setPredictionSmoothingAlpha(float alpha) {
	_predictionSmoothingAlpha = alpha;
}

void BaseFollower::setSmoothingAlpha(float alpha) {
	_positionSmoothingAlpha = alpha;
}


void BaseFollower::setup(const cv::Point2f & track) {
    pos = smoothed = lastSmoothed = cur = ofxCv::toOf(track);

	_setup(pos);
}

void BaseFollower::update(const cv::Point2f & track) {
	cur = ofxCv::toOf(track);
	lastSmoothed = smoothed;
	smoothed = glm::mix(smoothed, cur, _positionSmoothingAlpha);

	velocity = glm::mix(smoothed - lastSmoothed, velocity, _predictionSmoothingAlpha);

	pos = smoothed + velocity * _predictionDistance;

	_update(pos);
}

void BaseFollower::kill() {
	_kill();
}

void BaseFollower::draw() {
	_draw();
}

void BaseFollower::drawDebug() {
	ofPushStyle();
		float size = 2;
		ofSetColor(255);
		if(startedDying) {
			ofSetColor(ofColor::red);
			size = ofMap(ofGetElapsedTimef() - startedDying, 0, _dyingTime, size, 0, true);
		}
		ofNoFill();
		// ofDrawCircle(cur, 1);
		ofDrawCircle(smoothed, size);

		ofDrawLine(smoothed, pos);
		
		// ofLog() << "label: " << label << ", 222prediction distance: " << _predictionDistance << ", psa: " << _predictionSmoothingAlpha << ", sa: " << _positionSmoothingAlpha;
	ofPopStyle();
}