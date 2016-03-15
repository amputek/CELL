#ifndef Urchin_hpp
#define Urchin_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Feeler.hpp"

class Urchin : public Swimmer{
public:
    Urchin(vec2 loc, gl::TextureRef* tex);
    void update();
    void draw();
    void collide(vec2 loc);
    int contactAmount(){ return contacts; };
    
private:
    void updateFeelers();
    vector<Feeler*> feelers;
    vector<Path2d> paths;
    gl::TextureRef* img;
    int contacts;
};

#endif
