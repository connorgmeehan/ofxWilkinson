#pragma once

#include "ofMain.h"
#include "OlaCommunicator.h"

class SceneBuilder {
    public:

        ~SceneBuilder() {
            
        }
        void setup(int width, int height);
        void bindFrame();
        void unbindFrame();

        void sendToArtnet();
    private:
        int _width, _height;
        ofFbo _frame;

        OlaCommunicator _ola;
};