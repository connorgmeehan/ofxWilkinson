#include "SceneBuilder.h"

void SceneBuilder::setup(int width, int height){
    _width = width;
    _height = height;
    _frame.allocate(_width, _height, GL_RGB);
}

void SceneBuilder::bindFrame(){
    _frame.clear();
    _frame.begin();
}

void SceneBuilder::unbindFrame(){
    _frame.end();
    _ola.sendFrame(_frame);
}