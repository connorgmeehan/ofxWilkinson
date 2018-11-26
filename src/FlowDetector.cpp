#include "./FlowDetector.h"

void FlowDetector::setup(int width, int height, vector<glm::vec2> features){
    int nFeatures = features.size();
    _flow.setMaxFeatures(nFeatures);
    _flow.setFeaturesToTrack(features);
}

void FlowDetector::update(cv::Mat & cam){
    _flow.calcOpticalFlow(cam);
}

void FlowDetector::draw(int x, int y){
    _flow.draw(x,y);
}

std::vector<cv::Point2f> FlowDetector::getFeatures(){
    return _flow.getCvCurrent();
}

void FlowDetector::updateFeatures(std::vector<glm::vec2> & dirtyFeatures, std::vector<glm::vec2> & newFeatures){
    std::vector<glm::vec2> oldFeatures = _flow.getCurrent();
    for(auto & df : dirtyFeatures){
        oldFeatures.erase(std::remove(oldFeatures.begin(), oldFeatures.end(), df), oldFeatures.end());
    }
    oldFeatures.insert(oldFeatures.end(), newFeatures.begin(), newFeatures.end());
    _flow.setFeaturesToTrack(oldFeatures);
    ofLog() << "FlowDetector::updateFeatures() - dirty.size(): " << to_string(dirtyFeatures.size()) << ", new.size(): " << to_string(newFeatures.size()) << " features.size(): " << to_string(oldFeatures.size());
}