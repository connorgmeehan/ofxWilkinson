#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  cam.enableOrtho();

  // Setup ofxwilkinson settings
  wilkinson.setCameraDimensions(inputWidth, inputHeight);
  wilkinson.setOutputDimensions(16, 50, outputWidth, outputHeight);
  wilkinson.setLightingArrayDimensions(strandCount, strandLength);
  wilkinson.setup();

  curTime = ofGetElapsedTimef();
  triggerTime = curTime + 1.0f;

  RibbonFollower::setSegmentGrowthScale(1);
  RibbonFollower::setSegmentBaseSize(5);
  RibbonFollower::setBackroundCallback([&](ofColor& color) {
    bgColor = color;
  });
}

//--------------------------------------------------------------
void ofApp::update(){
  // If it's time for followers to make a new segment, set flag to true
  curTime = ofGetElapsedTimef();
  if(curTime > triggerTime) {
    triggerTime += triggerInterval;
    RibbonFollower::setShouldMakeNewSegment(true);
  }

  wilkinson.update();
  
  RibbonFollower::setShouldMakeNewSegment(false);
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(bgColor);
  ofFill();
  ofEnableBlendMode(OF_BLENDMODE_ALPHA);

  // wilkinson.bindFrame();
    wilkinson.drawFollowers();
  // wilkinson.unbindFrame();
  
  ofDisableBlendMode();
  wilkinson.draw(0,0);
}

void ofApp::exit(){
  wilkinson.exit();
}