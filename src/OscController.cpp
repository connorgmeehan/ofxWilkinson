#include "OscController.h"
#include "split.h"

OscController::OscController() :
  _gui(nullptr) {  
}

OscController::OscController(ofxGuiGroup & gui) : 
  _gui(&gui) {
  // Setup osc receiver
  int port = 8999;

  ofLog() << "OscController::OscController() -> starting osc server on port " << port;
  _oscReceiver.setup(port);
}

void OscController::buildCommandList() {
  if(_gui != nullptr) {
    std::vector<std::string> controlNames = _gui->getControlNames();
    
    // Loop through parent groups
    for (auto & controlName : controlNames) {
      ofxBaseGui * node = _gui->getControl(controlName);
      auto type = node->getParameter().valueType();
      auto name = node->getName();

      ofParameterGroup group = node->getParameter().castGroup();
      // Loop through sub-parameters
      for (int i = 0; i < group.size(); i++) {
        // generate osc path
        auto subName = group.get(i).getName();
        std::string path = std::string("/") + name + std::string("/") + subName;
        
        // generate human friendly type
        auto subType = group.get(i).type();
        auto commandType = getCommandTypeFromTypeString(subType);

        // add command to control list
        _controls.push_back(Command(path, commandType, &group.get(i)));
      }
    }
  } else {
    ofLog(OF_LOG_ERROR) << "OscController::buildCommandList() -> _gui is nullptr, reconstruct class with ofxGuiGroup";
  }
}

void OscController::update() {
  while (_oscReceiver.hasWaitingMessages()) {
    // Get next mesage
    ofxOscMessage message;
    _oscReceiver.getNextMessage(message);

    // Format to match ofxGuiGroup parameter name
    std::string commandPath = message.getAddress();

    std::vector<std::string> commands = split(commandPath, '/');
    commands.erase(commands.begin());
    if(commands[0].compare(std::string("help")) == 0) {
      printHelp();
    } else if(commands[0].compare(std::string("set")) == 0) {
      // Setting ofParameter
      commandPath.erase(commandPath.begin(), commandPath.begin()+4);
      handleSet(commandPath, message);
    } else if(commands[0].compare(std::string("get")) == 0) {
      // Getting value of ofParameter
      commandPath.erase(commandPath.begin(), commandPath.begin()+4);
      handleGet(commandPath);
    } else if(commands[0].compare(std::string("list")) == 0) {
      // Printing all commands
      printCommands();
    } else if(commands[0].compare(std::string("save")) == 0) {
        saveParameters(message);
    } else if(commands[0].compare(std::string("load")) == 0) {
        loadParameters(message);
    } else {
      std::cout << "OscController: Error: command for \"" << commandPath << "\"";
      for(int i = 0; i < message.getNumArgs(); i++) {
        std::cout << "\t" << message.getArgAsString(i);
      }
      std::cout << " was not found :(." << std::endl;

    }
  }
}

void OscController::printCommands() {

  auto typePrinter = [](CommandType type) -> char* {
    switch (type)
    {
    case OSC_INT:
      return "int,\t1 parameter\t1";
      break;
    case OSC_FLOAT:
      return "float,\t1 parameter\t1.0";
      break;
    case OSC_BOOL:
      return "bool,\t1 parameter\t1 or 0";
      break;
    case OSC_VEC2:
      return "vec2,\t2 parameter\t1.0 1.0";
      break;
    
    default:
      break;
    }
  };

  std::cout << "Printing command list: " << std::endl;
  for(auto & command : _controls) {
    std::cout << "\t" << command.path << "\t type: " << typePrinter(command.type) << std::endl;
  }
}

void OscController::printHelp() {
  std::cout << "OscController: HELP" << std::endl << 
  "-----------------------------" << std::endl << 
  "/help - prints this menu" << std::endl << 
  "/list - lists all availiable commands" << std::endl << 
  "/set/{{command}} {{param1}} {{param2}} - sets a parameter to a certain value" << std::endl << 
  "/get/{{command}} - gets the value of a parameter" << std::endl << 
  "/save [filename.xml/json] - saves config to xml (filename optional)" << std::endl << 
  "/load [filename.xml/json]- loads config from xml (filename optional)" << std::endl;
}

