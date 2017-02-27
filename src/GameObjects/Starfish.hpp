// Reacts to player proximity
// Changes colours and 'curls up'

#ifndef Starfish_hpp
#define Starfish_hpp

#include <iostream>
#include "Feeler.hpp"
#include "Swimmer.hpp"

class Starfish : public Swimmer{
public:
    Starfish(vec2 loc, gl::TextureRef* tex);
    ~Starfish()
    {
        for( vector<Feeler*>::iterator p = feelers.begin(); p != feelers.end(); ++p){
            delete *p;
        }
    }
    void update();
    void draw();
    void collide(const vec2 & loc);
    
    bool fleeing = false;
    
    //getters
    bool activated();
    float contacts = 0.0f;
private:
    vector<Feeler*> feelers;
    float spinCounter;
;
    gl::TextureRef* img;
};



#endif
