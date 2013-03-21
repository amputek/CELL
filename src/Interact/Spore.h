//Spores are circular egg-like objects that react to collisions
//After certain number of collisions, spores will hatch eggs
//

#ifndef __Cellv0__Spore__
#define __Cellv0__Spore__

#include <iostream>
#include "Dust.h"

using namespace std;

class Spore : public Dust{
public:
    Spore(Vec2f loc, float d, int t, gl::Texture* tex);
    void update();
    void draw();
    void collide(Vec2f loc);
    
    //getters
    int type(){ return sporeType; };
    bool contact(){ return sporeContact; };
    bool alive(){ return health > 0; };
    
    
    int health;
private:
    gl::Texture* img;
    int sporeType;      //three different types of Spore
    bool sporeContact;  //allows EntityManager to check if contact has been made
};



#endif /* defined(__Cellv0__Spore__) */
