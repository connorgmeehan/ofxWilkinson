#include "TestFollower.h"

std::function<void(cv::Point2f &)> TestFollower::_onDeath;
std::function<void(cv::Point2f &)> TestFollower::_onLife;

void TestFollower::setup(const cv::Point2f & track){
    _curpos = _smoothed = track;
    _onLife(_curpos);
} 

void TestFollower::update(const cv::Point2f& track){
    _totalDistance += cv::distance(track, _curpos);
    _curpos = track;
    _smoothed = cv::mix(_smoothed, _curpos, SMOOTH_VALUE);
}

void TestFollower::kill(){
    _onDeath(_curpos);
    ofxCv::PointFollower::kill();
}

void TestFollower::draw(){
    if(!dead) {
        ofDrawCircle(_curpos.x, _curpos.y, 2);
    } else { 
        ofDrawLine(_curpos.x-3, _curpos.y-3, _curpos.x+3, _curpos.x+3);
        ofDrawLine(_curpos.x-3, _curpos.y+3, _curpos.x+3, _curpos.x-3);
    }
    ofDrawBitmapString(getLabel(), _curpos.x, _curpos.y);
}

void TestFollower::setOnDeath(std::function<void(cv::Point2f &)> onDeath){
    _onDeath = onDeath;
}

void TestFollower::setOnLife(std::function<void(cv::Point2f &)> onLife){
    _onLife = onLife;
}
