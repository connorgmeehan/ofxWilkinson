#pragma once

#include "BaseFollower.h"

class PresenceFollower : public BaseFollower {
    public:
        void _setup(const glm::vec2 & pos);
        void _update(const glm::vec2 & pos);
        void _draw();

        static void setColorRange(float min, float max, bool inclusive);
        static void generateBlobTexture();
        static void setBlobSettings(float radius, int resolution);
    private:

        ofColor _color;

        static ofTexture _blobTexture;
        static bool _hsbSelectionInclusive;
        static float _minHSB, _maxHSB;
        static float _blobResolution, _blobRadius;
};