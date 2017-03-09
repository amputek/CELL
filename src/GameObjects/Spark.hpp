#ifndef Spark_hpp
#define Spark_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Finite.hpp"

class Spark : public Swimmer{
public:
    Spark( const vec2 & loc, int sparkType );
    ~Spark()
    {
        for( vector<Finite*>::iterator p = finites.begin(); p != finites.end(); ++p){
            delete *p;
        }
    }
    void draw( CellRenderer & renderer );
    void update();
    void pulse()
    {
        mRadius = 24.0f;
    }

private:
    vector<Finite*> finites;    //collection of positions to draw circles
    int type;
    
    float life = 0.0f;
};

#endif
