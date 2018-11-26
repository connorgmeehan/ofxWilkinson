#ifndef OFX_WILKINSON_UTILS
#define OFX_WILKINSON_UTILS

#include "ofMain.h"
#include "ofxCv.h"

namespace cv {
    template <typename GenType>
    inline GenType mix(GenType p1, GenType p2, float alpha) {
        return p1 + (p2 - p1) * alpha;
    }

    template <typename GenType>
    inline float distance(GenType p1, GenType p2) {
        return cv::norm(p1-p2);
    }

    // Converts open CV types to readable string i.e. CV_8UC3
    // Credit to Octopus on stack overflow
    // https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv 

    std::string type2str(int type);
};

#endif