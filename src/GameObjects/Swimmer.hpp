#ifndef Swimmer_hpp
#define Swimmer_hpp

#include <iostream>
#include "Braitenberg.hpp"

using namespace std;

class Swimmer : public Braitenberg{
    
public:
    Swimmer(vec2 loc);
    void update();
    void updateTarget(vector<GameObject*> movers);
    void setDestination(vec2 dest){ targetDestination = dest; };
    
    bool inSpace;
    
private:
    bool checkInSpace(vec2 loc, vector<GameObject*> movers);
    vec2 targetLocation;
    vec2 targetDestination;
};



#endif