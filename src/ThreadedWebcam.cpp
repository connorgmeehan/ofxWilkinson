#include "./ThreadedWebcam.h"

void ThreadedWebcam::setup(int width, int height){
    ofLog() << "ThreadedWebcam::setup(width:"<<width<<", height:"<<height<<"){..}";
    _cam.setup(width, height);

    _cam.listDevices();
    _cam.setVerbose(true);
    _cam.setDesiredFrameRate(ARTNET_MAX_FPS); 
    ofLog() << "\tCam Pixel format: " << _cam.getPixelFormat();

    _camOut.allocate(width, height, OF_IMAGE_COLOR);
    startThread();
    _width = width;
    _height = height;
}

void ThreadedWebcam::stop(){
    stopThread();
}

void ThreadedWebcam::threadedFunction(){
    while(isThreadRunning()){
        _cam.update();
        if(_cam.isFrameNew()){
            lock();
            _isFrameNew = true;
            // _camOut.setFromPixels(_cam.getPixels().getData(), _width, _height, OF_IMAGE_COLOR, true);
            // _camOut.update();
            _camOut = _cam.getPixels();
            unlock();
        }
    }
}

void ThreadedWebcam::draw(int x, int y){
    ofImage temp(_camOut);
    temp.draw(x,y);
}

bool ThreadedWebcam::isFrameNew(){
    if(_isFrameNew){
        _isFrameNew = false;
        return true;
    }
    return _isFrameNew;
}

ofPixels ThreadedWebcam::getPixels(){
  return _camOut;
}