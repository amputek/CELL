#ifndef Tail_hpp
#define Tail_hpp

#include <iostream>
#include "Finite.hpp"

using namespace std;
using namespace ci;

class Tail{
    
public:
    Tail( int ln, bool wt, float wd, bool fn );
    
    ~Tail(){
        for( vector<Finite*>::iterator p = tail.begin(); p != tail.end(); ++p){
            delete *p;
        }
    }
    
    
    void update(vec2 loc, float d);
    void draw();
    void incLength( float a ){ length+=a; };
    void incWidth( float a ){ width+= a; };
    void setWide(){ wideTail = true; };
    void setFins(){ fins = true; };
    
private:
    void addFin(int top, int mid, int point);
    void updateTailPaths();
    
    vector<Finite*> tail;
    
    float direction;
    const float rotMod = M_PI/2.5;

    //attributes
    float length = 2;
    float width = 0.3;
    bool wideTail;
    bool fins;
    
    //drawing bits
    Path2d mPath;
    Path2d lPath;
    Path2d rPath;
    vector<Path2d> finPaths;
    vector<Path2d> ridgePaths;
    
    
    float newFiniteCounter = 0.0f;
};


#endif
