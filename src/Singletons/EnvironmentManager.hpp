#ifndef EnvironmentManager_hpp
#define EnvironmentManager_hpp

#include <iostream>
#include "Splash.hpp"
#include "Beam.hpp"
#include "Bubble.hpp"
#include "Renderer.hpp"
#include "Mask.hpp"

using namespace std;
using namespace ci;

class EnvironmentManager{
public:
    EnvironmentManager(){};
    EnvironmentManager( CellRenderer * imgs, vector<GameObject*> * entities  );
    void splash( const vec2 & loc, int start, int end );
    void bubble( const vec2 & local, int amount );
    void update( const vec2 & heroPosition );
    void drawMask();
    
private:

    //Pointer to renderer's mask
    Mask * mask;
    
    //Pointer to actual entity collection
    vector<GameObject*> * entities;
    
    //Pointer to renderer
    CellRenderer * renderer;
    
};


#endif
