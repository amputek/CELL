// Reacts to player proximity
// Changes colours and 'curls up'

#ifndef Starfish_hpp
#define Starfish_hpp

#include <iostream>
#include "Feeler.hpp"
#include "Swimmer.hpp"
#include "Renderer.hpp"

class Starfish : public Swimmer{
public:
    Starfish(vec2 loc);
    ~Starfish()
    {
        for( vector<Feeler*>::iterator p = mFeelers.begin(); p != mFeelers.end(); ++p){
            delete *p;
        }
    }
    void update();
    void draw( CellRenderer * renderer );
    void collide(const vec2 & loc);
    bool isFleeing() const{ return mFleeing; }

    //getters
    bool reachedContactThreshold();

    
private:
    vector<Feeler*> mFeelers;
    float mSpinCounter;
    float mContacts = 0.0f;
    bool mFleeing = false;
    

};



#endif
