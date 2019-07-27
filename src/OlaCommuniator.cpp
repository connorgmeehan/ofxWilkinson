#include "OlaCommunicator.h"

#ifdef OLA_INSTALLED

OlaCommunicator::OlaCommunicator {
    ofLog(OF_LOG_WARNING) << "---";
    ofLog(OF_LOG_WARNING) << " OlaCommunicator intitialised.";
    ofLog(OF_LOG_WARNING) << "---";

    ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);

        if (!_olaWrapper.Setup()) {
        std::cerr << "Setup failed" << endl;
        ofExit(1);
    }
}

void OlaCommunicator::setup(int width, int height, int strandCount, int strandLength) {
    _fboStep = width / strandLength;
    _dataLength = strandLength * 3;
    _dmxBuffers.resize(strandCount);

    for(auto & dmxbuffer : _dmxBuffers){
        dmxbuffer.Blackout();
    }
}

void OlaCommunicator::sendFrame(ofFbo & fbo){
    ofPixels frame;
    fbo.readToPixels(frame);

    ola::client::SendDMXArgs dmxArgs = ola::client::SendDMXArgs();
    for(int i = 0; i < _dmxBuffers.size(); i++){
        _dmxBuffers[i].Set( &frame.getPixels()[_dataLength*i*_fboStep], (unsigned int) _dataLength );
        _olaWrapper.GetClient()->SendDMX(i, _dmxBuffers.at(i), dmxArgs);
    }
}

#endif
