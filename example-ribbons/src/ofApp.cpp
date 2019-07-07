#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

  cam.enableOrtho();

  // Setup ofxwilkinson settings
  wilkinson.setCameraDimensions(inputWidth, inputHeight);
  wilkinson.setOutputDimensions(16, 50, outputWidth, outputHeight);
  wilkinson.setLightingArrayDimensions(strandCount, strandLength);
  wilkinson.setup();

  // Setup follower FBO
  followerFbo.allocate(outputWidth, outputHeight, GL_RGB);
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
  
  // 
  curTime = ofGetElapsedTimef();
  triggerTime = curTime + 1.0f;

  RibbonFollower::setSegmentGrowthScale(1);
  RibbonFollower::setSegmentBaseSize(5);
}
//--------------------------------------------------------------
void ofApp::update(){
  // If it's time for followers to make a new segment, set flag to true
  curTime = ofGetElapsedTimef();
  if(curTime > triggerTime) {
    triggerTime += triggerInterval;
    RibbonFollower::setShouldMakeNewSegment(true);

    auto & followers = wilkinson.getFollowers();

    ofRectangle borderRect = ofRectangle(0, 0, outputWidth/2, outputHeight/2);

    for(auto & f : followers) {
      if(f.envelopedBy(borderRect)){
          bgColor = ofFloatColor(f.getColor());
          timeOfLastBackgroundReset = ofGetElapsedTimef();
          f._kill();
      }
    }
    float timeSinceLastBgReset = ofGetElapsedTimef() - timeOfLastBackgroundReset; 
    bgStrength = ofClamp(timeSinceLastBgReset, 0, bgFadeTime) / bgFadeTime;
  }

  wilkinson.update();
  
  RibbonFollower::setShouldMakeNewSegment(false);
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofDrawBitmapString( ofToString(ofGetFrameRate()), 20, ofGetHeight() - 20);
  ofSetColor(ofColor::white);  
  ofFill();

  followerFbo.begin();
    ofClear(0);
    wilkinson.drawFollowers();
  followerFbo.end();

  ofSetColor(ofColor::white);

  backgroundShader.begin();
    backgroundShader.setUniformTexture("tex0", followerFbo.getTexture(), 0);
    backgroundShader.setUniform2f("resolution", outputWidth, outputHeight);
    backgroundShader.setUniform1f("i_time", ofGetElapsedTimef());
    backgroundShader.setUniform1f("bg_strength", bgStrength);
    backgroundShader.setUniform4f("bg_color", glm::vec4(bgColor.r, bgColor.g, bgColor.b, 1));

    wilkinson.bindFrame();
      ofClear(0);
      plane.draw();
    wilkinson.unbindFrame();
  backgroundShader.end();

  wilkinson.draw(40,40);

  ofSetColor(ofColor::white);

  // draw follower FBO for debugging
  followerFbo.draw(20 + outputWidth, 250);

  // draws output frame (for debugging)
  wilkinson.drawOutputFrame(20 + outputWidth*2, 250);

  ofPushMatrix();
    ofTranslate(20, 250);
    wilkinson.drawFollowers();
  ofPopMatrix();
}

void ofApp::exit(){
  wilkinson.exit();
}