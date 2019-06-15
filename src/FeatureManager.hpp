template <class UserFollower>
void FeatureManager<UserFollower>::setup() {
    _featureManagerParams.add(_trackingPersistance.set("tracking_persistence", 15, 5, 40));
    _featureManagerParams.add(_trackingMaxDistance.set("tracking_distance_max", 30, 5, 40));
    _featureManagerParams.add(_featurePredictionDistance.set("tracking_predict_scale", 3, 0, 5));
    _featureManagerParams.add(_featurePredictionSmoothingAlpha.set("tracking_predict_smoothing", 0.2f, 0, 1));
    _featureManagerParams.add(_featureSmoothingAlpha.set("position_smoothing", 0.2, 0.0, 1.0));

    _featurePredictionDistance.addListener(this, &FeatureManager<UserFollower>::onFeaturePredictionDistance);
    _featurePredictionSmoothingAlpha.addListener(this, &FeatureManager<UserFollower>::onFeaturePredictionSmoothAmount);
    _featureSmoothingAlpha.addListener(this, &FeatureManager<UserFollower>::onFeatureSmoothAmount);

    UserFollower::setPredictionDistance(_featurePredictionDistance);
    UserFollower::setPredictionSmoothingAlpha(_featurePredictionSmoothingAlpha);
    UserFollower::setSmoothingAlpha(_featureSmoothingAlpha);
}

template <class UserFollower>
void FeatureManager<UserFollower>::update(std::vector<cv::Point2f> & features) {

    scaleFeaturestoOutput(features);

    _tracker.track(features);
}

template <class UserFollower>
void FeatureManager<UserFollower>::draw(int width, int height) {
    ofPushMatrix();
        ofTranslate(0, height);
        auto & followers = _tracker.getFollowers();
        for(auto & f : followers) {
            f.drawDebug();
        }
    ofPopMatrix();
}

template <class UserFollower>
ofParameterGroup & FeatureManager<UserFollower>::getParameters() {
    return _featureManagerParams;
}

template <class UserFollower>
std::vector<UserFollower> & FeatureManager<UserFollower>::getFollowers() {
    return _tracker.getFollowers();
}

template <class UserFollower>
void FeatureManager<UserFollower>::onTrackingPersistance(float & val){
    _tracker.setPersistance(val);
}

template <class UserFollower>
void FeatureManager<UserFollower>::onTrackingMaxDistance(float & val){
    _tracker.setMaxDistance(val);
}

template <class UserFollower>
void FeatureManager<UserFollower>::onFeaturePredictionDistance(float & val){
    UserFollower::setPredictionDistance(val);
}

template <class UserFollower>
void FeatureManager<UserFollower>::onFeaturePredictionSmoothAmount(float & val){
    UserFollower::setPredictionSmoothingAlpha(1.0f - val);
}

template <class UserFollower>
void FeatureManager<UserFollower>::onFeatureSmoothAmount(float & val){
    UserFollower::setSmoothingAlpha(1.0f - val);
}

template <class UserFollower>
void FeatureManager<UserFollower>::setOutputScale(glm::vec2 sourceDimensions, glm::vec2 outputDimensions) {
    glm::vec2 glmOutputScale = outputDimensions / sourceDimensions;
    _featurePositionScale = cv::Point_<float>(glmOutputScale.x, glmOutputScale.y);
}

template <class UserFollower>
void FeatureManager<UserFollower>::scaleFeaturestoOutput(std::vector<cv::Point2f> & features) {
    for ( auto & f : features ) {
        f.x *= _featurePositionScale.x; 
        f.y *= _featurePositionScale.y; 
    }
}