#ifndef Tail_hpp
#define Tail_hpp

#include <iostream>
#include "Finite.hpp"
#include "GameObject.hpp"

using namespace std;
using namespace ci;

class Tail : public IDrawable{
    
public:
    Tail( int ln, bool wt, float wd, bool fn );
    
    ~Tail(){
        for( vector<Finite*>::iterator p = tail.begin(); p != tail.end(); ++p){
            delete *p;
        }
    }
    
    
    void update(vec2 loc, float d);
    void draw( CellRenderer & renderer );
    void incLength( float a ){ length+=a; };
    void incWidth( float a ){ width+= a; };
    void setWide(){ wideTail = true; };
    void setFins(){ fins = true; };
    
private:

    vector<Finite*> tail;
    
    float direction;
    const float rotMod = M_PI/2.5;

    //attributes
    float length = 2;
    float width = 0.3;
    bool wideTail;
    bool fins;
    

    float newFiniteCounter = 0.0f;
};


#endif
