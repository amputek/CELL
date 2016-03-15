// Reacts to player proximity
// Changes colours and 'curls up'

#ifndef Starfish_hpp
#define Starfish_hpp

#include <iostream>
#include "Feeler.hpp"
#include "Swimmer.hpp"

class Starfish : public Swimmer{
public:
    Starfish(vec2 loc);
    void update();
    void draw();
    void collide(vec2 loc);
    
    //getters
    bool activated();
    
private:
    vector<Feeler*> feelers;
    float spinCounter;
    int contacts;
};



#endif
