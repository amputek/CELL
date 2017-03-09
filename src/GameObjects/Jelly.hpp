#ifndef Jelly_hpp
#define Jelly_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Feeler.hpp"
#include "Renderer.hpp"

class Jelly : public Swimmer{
public:
    Jelly(vec2 loc, int type);
    ~Jelly(){
        for( vector<Feeler*>::iterator p = feelers.begin(); p != feelers.end(); ++p ){
            delete *p;
        }
    }
    
    void update();
    void draw( CellRenderer & renderer );
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
    float feelerStartRadius = 0.5f; //modifies the radius of the feeler positions (depends on the png being used)
    int jellyType = 0;

};

#endif
