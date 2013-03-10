//
//  Tail.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Tail__
#define __Cellv0__Tail__

#include <iostream>
#include "Finite.h"

using namespace std;

class Tail{
    
public:
    Tail( int ln, bool wt, float wd, bool fn );
    void update(Vec2f loc, float d);
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
};


#endif /* defined(__Cellv0__Tail__) */
