#include "ofMain.h"

#ifdef OLA_INSTALLED
#include <ola/DmxBuffer.h>
#include <ola/io/SelectServer.h>
#include <ola/Logging.h>
#include <ola/client/OlaClient.h>
#include <ola/client/ClientWrapper.h>
#include <ola/Callback.h>

class OlaCommunicator {
    bool debug = true;
    ofFbo * debugFbo;
    std::string mAddress;
    int mPort = -1;
    int array_height = 16;
    int array_width = 50;
    std::vector<ola::DmxBuffer> dmxbuffers;
    ola::client::OlaClientWrapper wrapper;
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

        void setup(int _width, int _height) {
            dmxbuffers.resize(array_height);

            for(auto & dmxbuffer : dmxbuffers){
                dmxbuffer.Blackout();
            }
        }

        void sendFrame(ofFbo & fbo){
            ofPixels frame;
            fbo.readToPixels(frame);

            int fbo_step = 4;

            ola::client::SendDMXArgs dummy_args = ola::client::SendDMXArgs();
            for(int i = 0; i < dmxbuffers.size(); i++){
                dmxbuffers[i].Set( &frame.getPixels()[150*i*fbo_step], (unsigned int) 150 );
                wrapper.GetClient()->SendDMX(i, dmxbuffers.at(i), dummy_args);
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