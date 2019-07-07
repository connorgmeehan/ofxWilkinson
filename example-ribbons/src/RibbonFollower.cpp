#include "ColorConverter.h"
#include "RibbonFollower.h"

bool RibbonFollower::_shouldMakeNewSegment;
float RibbonFollower::_segmentGrowthScale = 1;
float RibbonFollower::_segmentBaseSize = 5;

void RibbonFollower::_setup(const glm::vec2 & pos) {
  float hue = ofRandom(0, 255);
  hue *= 360 / 255;
  
  HsvColor hsvColor = {hue, 255, 255};
  RgbColor rgbColor = HsvToRgb(hsvColor);

  _color = ofColor(+rgbColor.r, +rgbColor.g, +rgbColor.b, 255);
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

  
  if (_shouldMakeNewSegment) {
    _segments.push_back(pos);
  }
}

void RibbonFollower::_draw() {
  ofSetColor(_color, _alpha);
  ofDrawCircle(smoothed, 3);

  float radius = (float) _segments.size() * _segmentGrowthScale;
  for(glm::vec2 & p : _segments) {
    radius -= _segmentGrowthScale;
    ofDrawRectangle(p.x - radius, p.y - radius, radius * 2, radius * 2);
  }
}

ofColor & RibbonFollower::getColor() {
  return _color;
}

bool RibbonFollower::envelopedBy(ofRectangle & rect) {
  if(_segments.size() > 0 && !_isDying) {
    glm::vec2 & p = _segments[0];

    float radius = (float) _segments.size() * _segmentGrowthScale;
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