#include "PresenceFollower.h"
#include "ColorConverter.h"

ofTexture PresenceFollower::_blobTexture;

bool PresenceFollower::_hsbSelectionInclusive;
float PresenceFollower::_minHSB;
float PresenceFollower::_maxHSB;
float PresenceFollower::_blobResolution;
float PresenceFollower::_blobRadius;

void PresenceFollower::_setup(const glm::vec2 & pos) {
    float hue = ofRandom(_minHSB, _maxHSB);
    hue *= 360 / 255;
    
    HsvColor hsvColor = {hue, 255, 255};
    RgbColor rgbColor = HsvToRgb(hsvColor);

    _color = ofColor(rgbColor.r, rgbColor.g, rgbColor.b);

    // ofLog() << "PresenceFollower::_setup() -> \tpos: " << pos.x << "," << pos.y << "\t\tgenerated hue: " << hue <<"\t _color.getHue(): " << _color.getHue();
    // TODO: add if inclusive or not
}

void PresenceFollower::_update(const glm::vec2 & pos) {

}

void PresenceFollower::_draw() {
    ofSetColor(_color);
    ofDrawCircle(pos, 3);
    _blobTexture.draw(pos-_blobRadius);
}

void PresenceFollower::generateBlobTexture() {
    ofMesh tempMesh;
    tempMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
   
    // add origin to mesh
    tempMesh.addVertex(glm::vec3(0.0f) + glm::vec3(_blobRadius, _blobRadius, 0.0f));
    tempMesh.addColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));

    float i_const = 360.0f/(float) _blobResolution * DEG_TO_RAD;

    for(int i = 0; i < _blobResolution+1; i++) {
        float theta = (float) i * i_const;

        tempMesh.addVertex( glm::vec3(
            sin(theta) * _blobRadius + _blobRadius,
            cos(theta) * _blobRadius + _blobRadius,
            0.0f
        ) );
        tempMesh.addColor(ofFloatColor(0));
    }

    ofFbo tempFbo;
    tempFbo.allocate(_blobRadius*2, _blobRadius*2, GL_RGBA);
    tempFbo.begin();
    ofClear(0);
    tempMesh.drawFaces();
    tempFbo.end();

    _blobTexture = tempFbo.getTexture();
}

void PresenceFollower::setColorRange(float min, float max, bool inclusive) {
    ofLog() << "PresenceFollower::setColorRange( min: " << min <<" , max: " << max << " , inclusive: " << inclusive << ")";
    _minHSB = min;
    _maxHSB = max;
    _hsbSelectionInclusive = inclusive;
}

void PresenceFollower::setBlobSettings(float radius, int resolution) {
    _blobRadius = radius;
    _blobResolution = resolution;
}