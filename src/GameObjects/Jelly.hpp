#ifndef Jelly_hpp
#define Jelly_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Feeler.hpp"

class Jelly : public Swimmer{
    
public:
    Jelly(vec2 loc, int type, gl::TextureRef* tex);
    void update();
    void draw();
    void collide(vec2 loc);
    vector<bool> * contacts(){ return &jellyContacts; };
    
private:
    vector<Feeler*> feelers;
    vector<Path2d> paths;
    vector<bool> jellyContacts;
    
    float counter;
    gl::TextureRef* img;
    
    float rMod; //modifies the radius of the feeler positions (depends on the png being used)
};

#endif
