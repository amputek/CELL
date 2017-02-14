#ifndef Feeler_hpp
#define Feeler_hpp

#include <iostream>
#include "Spring.hpp"

using namespace ci;

class Feeler : public GameObject{
    
public:
    Feeler(vec2 loc, int joints, float d);
    void update();
    void collide(vec2 heroLoc);
    void addForce(vec2 force);
    
    //getters
    bool contact(){ return feelerContact; };
    vec2 getEnd(){ return springs.at(springs.size()-1)->local; };
    Path2d getPath(){ return path; };
    int length(){ return springs.size(); };
    vector<vec2> * getPoints(){ return &points; };
    
private:
    void updatePoints();
    Path2d path;
    bool feelerContact;
    vector<vec2> points;
    vector<Spring*> springs;
    const float accuracy = 30.0f;
    
};

#endif
