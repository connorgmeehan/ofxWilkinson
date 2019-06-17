#pragma once

#include "ofMain.h"
#include "OlaCommunicator.h"

class SceneBuilder {
    public:

        ~SceneBuilder() {
            
        }
        void setup(int width, int height, int strandCount, int strandLength);
        void bindFrame();
        void unbindFrame();
        void draw(int x, int y);

        void sendToArtnet();
    private:
        int _width, _height;
        ofFbo _frame;

        OlaCommunicator _ola;
};