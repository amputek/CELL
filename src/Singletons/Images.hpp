#ifndef Images_hpp
#define Images_hpp

#include <stdio.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Log.h"

using namespace std;
using namespace ci::app;
using namespace ci;

class Images{
public:
    
    Images();
    
    cinder::gl::TextureRef * plankton(int i){ return &planktonImgs.at(i); };
    cinder::gl::TextureRef * spore(int i){    return &sporeImgs.at(i);    };
    cinder::gl::TextureRef * bubble(){        return &bubbleImg;          };
    cinder::gl::TextureRef * beam(){          return &beamImg;            };
    cinder::gl::TextureRef * jelly(int i ){   return &jellyImgs.at(i);    };
    cinder::gl::TextureRef * friendly(){      return &friendlyImg;        };
    cinder::gl::TextureRef * urchin(){        return &urchinImg;          };
    cinder::gl::TextureRef * egg(){           return &eggImg;             };
    cinder::gl::TextureRef * splash(){        return &splashImg;          };
    vector<gl::TextureRef> * player(){        return &playerImgs;         };

    cinder::gl::TextureRef * cinder(){        return &cinderSplashScreen; };
    cinder::gl::TextureRef * title(){         return &titleSplashScreen;  };
    cinder::gl::TextureRef * instructions(){  return &instructionImage;   };
    cinder::gl::TextureRef * mask(){          return &maskImg;            };
    
    
private:
    
    cinder::gl::TextureRef loadPngImage( string src ){
        string fullsrc = "images/" + src + ".png";
        try {
            return gl::Texture::create( loadImage( loadResource( fullsrc ) ) );
        } catch( Exception &exc ) {
            CI_LOG_EXCEPTION( "failed to load image.", exc );
            return nullptr;
        }
    }
    vector<gl::TextureRef> planktonImgs;
    vector<gl::TextureRef> sporeImgs;
    gl::TextureRef bubbleImg;
    gl::TextureRef beamImg;
    vector<gl::TextureRef> jellyImgs;
    gl::TextureRef friendlyImg;
    gl::TextureRef urchinImg;
    gl::TextureRef eggImg;
    vector<gl::TextureRef> playerImgs;
    gl::TextureRef titleSplashScreen;
    gl::TextureRef cinderSplashScreen;
    gl::TextureRef maskImg;
    gl::TextureRef splashImg;
    gl::TextureRef instructionImage;
    
};


#endif /* Images_hpp */
