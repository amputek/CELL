#include "Renderer.hpp"

CellRenderer :: CellRenderer(){
    
    planktonImgs[0] = loadPngImage( "plankton/a" );
    planktonImgs[1] = loadPngImage( "plankton/b" );
    planktonImgs[2] = loadPngImage( "plankton/c" );
    planktonImgs[3] = loadPngImage( "plankton/d" );
    planktonImgs[4] = loadPngImage( "plankton/e" );
    
    beamImg =       loadPngImage( "environment/beam3"            );
    bubbleImgs[0] = loadPngImage( "environment/bubble/0"     );
    bubbleImgs[1] = loadPngImage( "environment/bubble/1"     );
    bubbleImgs[2] = loadPngImage( "environment/bubble/2" );
    splashImg =     loadPngImage( "environment/splash"            );
    maskImg =       loadPngImage( "environment/mask-1"            );
    
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

    playerImgs[0] = loadPngImage( "player/zero2"    );
    playerImgs[1] = loadPngImage( "player/one"      );
    playerImgs[2] = loadPngImage( "player/two"      );
    
    jellyImgs[0] = loadPngImage( "jelly/blue"      );
    jellyImgs[1] = loadPngImage( "jelly/green"     );
    jellyImgs[2] = loadPngImage( "jelly/pink"      );
    
    friendlyImg = loadPngImage( "friendly"        );
    urchinImg =   loadPngImage( "twirl"           );
    eggImg =      loadPngImage( "egg"             );
    sparkImg =    loadPngImage( "spark"            );
    
    title1     =     loadPngImage( "title/1" );
    title2     =     loadPngImage( "title/2" );
    title3     =     loadPngImage( "title/3" );
    title4     =     loadPngImage( "title/4" );



    gl::Fbo::Format format;
    miniMap = gl::Fbo::create( 800, 600, format );
    
    

    
//    loadPngImage( "mask-3"            );
}
