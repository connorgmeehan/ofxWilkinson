#include "ofMain.h"

#ifdef OLA_INSTALLED

#include <ola/DmxBuffer.h>
#include <ola/io/SelectServer.h>
#include <ola/Logging.h>
#include <ola/client/OlaClient.h>
#include <ola/client/ClientWrapper.h>
#include <ola/Callback.h>

class OlaCommunicator {
    int _strandLength;
    int _fboStep; // length of step 
    int _dataLength; // Length of entire RGB data array (3 * strandLength)
    std::vector<ola::DmxBuffer> _dmxBuffers;
    ola::client::OlaClientWrapper _olaWrapper;
     public:
        OlaCommunicator();

        void setup(int width, int height, int strandCount, int strandLength);
        void sendFrame(ofFbo & fbo);
};
#endif

#ifndef OLA_INSTALLED

#include "ofLog.h";
// STUB
class OlaCommunicator{
    public: 
        OlaCommunicator(){
            ofLog(OF_LOG_WARNING) << "---";
            ofLog(OF_LOG_WARNING) << " OlaCommunicator stub intitialised.  Wont be able to communicate with LEDs.";
            ofLog(OF_LOG_WARNING) << "---";
        }

        void setup(int width, int height, int strandCount, int strandLength) {}

        void sendFrame(ofFbo & fbo){}
};
#endif
