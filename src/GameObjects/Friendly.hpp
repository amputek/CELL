#ifndef Friendly_hpp
#define Friendly_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Tail.hpp"

class Friendly : public Swimmer{
public:
    Friendly(vec2 loc);
    ~Friendly(){
        delete tail;
    }
    
    
    void draw( CellRenderer & renderer );
    void update();
    void birth();
    
    //getters
    bool born(){ return mborn; };
    
private:
    float levelCounter = 0.0f;
    Tail* tail;
    bool mborn = false;
    bool birthing = false;
    float birthCount = 0.0f;
};


#endif
