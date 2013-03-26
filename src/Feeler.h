//
//  Feeler.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Feeler__
#define __Cellv0__Feeler__

#include <iostream>
#include "Spring.h"

class Feeler : public GameObject{
    
public:
    Feeler(Vec2f loc, int joints, float d);
    void update();
    void collide(Vec2f heroLoc);
    void addForce(Vec2f force);
    
    //getters
    bool contact(){ return feelerContact; };
    Vec2f getEnd(){ return springs.at(springs.size()-1)->local; };
    Path2d getPath(){ return path; };
    int length(){ return springs.size(); };
    vector<Vec2f> getPoints(){ return points; };
    
private:
    void updatePoints();
    Path2d path;
    bool feelerContact;
    vector<Vec2f> points;
    vector<Spring*> springs;
    const float accuracy = 30.0f;
    
};

#endif /* defined(__Cellv0__Feeler__) */
