#include "ofMain.h"

#ifdef OLA_INSTALLED

#include <ola/DmxBuffer.h>
#include <ola/io/SelectServer.h>
#include <ola/Logging.h>
#include <ola/client/OlaClient.h>
#include <ola/client/ClientWrapper.h>
#include <ola/Callback.h>

class OlaCommunicator {
    int _arrayWidth, _arrayHeight;
    int _strandLength;
    std::vector<ola::DmxBuffer> _dmxBuffers;
    ola::client::OlaClientWrapper _olaWrapper;
     public:

        OlaCommunicator(){

            ofLog(OF_LOG_WARNING) << "---";
            ofLog(OF_LOG_WARNING) << " OlaCommunicator intitialised.";
            ofLog(OF_LOG_WARNING) << "---";

            ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);

             if (!wrapper.Setup()) {
                std::cerr << "Setup failed" << endl;
                ofExit(1);
            }
        }

        void setup(int width, int height, int strandCount, int strandLength) {
            _arrayWidth = arrayWidth;
            _arrayHeight = arrayHeight;
            
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

            ola::client::SendDMXArgs dummy_args = ola::client::SendDMXArgs();
            for(int i = 0; i < _dmxBuffers.size(); i++){
                _dmxBuffers[i].Set( &frame.getPixels()[_strandLength*i*_fboStep], (unsigned int) 150 );
                _olaWrapper.GetClient()->SendDMX(i, _dmxBuffers.at(i), dummy_args);
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

        void sendFrame(ofFbo & fbo){

        }
};
#endif