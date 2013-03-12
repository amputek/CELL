//
// Image class that loads and stores image references
// Cleaner than keeping it inside the Entity Manager

#ifndef __Cellv0__Images__
#define __Cellv0__Images__

#include <iostream>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"

using namespace std;
using namespace ci::app;
using namespace ci;

class Images{
public:
    
    Images();
    
    cinder::gl::Texture* plankton(int i){ return planktonImgs.at(i); };
    cinder::gl::Texture* spore(int i){    return sporeImgs.at(i);    };
    cinder::gl::Texture* bubble(){        return bubbleImg;          };
    cinder::gl::Texture* beam(){          return beamImg;            };
    cinder::gl::Texture* jelly(int i ){   return jellyImgs.at(i);    };
    cinder::gl::Texture* friendly(){      return friendlyImg;        };
    cinder::gl::Texture* urchin(){        return urchinImg;          };
    cinder::gl::Texture* egg(){           return eggImg;             };
    vector<cinder::gl::Texture*> player(){return playerImgs;         };
    cinder::gl::Texture* title(){         return titleSplashScreen;  };
    cinder::gl::Texture* cinder(){        return cinderSplashScreen; };
    cinder::gl::Texture* mask(){          return maskImg;            };
    cinder::gl::Texture* menu(){          return menuImg;            };
    cinder::gl::Texture* menuy(){         return menuYes;            };
    cinder::gl::Texture* menun(){         return menuNo;             };
    
private:
    vector<gl::Texture*> planktonImgs;
    vector<gl::Texture*> sporeImgs;
    gl::Texture* bubbleImg;
    gl::Texture* beamImg;
    vector<gl::Texture*> jellyImgs;
    gl::Texture* friendlyImg;
    gl::Texture* urchinImg;
    gl::Texture* eggImg;
    vector<gl::Texture*> playerImgs;
    gl::Texture* titleSplashScreen;
    gl::Texture* cinderSplashScreen;
    gl::Texture* maskImg;
    
    gl::Texture* menuImg;
    gl::Texture* menuYes;
    gl::Texture* menuNo;

};



#endif /* defined(__Cellv0__Images__) */
