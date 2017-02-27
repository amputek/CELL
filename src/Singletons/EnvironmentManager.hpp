#ifndef EnvironmentManager_hpp
#define EnvironmentManager_hpp

#include <iostream>
#include "Splash.hpp"
#include "Beam.hpp"
#include "Bubble.hpp"
#include "Images.hpp"
#include "Mask.hpp"

using namespace std;
using namespace ci;

class EnvironmentManager{
public:
    EnvironmentManager( Images* imgs );
    void splash( vec2 loc, int start, int end );
    void bubble( vec2 local, int amount );
    void update( const vec2 & heroGlobal, const vec2 & heroLocal);
    void draw();
    void drawMask();
    
private:
    void updateBeams();
    void updateBubbles();
    void updateSplashes();
    
    //Collections and Objects
    Mask* mask;
    vector<Splash*> splashes;
    vector<Beam*> beams;
    vector<Bubble*> bubbles;

    
    Images* image;
    
};


#endif
