#ifndef Spark_hpp
#define Spark_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Finite.hpp"

class Spark : public Swimmer{
public:
    Spark( const vec2 & loc, int sparkType, gl::TextureRef * tex );
    ~Spark()
    {
        for( vector<Finite*>::iterator p = finites.begin(); p != finites.end(); ++p){
            delete *p;
        }
    }
    void draw();
    void update();

private:
    vector<Finite*> finites;    //collection of positions to draw circles
    int type;
    gl::TextureRef* img;
    
    float life = 0.0f;
};

#endif
