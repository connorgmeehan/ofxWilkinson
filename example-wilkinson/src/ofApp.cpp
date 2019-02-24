#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  // Setup follower settings
  PresenceFollower::setBlobSettings(10, 32);
  PresenceFollower::setColorRange(100, 200, false);
  PresenceFollower::generateBlobTexture();

  // Setup ofxwilkinson settings
  wilkinson.setCameraDimensions(inputWidth, inputHeight);
  wilkinson.setOutputDimensions(16, 50, outputWidth, outputHeight);
  wilkinson.setup();

  // Setup follower FBO
  followerFbo.allocate(outputWidth, outputHeight, GL_RGBA);
  followerFbo.begin();
  ofClear(0);
  followerFbo.end();

  // Setup shader
  backgroundShader.load("background");
}

//--------------------------------------------------------------
void ofApp::update(){
  wilkinson.update();

  shaderBackgroundStrength = glm::mix(
    shaderBackgroundStrength,
    (float)1/(wilkinson.getFollowers().size()+1),
    0.05f
  );

}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(12);

  // Reset the styles so everything renders normally
  ofSetColor(ofColor::white);
  ofFill();

  // Enable additive blend mode (like photoshop linear dodge, overlayed colours add to eachother becoming more white)
  ofEnableBlendMode(OF_BLENDMODE_ADD);

  // Draw Foreground into an FBO so we can use it in a shader
  followerFbo.begin();
    ofClear(0);
    // Draw the followers that we've defined
    wilkinson.drawFollowers();
  followerFbo.end();

  // Disable the add blend mode, we're just drawing regularly now
  ofEnableBlendMode(OF_BLENDMODE_DISABLED);

  // Foreground into shader background and draw in FBO
  backgroundShader.begin();
    backgroundShader.setUniformTexture("tex0_in", followerFbo.getTexture(), 0);
    backgroundShader.setUniform2f("resolution", outputWidth, outputHeight);
    backgroundShader.setUniform1f("threshold", 0.2f);
    backgroundShader.setUniform1f("i_time", ofGetElapsedTimef());
    backgroundShader.setUniform1f("bg_strength", shaderBackgroundStrength);
    // Bind the wilkinson output frame so it can be outputted to the lighting display
    wilkinson.bindFrame();
      ofClear(0);

      ofDrawRectangle(0, 0, 1024, 768);

    wilkinson.unbindFrame();
  backgroundShader.end();
  
  wilkinson.draw(20, 20);
}

void ofApp::exit(){
  wilkinson.exit();
}