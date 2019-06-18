#include "SceneBuilder.h"

void SceneBuilder::setup(int width, int height, int strandCount, int strandLength){
    _width = width;
    _height = height;
    _frame.allocate(_width, _height, GL_RGB);
    _ola.setup(width, height, strandCount, strandLength);
}

void SceneBuilder::bindFrame(){
    _frame.begin();
    ofClear(0);
}

void SceneBuilder::unbindFrame(){
    _frame.end();
    _ola.sendFrame(_frame);
}

void SceneBuilder::draw(int x, int y) {
    _frame.draw(x, y);
}