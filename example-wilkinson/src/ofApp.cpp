#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  PresenceFollower::setBlobSettings(10, 32);
  PresenceFollower::setColorRange(20, 180, false);
  PresenceFollower::generateBlobTexture();

  wilkinson.setCameraDimensions(640, 360);
  wilkinson.setOutputDimensions(16, 50, 64, 50);
  wilkinson.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
  wilkinson.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(12);
  wilkinson.draw(20, 20);

  ofPushMatrix();
    ofTranslate(640,20);
    for(auto & f : wilkinson.getFollowers()) {
      f.draw();
    }
  ofPopMatrix();
}

void ofApp::exit(){
  wilkinson.exit();
}