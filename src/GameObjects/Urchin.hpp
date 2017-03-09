#ifndef Urchin_hpp
#define Urchin_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Feeler.hpp"
#include "Renderer.hpp"

class Urchin : public Swimmer{
public:
    Urchin(vec2 loc);
    
    ~Urchin(){
        for( vector<Feeler*>::iterator p = mFeelers.begin(); p != mFeelers.end(); ++p){
            delete *p;
        }
    }
    
    void update();
    void draw( CellRenderer & renderer );
    void collide(const vec2 & loc, float colliderSize);
    
    
    int getContactAmount() const {
        return mContactCount;
    };
    
private:
    void updateFeelers();
    vector<Feeler*> mFeelers;
    int mContactCount;
};

#endif
