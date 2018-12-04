template <class UserFollower>
void FeatureManager<UserFollower>::setup() {
    _featureManagerParams.add(_trackingPersistance.set("Tracking Persistance", 15, 5, 40));
    _featureManagerParams.add(_trackingMaxDistance.set("Tracking Max Distance", 30, 5, 40));
    _featureManagerParams.add(_featurePredictionDistance.set("Predict Ahead Value", 3, 0, 5));
    _featureManagerParams.add(_featurePredictionSmoothingAlpha.set("Predict Ahead Smoothing", 0.2f, 0, 1));
    _featureManagerParams.add(_featureSmoothingAlpha.set("Position Smoothing Amount", 0.2, 0.0, 1.0));

    _featurePredictionDistance.addListener(this, &FeatureManager<UserFollower>::onFeaturePredictionDistance);
    _featurePredictionSmoothingAlpha.addListener(this, &FeatureManager<UserFollower>::onFeaturePredictionSmoothAmount);
    _featureSmoothingAlpha.addListener(this, &FeatureManager<UserFollower>::onFeatureSmoothAmount);

    UserFollower::setPredictionDistance(_featurePredictionDistance);
    UserFollower::setPredictionSmoothingAlpha(_featurePredictionSmoothingAlpha);
    UserFollower::setSmoothingAlpha(_featureSmoothingAlpha);
}

template <class UserFollower>
void FeatureManager<UserFollower>::update(std::vector<cv::Point2f> & features) {
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
