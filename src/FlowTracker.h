#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "TestFollower.h"

class FlowTracker {
    public:
        void setup() {
            _tracker.setPersistence(15);
            _tracker.setMaximumDistance(25);

            TestFollower::setOnDeath([&](cv::Point2f & feature){
                _dirtyFeatures.emplace_back(feature);
            });
        }

        void update(const std::vector<Point2f> & points) {
            _tracker.track( points );
        }

        void draw( int x, int y ) {

            auto & followers = _tracker.getFollowers();

            ofPushMatrix();
                ofTranslate(x, y);
                for( auto & f : followers ) {
                    f.draw();
                }
            ofPopMatrix();
        }

        std::vector<TestFollower> & getFollowers() {
            return _tracker.getFollowers();
        }

        size_t size() {
            return _tracker.getFollowers().size();
        }
    private:
        ofxCv::PointTrackerFollower<TestFollower> _tracker;

        std::vector<cv::Point2f> _dirtyFeatures;
    
};