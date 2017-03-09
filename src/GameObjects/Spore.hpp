//Spores are circular egg-like objects that react to collisions
//After certain number of collisions, spores will hatch eggs
//

#ifndef Spore_hpp
#define Spore_hpp

#include <iostream>
#include "Dust.hpp"
#include "Renderer.hpp"

using namespace std;

class Spore : public Dust{
public:
    Spore(vec2 loc, float depth, int type);
    void update();
    void draw( CellRenderer * renderer );
    void collide( const vec2 & loc);
    
    //getters
    int getType() const{ return mSporeType; };
    bool isInContact() const { return mContactFlag; };
    bool isAlive() const { return mHealth > 0; };
    int getHealth() const { return mHealth; }
    
private:
    int mHealth;
    int mSporeType;      //three different types of Spore
    bool mContactFlag;  //allows EntityManager to check if contact has been made
};



#endif
