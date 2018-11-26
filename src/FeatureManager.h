#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class FeatureOrigin {
    public:
        FeatureOrigin( glm::vec2 pos ) : _pos(pos) { }

        glm::vec2 & getPos(){ return _pos; }
    private:
        glm::vec2 _pos;
};

class FeatureManager {
    public:

        void setPadding(const glm::vec2 & padding) {
            _padding = padding;
        }

        void setup( int rows, int cols, int width, int height) {
            for (int x = 0; x < cols; ++x) {
                for (int y = 0; y < rows; ++y) {
                    glm::vec2 pos( (float)x/(float)cols * (width-_padding.x*2) + _padding.x, (float)y/(float)rows * (height-_padding.y*2) + _padding.y );
                    _featureOrigins.emplace_back(pos);
                }
            }
            ofLog() << "x";
        }

        std::vector<glm::vec2> getInitialFeatures() {
            if(_receivedIntitialFeatures) {
                ofLog(OF_LOG_ERROR) << "FeatureManager::getIntitalFeatures() - Already gotten initial features.  Use update() to update current features";
                throw "Closing";
            }
            std::vector<glm::vec2> retval;
            for ( auto & fo : _featureOrigins ) {
                retval.emplace_back(fo.getPos());
            }
            return retval;
        }

        void draw(int x, int y) {
            ofPushMatrix();
                ofTranslate(x, y);
                for(auto & fo : _featureOrigins) {
                    ofDrawRectangle(fo.getPos()-glm::vec2(1, 1), 2, 2);
                }
            ofPopMatrix();
        }

    private:
        glm::vec2 _padding;
        std::vector<FeatureOrigin> _featureOrigins;
        bool _receivedIntitialFeatures;
};