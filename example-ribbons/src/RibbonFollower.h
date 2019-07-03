#pragma once

#include "BaseFollower.h"

class RibbonFollower : public BaseFollower {
    public:
        void _setup(const glm::vec2 & pos);
        void _update(const glm::vec2 & pos);
        void _draw();
        void _kill();
        static void setColorRange(float min, float max, bool inclusive);
        static void setShouldMakeNewSegment(bool shouldMakeNewSegment);
        static void setSegmentGrowthScale(float segmentGrowthScale);
        static void setSegmentBaseSize(float segmentBaseSize);
        static void setSegmentKillTrigger(int segmentKillTrigger);
        static void setBackroundCallback(ofColor color);
    private:
        static bool _shouldMakeNewSegment;
        static float _segmentGrowthScale;
        static float _segmentBaseSize;
        static int _segmentKillTrigger;
        static std::function<void()> _backgroundCallback;

        ofColor _color;
        float _alpha;
        std::vector<glm::vec2> _segments;

        float width;
};