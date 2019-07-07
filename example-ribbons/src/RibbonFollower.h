#pragma once

#include "BaseFollower.h"

class RibbonFollower : public BaseFollower {
    public:
        void _setup(const glm::vec2 & pos);
        void _update(const glm::vec2 & pos);
        void _draw();
        void _kill();

        bool outside(ofRectangle & rect);
        ofColor & getColor();
        static void setColorRange(float min, float max, bool inclusive);
        static void setShouldMakeNewSegment(bool shouldMakeNewSegment);
        static void setSegmentGrowthScale(float segmentGrowthScale);
        static void setSegmentBaseSize(float segmentBaseSize);
        static void setSegmentKillTrigger(int segmentKillTrigger);
    private:
        static bool _shouldMakeNewSegment;
        static float _segmentGrowthScale;
        static float _segmentBaseSize;
        static int _segmentKillTrigger;
        static std::function<void(ofColor&)> _backgroundCallback;

        ofColor _color;
        int _alpha = 255;
        std::vector<glm::vec2> _segments;

        float width;
};