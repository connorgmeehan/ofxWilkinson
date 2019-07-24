#include "ColorConverter.h"
#include "RibbonFollower.h"

bool RibbonFollower::_shouldMakeNewSegment;
float RibbonFollower::_segmentGrowthScale = 50;
float RibbonFollower::_segmentBaseSize = 5;
float RibbonFollower::_nextSegmentDistance = 0.5;

void RibbonFollower::_setup(const glm::vec2 & pos) {
  float hue = ofRandom(0, 255);
  hue *= 360 / 255;
  
  HsvColor hsvColor = {hue, 255, 255};
  RgbColor rgbColor = HsvToRgb(hsvColor);

  _color = ofColor(+rgbColor.r, +rgbColor.g, +rgbColor.b, 255);
  _segments.push_back(glm::vec3(pos.x, pos.y, ofGetElapsedTimef()));
}

void RibbonFollower::_update(const glm::vec2 & pos) {
  float timeDying = _curTime - startedDying;
  if(_isDying){
      // ofLog() << "RibbonFollower::_update() -> isDying: " << _isDying << "| _alpha: " << _alpha << "= 255 - (timeDying: " << timeDying << "/ _dyingTime: " << _dyingTime << " * 255) ";
    _alpha = 255 - (timeDying / _dyingTime * 255);
    if(timeDying > _dyingTime) {
      this->PointFollower::kill();
    }
  }
}

void RibbonFollower::_draw() {
  ofSetColor(_color, _alpha);
  ofDrawCircle(smoothed, 3);

  for(glm::vec3 & p : _segments) {
    float radius = calculateRadius(p.z);
    ofDrawRectangle(p.x - radius, p.y - radius, radius * 2, radius * 2);
  }
}

ofColor & RibbonFollower::getColor() {
  return _color;
}

void RibbonFollower::_updateSegments() {
  if(_segments.size() > 0) {
    glm::vec2 lastPos(_segments[_segments.size()-1].x, _segments[_segments.size()-1].y);
    if(glm::distance(smoothed, lastPos) > _nextSegmentDistance) {
      _segments.push_back(glm::vec3(pos.x, pos.y, ofGetElapsedTimef()));
    }
  }
}

bool RibbonFollower::envelopedBy(ofRectangle & rect) {
  if(_segments.size() > 0 && !_isDying) {
    glm::vec3 & p = _segments[0];

    float radius = calculateRadius(p.z);
    ofRectangle firstRect = ofRectangle(p.x - radius, p.y - radius, radius, radius);
    if(
      firstRect.getX() < rect.getX() &&
      firstRect.getY() < rect.getY() &&
      firstRect.getWidth() > rect.getWidth() &&
      firstRect.getHeight() > rect.getHeight()) {
      return true;
    }
  }
  return false;
}

void RibbonFollower::setShouldMakeNewSegment(bool shouldMakeNewSegment) {
  _shouldMakeNewSegment = shouldMakeNewSegment;
}

void RibbonFollower::setSegmentGrowthScale(float segmentGrowthScale) {
  _segmentGrowthScale = segmentGrowthScale;
}

void RibbonFollower::setSegmentBaseSize(float segmentBaseSize) {
  _segmentBaseSize = segmentBaseSize;
}

void RibbonFollower::_kill() {
  if (_isDying == false) {
    _isDying = true;
    startedDying = _curTime;
  }

  float timeDying = _curTime - startedDying;
    _alpha = 255 - (timeDying / _dyingTime * 255);
  if(_isDying && timeDying > _dyingTime) {
    this->PointFollower::kill();
  }
}

float RibbonFollower::calculateRadius(float timeOfCreation) {
  return _segmentBaseSize + powf((ofGetElapsedTimef() - timeOfCreation) * _segmentGrowthScale, 1.8f);;
}