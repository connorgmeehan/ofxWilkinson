#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  wilkinson.setCameraDimensions(640, 360);
  wilkinson.setOutputDimensions(64, 50, 16, 50);
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
}

void ofApp::exit(){
  wilkinson.exit();
}