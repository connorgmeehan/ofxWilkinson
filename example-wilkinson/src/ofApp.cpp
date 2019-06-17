#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  cam.enableOrtho();

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

  // Setup plane to mix background / FBO of followers on
  plane.set(outputWidth*2, outputHeight*2);
  plane.setPosition(0,0,0);
  plane.setResolution(2,2);
  plane.mapTexCoordsFromTexture(followerFbo.getTexture());

  // Setup shader
  #ifdef TARGET_OPENGLES 
    backgroundShader.load("background_gles");
	#else
    backgroundShader.load("background");
	#endif

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
  ofBackground(100);
  ofFill();

  // Enable additive blend mode (like photoshop linear dodge, overlayed colours add to eachother becoming more white)
  ofEnableBlendMode(OF_BLENDMODE_ADD);

  // Draw Foreground into an FBO so we can use it in a shader
  followerFbo.begin();
    ofClear(0);
    // Draw the followers that we've defined
    wilkinson.drawFollowers();
  followerFbo.end();

  // Reset the styles so plane renders normally
  ofSetColor(ofColor::white);

  // Disable the add blend mode, we're just drawing regularly now
  ofEnableBlendMode(OF_BLENDMODE_DISABLED);

  // Foreground into shader background and draw in FBO
  backgroundShader.begin();
    backgroundShader.setUniformTexture("tex0", followerFbo.getTexture(), 0);
    backgroundShader.setUniform2f("resolution", outputWidth, outputHeight);
    backgroundShader.setUniform1f("threshold", 0.2f);
    backgroundShader.setUniform1f("i_time", ofGetElapsedTimef());
    backgroundShader.setUniform1f("bg_strength", shaderBackgroundStrength);
    // Bind the wilkinson output frame so it can be outputted to the lighting display
    wilkinson.bindFrame();
      ofClear(0);
      plane.draw();
    wilkinson.unbindFrame();
  backgroundShader.end();
  
  // draws debug gui
  wilkinson.draw(20, 20);

  // Reset the styles so plane renders normally
  ofSetColor(ofColor::white);
  // draw follower FBO for debugging
  followerFbo.draw(20, 250);

  // draws output frame (for debugging)
  wilkinson.drawOutputFrame(20 + outputWidth, 250);
}

void ofApp::exit(){
  wilkinson.exit();
}