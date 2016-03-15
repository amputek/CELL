#ifndef Spark_hpp
#define Spark_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Finite.hpp"

class Spark : public Swimmer{
public:
    Spark(vec2 loc, int t);
    void draw();
    void update();

private:
    vector<Finite*> finites;    //collection of positions to draw circles
    int type;
};

#endif