void OscController::handleGet(std::string & commandPath) {
  Command selectedCommand = getCommandFromPath(commandPath);
  switch (selectedCommand.type)
  {
  case OSC_BOOL: {
    ofLog() << "GET: " << commandPath << " is " << selectedCommand.pParameter->cast<bool>().toString();
    break;
  }
  case OSC_INT: {
    ofLog() << "GET: " << commandPath << " is " << selectedCommand.pParameter->cast<int>().toString();
    break;
  }
  case OSC_FLOAT: {
    ofLog() << "GET: " << commandPath << " is " << selectedCommand.pParameter->cast<float>().toString();
    break;
  }
  case OSC_VEC2: {
    ofLog() << "GET: " << commandPath << " is " << selectedCommand.pParameter->cast<glm::vec2>().toString();
    break;  
  }
  default:
    ofLog() << "\tError: Command " << commandPath << " not found.";
    break;
  }
}

void OscController::handleSet(std::string & commandPath, ofxOscMessage & message) {
  Command selectedCommand = getCommandFromPath(commandPath);
  switch (selectedCommand.type)
  {
  case OSC_BOOL: {
    selectedCommand.pParameter->cast<bool>().set(message.getArgAsBool(0));
    ofLog() << "Success!  " << commandPath << " is now equal to " << message.getArgAsBool(0) << ".";
    break;
  }
  case OSC_INT: {
    selectedCommand.pParameter->cast<int>().set(message.getArgAsInt32(0));
    ofLog() << "Success!  " << commandPath << " is now equal to " << message.getArgAsInt32(0) << ".";
    break;
  }
  case OSC_FLOAT: {
    selectedCommand.pParameter->cast<float>().set(message.getArgAsFloat(0));
    ofLog() << "Success!  " << commandPath << " is now equal to " << message.getArgAsFloat(0) << ".";
    break;
  }
  case OSC_VEC2: {
    selectedCommand.pParameter->cast<glm::vec2>()
      .set(glm::vec2(message.getArgAsFloat(0), message.getArgAsFloat(1)));
    ofLog() << "Success!  " << commandPath << " is now equal to glm::vec2( " << message.getArgAsFloat(0) << ", " << message.getArgAsFloat(1) << ")"; 
    break;  
  }
  default:
    ofLog() << "\tError: Command " << commandPath << " not found.";
    break;
  }
}

void OscController::saveParameters(ofxOscMessage & message) {
  if (message.getNumArgs() > 0) {
    this->_gui->saveToFile(message.getArgAsString(0));
    ofLog() << "OscController: saved parameters to " << message.getArgAsString(0) << ".";
  } else {
    this->_gui->saveToFile("ofxWilkinson.json");
    ofLog() << "OscController: saved parameters to ofxWilkinson.";
  }
}

void OscController::loadParameters(ofxOscMessage & message) {
  if (message.getNumArgs() > 0) {
    this->_gui->loadFromFile(message.getArgAsString(0));
    ofLog() << "OscController: loaded parameters from " << message.getArgAsString(0) << ".";
  } else {
    this->_gui->loadFromFile("ofxWilkinson.json");
    ofLog() << "OscController: loaded parameters to ofxWilkinson.";
  }
}


Command OscController::getCommandFromPath(std::string & commandPath) {
  std::vector<Command>::iterator el = std::find_if(_controls.begin(), _controls.end(), 
    [&](const auto& val) {
      return val.path.compare(commandPath) == 0;
  });

  if(el != _controls.end()) {
    return *el;
  }
  return Command("", OSC_NULL, nullptr);
}

CommandType  OscController::getCommandTypeFromTypeString(std::string & type) {
  if(type==typeid(ofParameter<float>).name()){
    return OSC_FLOAT;
	}else if(type==typeid(ofParameter<int>).name()){
		return OSC_INT;
	}else if(type==typeid(ofParameter<bool>).name()){
		return OSC_BOOL;
	} else if(type== typeid(ofParameter<glm::vec2>).name()) {
    return OSC_VEC2;
  }
}