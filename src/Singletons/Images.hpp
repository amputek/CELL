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
    
    gl::TextureRef sporeImgs[3][3];
    gl::TextureRef planktonImgs[5];
    gl::TextureRef grassBaseImg;
    gl::TextureRef cursorImg;
    gl::TextureRef bubbleImgs[3];
    gl::TextureRef beamImg;
    gl::TextureRef jellyImgs[3];
    gl::TextureRef friendlyImg;
    gl::TextureRef urchinImg;
    gl::TextureRef eggImg;
    gl::TextureRef starImg;
    gl::TextureRef sparkImg;
    gl::TextureRef playerImgs[3];
    gl::TextureRef titleSplashScreen;
    gl::TextureRef titleSplashScreenBlur;
    gl::TextureRef title1;
    gl::TextureRef title2;
    gl::TextureRef title3;
    gl::TextureRef title4;
    gl::TextureRef cinderSplashScreen;
    gl::TextureRef maskImg;
    gl::TextureRef splashImg;
    gl::TextureRef instructionImage;
    
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

    
};


#endif /* Images_hpp */
