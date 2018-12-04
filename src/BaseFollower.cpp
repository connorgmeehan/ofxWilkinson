
#include "BaseFollower.h"

glm::vec2 BaseFollower::_scale;
float BaseFollower::_predictionDistance;
float BaseFollower::_predictionSmoothingAlpha;
float BaseFollower::_positionSmoothingAlpha;

void BaseFollower::setScale( glm::vec2 inQuad, glm::vec2 outQuad) {
	_scale = inQuad / outQuad;
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
    pos = smoothed = lastSmoothed = cur = ofxCv::toOf(track) * _scale;

	_setup(track);
}

void BaseFollower::update(const cv::Point2f & track) {
	cur = ofxCv::toOf(track) * _scale;
	lastSmoothed = smoothed;
	smoothed = glm::mix(smoothed, cur, _positionSmoothingAlpha);

	velocity = glm::mix(smoothed - lastSmoothed, velocity, _predictionSmoothingAlpha);

	pos = smoothed + velocity * _predictionDistance;

	_update(track);
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
			size = ofMap(ofGetElapsedTimef() - startedDying, 0, DYING_TIME, size, 0, true);
		}
		ofNoFill();
		// ofDrawCircle(cur, 1);
		ofDrawCircle(smoothed, size);

		ofDrawLine(smoothed, pos);
		
		// ofLog() << "label: " << label << ", 222prediction distance: " << _predictionDistance << ", psa: " << _predictionSmoothingAlpha << ", sa: " << _positionSmoothingAlpha;
	ofPopStyle();
}