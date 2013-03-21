//
//  EnvironmentManager.h
//  Cellv0
//
//  Created by Rob on 09/03/2013.
//
//

#ifndef __Cellv0__EnvironmentManager__
#define __Cellv0__EnvironmentManager__

#include <iostream>
#include "Splash.h"
#include "Beam.h"
#include "Bubble.h"
#include "Images.h"
#include "Mask.h"
#include "SeaSurface.h"

using namespace std;

class EnvironmentManager{
public:
    EnvironmentManager( Images* imgs );
    void splash( Vec2f loc, int start, int end );
    void bubble( Vec2f local, int amount );
    void update( Vec2f heroLoc);
    void draw();
    void drawMask();
    
private:
    void updateBeams();
    void updateBubbles();
    void updateSplashes();
    
    Mask* mask;
    SeaSurface* surface;
    //  Floor* floor;
    vector<Splash*> splashes;
    vector<Beam*> beams;
    vector<Bubble*> bubbles;
    
    Images* image;
    
};


#endif /* defined(__Cellv0__EnvironmentManager__) */
