#ifndef EnvironmentManager_hpp
#define EnvironmentManager_hpp

#include <iostream>
#include "Splash.hpp"
#include "Beam.hpp"
#include "Bubble.hpp"
#include "Images.hpp"
#include "Mask.hpp"
#include "Feeler.hpp"
#include "SeaSurface.hpp"
#include "SeaFloor.hpp"

using namespace std;
using namespace ci;

class EnvironmentManager{
public:
    EnvironmentManager( Images* imgs );
    void splash( vec2 loc, int start, int end );
    void bubble( vec2 local, int amount );
    void update( vec2 heroLoc);
    void draw();
    void drawMask();
    
private:
    void updateBeams();
    void updateBubbles();
    void updateSplashes();
    void updateGrass(vec2 heroLoc);
    void drawGrass();
    
    //Collections and Objects
    Mask* mask;
    SeaSurface* surface;
    SeaFloor* floor;
    vector<Splash*> splashes;
    vector<Beam*> beams;
    vector<Bubble*> bubbles;
    vector<Feeler*> grass;
    
    Images* image;
    
};


#endif
