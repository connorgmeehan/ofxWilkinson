#pragma once

#include "ofxOsc.h"
#include "ofxGuiGroup.h"

enum CommandType : char {
  OSC_NULL,
  OSC_BOOL,
  OSC_INT,
  OSC_FLOAT,
  OSC_VEC2
};

struct Command {
  Command(std::string path, CommandType type, ofAbstractParameter * parameterpointer) {
    this->path = path;
    this->type = type;
    this->pParameter = parameterpointer;
  }
  std::string path = "";
  CommandType type;
  ofAbstractParameter * pParameter = nullptr;
};

class OscController {  
  public:
    OscController();
    OscController(ofxGuiGroup & gui);

    void buildCommandList();
    void update();

  private:
    Command getCommandFromPath(std::string & commandPath);
    CommandType getCommandTypeFromTypeString(std::string & type);
    void printCommands();
    void printHelp();
    void handleGet(std::string & commandPath);
    void handleSet(std::string & commandPath, ofxOscMessage & message);

    ofxOscReceiver _oscReceiver;

    ofxGuiGroup * _gui;
    std::vector<Command> _controls;
};