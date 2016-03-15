//
//  Images.cpp
//  Cell09
//
//  Created by Rob Dawson on 13/03/2016.
//
//

#include "Images.hpp"

Images :: Images(){
    
    planktonImgs.push_back( loadPngImage( "plankton_one"    ) );
    planktonImgs.push_back( loadPngImage( "plankton_tri"    ) );
    planktonImgs.push_back( loadPngImage( "plankton_three"  ) );
    planktonImgs.push_back( loadPngImage( "plankton_long"   ) );
    planktonImgs.push_back( loadPngImage( "plankton_four"   ) );
    planktonImgs.push_back( loadPngImage( "plankton_square" ) );
    planktonImgs.push_back( loadPngImage( "plankton_big"    ) );
    
    beamImg =               loadPngImage( "beam"            );
    bubbleImg =             loadPngImage( "bubble"          );
    
    sporeImgs.push_back(    loadPngImage( "spore_orange"    ) );
    sporeImgs.push_back(    loadPngImage( "spore_green"     ) );
    sporeImgs.push_back(    loadPngImage( "spore_pink"      ) );
    
    playerImgs.push_back(   loadPngImage( "player_zero"     ) );
    playerImgs.push_back(   loadPngImage( "player_one"      ) );
    playerImgs.push_back(   loadPngImage( "player_two"      ) );
    
    jellyImgs.push_back(    loadPngImage( "jelly_blue"      ) );
    jellyImgs.push_back(    loadPngImage( "jelly_green"     ) );
    jellyImgs.push_back(    loadPngImage( "jelly_pink"      ) );
    
    friendlyImg =           loadPngImage( "friendly"        );
    urchinImg =             loadPngImage( "twirl"           );
    eggImg =                loadPngImage( "egg"             );
    
    titleSplashScreen =     loadPngImage( "cell-splashscreen" );
    instructionImage  =     loadPngImage( "instructions"      );
    maskImg =               loadPngImage( "mask-1"            );

    
//    loadPngImage( "mask-3"            );
}