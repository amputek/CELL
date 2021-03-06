#ifndef Jelly_hpp
#define Jelly_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Feeler.hpp"

class Jelly : public Swimmer{
public:
    Jelly(vec2 loc, int type, gl::TextureRef* tex);
    ~Jelly(){
        for( vector<Feeler*>::iterator p = feelers.begin(); p != feelers.end(); ++p ){
            delete *p;
        }
    }
    
    void update();
    void draw();
    void collide(const vec2 & loc, float colliderSize);
    vector<bool> contacts(){
        vector<bool> temp;
        for(int i = 0; i < feelers.size(); i++ )
            temp.push_back( feelers.at(i)->inContactWithCollider() );
        return temp;
    };
    

    
private:
    
    vector<Feeler*> feelers;
    
    float counter = 0.0f;
    gl::TextureRef* img;
    
    float feelerStartRadius = 0.5f; //modifies the radius of the feeler positions (depends on the png being used)
    int jellyType = 0;

};

#endif
