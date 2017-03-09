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
    ~Starfish()
    {
        for( vector<Feeler*>::iterator p = feelers.begin(); p != feelers.end(); ++p){
            delete *p;
        }
    }
    void update();
    void draw( CellRenderer & renderer );
    void collide(const vec2 & loc);
    
    bool fleeing = false;
    
    //getters
    bool activated();
    float contacts = 0.0f;
private:
    vector<Feeler*> feelers;
    float spinCounter;

};



#endif
