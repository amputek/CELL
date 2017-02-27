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
    Spore(vec2 loc, float depth, int type, gl::TextureRef* tex);
    void update();
    void draw();
    void collide(vec2 loc);
    
    //getters
    int type(){ return sporeType; };
    bool contact(){ return sporeContact; };
    bool alive(){ return health > 0; };
    
    
    int health;
private:
    gl::TextureRef* img;
    int sporeType;      //three different types of Spore
    bool sporeContact;  //allows EntityManager to check if contact has been made
};



#endif
