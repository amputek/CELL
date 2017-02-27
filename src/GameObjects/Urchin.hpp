#ifndef Urchin_hpp
#define Urchin_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Feeler.hpp"

class Urchin : public Swimmer{
public:
    Urchin(vec2 loc, gl::TextureRef* tex);
    
    ~Urchin(){
        for( vector<Feeler*>::iterator p = feelers.begin(); p != feelers.end(); ++p){
            delete *p;
        }
    }
    
    void update();
    void draw();
    void collide(const vec2 & loc, float colliderSize);
    int contactAmount(){
        int n = contactCount;
        //if(contactCount > 0 ) contactCount = 0;
        return n;
    };
    
private:
    void updateFeelers();
    vector<Feeler*> feelers;
    gl::TextureRef* img;
    int contactCount;
};

#endif
