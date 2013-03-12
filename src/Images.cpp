#include "Images.h"

Images :: Images(){
    
    planktonImgs.push_back( new gl::Texture( loadImage( loadResource( "plankton_one.png"   ) ) ) );
    planktonImgs.push_back( new gl::Texture( loadImage( loadResource( "plankton_tri.png"   ) ) ) );
    planktonImgs.push_back( new gl::Texture( loadImage( loadResource( "plankton_three.png" ) ) ) );
    planktonImgs.push_back( new gl::Texture( loadImage( loadResource( "plankton_long.png"  ) ) ) );
    planktonImgs.push_back( new gl::Texture( loadImage( loadResource( "plankton_four.png"  ) ) ) );
    
    beamImg =               new gl::Texture( loadImage( loadResource( "beam.png"           ) ) );
    bubbleImg =             new gl::Texture( loadImage( loadResource( "bubble.png"         ) ) );
    
    
    sporeImgs.push_back(    new gl::Texture( loadImage( loadResource( "spore_orange.png"   ) ) ) );
    sporeImgs.push_back(    new gl::Texture( loadImage( loadResource( "spore_green.png"    ) ) ) );
    sporeImgs.push_back(    new gl::Texture( loadImage( loadResource( "spore_pink.png"     ) ) ) );
    
    
    playerImgs.push_back(   new gl::Texture( loadImage( loadResource( "player_oneeye.png"  ) ) ) );
    playerImgs.push_back(   new gl::Texture( loadImage( loadResource( "player_twoeye.png"  ) ) ) );
    playerImgs.push_back(   new gl::Texture( loadImage( loadResource( "player_simple.png"  ) ) ) );
    playerImgs.push_back(   new gl::Texture( loadImage( loadResource( "player_ring.png"    ) ) ) );

    jellyImgs.push_back(    new gl::Texture( loadImage( loadResource( "jelly_blue.png"     ) ) ) );
    jellyImgs.push_back(    new gl::Texture( loadImage( loadResource( "jelly_green.png"    ) ) ) );
    jellyImgs.push_back(    new gl::Texture( loadImage( loadResource( "jelly_pink.png"     ) ) ) );
    
    friendlyImg =           new gl::Texture( loadImage( loadResource( "friendly.png"       ) ) );
    urchinImg =             new gl::Texture( loadImage( loadResource( "twirl.png"          ) ) );
    eggImg =                new gl::Texture( loadImage( loadResource( "egg.png"            ) ) );
    
    titleSplashScreen =     new gl::Texture( loadImage( loadResource( "cell-splashscreen.png" ) ) );
    maskImg =               new gl::Texture( loadImage( loadResource( "mask-1.png"            ) ) );
    
    menuImg =               new gl::Texture( loadImage( loadResource( "menu.png"            ) ) );
    menuYes =               new gl::Texture( loadImage( loadResource( "menu-yes.png"        ) ) );
    menuNo =                new gl::Texture( loadImage( loadResource( "menu-no.png"         ) ) );
    
}