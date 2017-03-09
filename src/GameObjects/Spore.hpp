//Spores are circular egg-like objects that react to collisions
//After certain number of collisions, spores will hatch eggs
//

#ifndef Spore_hpp
#define Spore_hpp

#include <iostream>
#include "Dust.hpp"

using namespace std;

class Spore : public Dust{
public:
    Spore(vec2 loc, float depth, int type);
    void update();
    void draw( CellRenderer & renderer );
    void collide( const vec2 & loc);
    
    //getters
    int type(){ return sporeType; };
    bool contact(){ return sporeContact; };
    bool alive(){ return health > 0; };
    int getHealth(){ return health; }
    
private:
    int health;
    int sporeType;      //three different types of Spore
    bool sporeContact;  //allows EntityManager to check if contact has been made
};



#endif
