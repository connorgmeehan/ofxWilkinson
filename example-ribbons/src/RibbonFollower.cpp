#include "ColorConverter.h"
#include "RibbonFollower.h"

bool RibbonFollower::_shouldMakeNewSegment;
float RibbonFollower::_segmentGrowthScale = 1;
float RibbonFollower::_segmentBaseSize = 5;
int RibbonFollower::_segmentKillTrigger = 20;
std::function<void()> RibbonFollower::_backgroundCallback;

void RibbonFollower::_setup(const glm::vec2 & pos) {
  float hue = ofRandom(0, 255);
  hue *= 360 / 255;
  
  HsvColor hsvColor = {hue, 255, 255};
  RgbColor rgbColor = HsvToRgb(hsvColor);

  _color = ofColor(rgbColor.r, rgbColor.g, rgbColor.b, 1.0f);
}

void RibbonFollower::_update(const glm::vec2 & pos) {
  if (_shouldMakeNewSegment) {
    _segments.push_back(pos);
  }

  if(_segments.size() > _segmentKillTrigger) {
    _kill();
    _backgroundCallback(_color);
  }
}

void RibbonFollower::_draw() {
  ofSetColor(_color.r, color.g, color.b, _alpha);
  ofDrawCircle(smoothed, 3);

  float radius = (float) _segments.size() * _segmentGrowthScale;
  for(glm::vec2 & p : _segments) {
    radius -= _segmentGrowthScale;
    ofDrawRectangle(p.x - radius, p.y - radius, radius * 2, radius * 2);
  }
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

void RibbonFollower::setBackroundCallback(std::function<void(ofColor&)> backgroundCallback) {
  _backgroundCallback = backgroundCallback;
} 

void RibbonFollower::_kill() {
  float curTime = ofGetElapsedTimef();
  if (startedDying == 0) {
    startedDying = curTime;
  }
  _alpha = 255 - ((curTime - startedDying) / _dyingTime * 255);

  if(curTime - startedDying > _dyingTime) {
    this->PointFollower::kill();
  }
}