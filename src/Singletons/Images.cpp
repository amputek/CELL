#include "Images.hpp"

Images :: Images(){
    
    planktonImgs[0] = loadPngImage( "plankton/a" );
    planktonImgs[1] = loadPngImage( "plankton/b" );
    planktonImgs[2] = loadPngImage( "plankton/c" );
    planktonImgs[3] = loadPngImage( "plankton/d" );
    planktonImgs[4] = loadPngImage( "plankton/e" );
    
    beamImg =       loadPngImage( "beam_new"            );

    bubbleImgs[0] = loadPngImage( "bubble/0"     );
    bubbleImgs[1] = loadPngImage( "bubble/1"     );
    bubbleImgs[2] = loadPngImage( "bubble/2" );
    
    splashImg =             loadPngImage( "splash"            );
    sparkImg =              loadPngImage( "spark"            );

    cursorImg =              loadPngImage( "cursor"            );

    sporeImgs[0][0] = loadPngImage( "spore/orange_0" );
    sporeImgs[0][1] = loadPngImage( "spore/orange_1" );
    sporeImgs[0][2] = loadPngImage( "spore/orange_2" );

    sporeImgs[1][0] = loadPngImage( "spore/purple_0" );
    sporeImgs[1][1] = loadPngImage( "spore/purple_1" );
    sporeImgs[1][2] = loadPngImage( "spore/purple_2" );
    
    sporeImgs[2][0] = loadPngImage( "spore/green_0" );
    sporeImgs[2][1] = loadPngImage( "spore/green_1" );
    sporeImgs[2][2] = loadPngImage( "spore/green_2" );

    starImg =             loadPngImage( "bubble/2"            );
    
    playerImgs[0] = loadPngImage( "player_zero2"    );
    playerImgs[1] = loadPngImage( "player_one"      );
    playerImgs[2] = loadPngImage( "player_two"      );
    
    jellyImgs[0] = loadPngImage( "jelly_blue"      );
    jellyImgs[1] = loadPngImage( "jelly_green"     );
    jellyImgs[2] = loadPngImage( "jelly_pink"      );
    
    friendlyImg =           loadPngImage( "friendly"        );
    urchinImg =             loadPngImage( "twirl"           );
    eggImg =                loadPngImage( "egg"             );
    
    
    title1     =     loadPngImage( "title/1" );
    title2     =     loadPngImage( "title/2" );
    title3     =     loadPngImage( "title/3" );
    title4     =     loadPngImage( "title/4" );

    maskImg =               loadPngImage( "mask-1"            );

    
//    loadPngImage( "mask-3"            );
}
