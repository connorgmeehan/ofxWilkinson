#include "PointWarper.h"

void PointWarper::setup(int width, int height){
    _width = width;
    _height = height;

    // setup defaults
    _pointWarpParams.add(_pTopLeft.set("Top Left", glm::vec2(0,0), glm::vec2(0,0), glm::vec2(_width, _height)));
    _pointWarpParams.add(_pTopRight.set("Top Right", glm::vec2(_width,0), glm::vec2(0,0), glm::vec2(_width, _height)));
    _pointWarpParams.add(_pBottomRight.set("Bottom Right", glm::vec2(_width,_height), glm::vec2(0,0), glm::vec2(_width, _height)));
    _pointWarpParams.add(_pBottomLeft.set("Bottom Left", glm::vec2(0,_height), glm::vec2(0,0), glm::vec2(_width, _height)));

    // bind listeners
    _pTopLeft.addListener(this, &PointWarper::onTopLeft);
    _pTopRight.addListener(this, &PointWarper::onTopRight);
    _pBottomRight.addListener(this, &PointWarper::onBottomRight);
    _pBottomLeft.addListener(this, &PointWarper::onBottomLeft);

    // apply defaults
    _srcPoints[0] =  ofxCv::toCv(_pTopLeft);
    _srcPoints[1] =  ofxCv::toCv(_pTopRight);
    _srcPoints[2] =  ofxCv::toCv(_pBottomRight);
    _srcPoints[3] =  ofxCv::toCv(_pBottomLeft);
    _destPoints[0] = cv::Point2f(0,0);
    _destPoints[1] = cv::Point2f(_width,0);
    _destPoints[2] = cv::Point2f(_width,height);
    _destPoints[3] = cv::Point2f(0,_height);
}

std::vector<cv::Point2f> & PointWarper::warpPoints(std::vector<cv::Point2f> & toWarp){
    // TODO:: Make this a variable
    cv::Matx33f perspective = cv::getPerspectiveTransform(_srcPoints, _destPoints);
    _warpedPoints.resize(toWarp.size());

    for(int i = 0; i < toWarp.size(); i++) {
        cv::Point3f temp(toWarp[i].x, toWarp[i].y, 1);
        temp = perspective.inv() * temp;
        _warpedPoints[i] = cv::Point2f(temp.x, temp.y);
    }

    return _warpedPoints;
}

void PointWarper::draw(){

    ofSetColor(ofColor::red);
    ofNoFill();
    ofSetLineWidth(1);

    ofPolyline quad;
    quad.addVertex(glm::vec3(_pTopLeft.get().x, _pTopLeft.get().y, 0) );
    quad.addVertex(glm::vec3(_pTopRight.get().x, _pTopRight.get().y, 0) );
    quad.addVertex(glm::vec3(_pBottomRight.get().x, _pBottomRight.get().y, 0) );
    quad.addVertex(glm::vec3(_pBottomLeft.get().x, _pBottomLeft.get().y, 0) );
    quad.addVertex(glm::vec3(_pTopLeft.get().x, _pTopLeft.get().y, 0) );
    quad.draw();

    ofDrawBitmapString("tl " + ofToString(_pTopLeft), _pTopLeft);
    ofDrawBitmapString("tr " + ofToString(_pTopRight), _pTopRight);
    ofDrawBitmapString("br " + ofToString(_pBottomRight), _pBottomRight);
    ofDrawBitmapString("bl " + ofToString(_pBottomLeft), _pBottomLeft);

    ofDrawRectangle(0, _height, _width, _height);
    for(auto & p : _warpedPoints) {
        ofDrawCircle(p.x, p.y + _height, 3);
    }
}

ofParameterGroup & PointWarper::getParameters() {
    return _pointWarpParams;
}

void PointWarper::onTopLeft(glm::vec2 & val){
    _srcPoints[0] = cv::Point2f(val.x, val.y);
}

void PointWarper::onTopRight(glm::vec2 & val){
    _srcPoints[1] = cv::Point2f(val.x, val.y);
}

void PointWarper::onBottomRight(glm::vec2 & val){
    _srcPoints[2] = cv::Point2f(val.x, val.y);
}

void PointWarper::onBottomLeft(glm::vec2 & val){
    _srcPoints[3] = cv::Point2f(val.x, val.y);
}
