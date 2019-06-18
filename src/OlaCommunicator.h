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

        OlaCommunicator(){

            ofLog(OF_LOG_WARNING) << "---";
            ofLog(OF_LOG_WARNING) << " OlaCommunicator intitialised.";
            ofLog(OF_LOG_WARNING) << "---";

            ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);

             if (!_olaWrapper.Setup()) {
                std::cerr << "Setup failed" << endl;
                ofExit(1);
            }
        }

        void setup(int width, int height, int strandCount, int strandLength) {
            _fboStep = width / strandLength;
            _dataLength = strandLength * 3;
            _dmxBuffers.resize(strandCount);

            for(auto & dmxbuffer : _dmxBuffers){
                dmxbuffer.Blackout();
            }
        }

        void sendFrame(ofFbo & fbo){
            ofPixels frame;
            fbo.readToPixels(frame);

            ola::client::SendDMXArgs dmxArgs = ola::client::SendDMXArgs();
            for(int i = 0; i < _dmxBuffers.size(); i++){
                _dmxBuffers[i].Set( &frame.getPixels()[_dataLength*i*_fboStep], (unsigned int) _dataLength );
                _olaWrapper.GetClient()->SendDMX(i, _dmxBuffers.at(i), dmxArgs);
            }
        }
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