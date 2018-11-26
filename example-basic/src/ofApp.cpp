#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  wilkinson.setDimensions(640, 360);
  wilkinson.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
  wilkinson.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(12);
  wilkinson.draw(100, 100);
}

void ofApp::exit(){
  wilkinson.exit();
}