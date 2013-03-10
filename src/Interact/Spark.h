//
//  Spark.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Spark__
#define __Cellv0__Spark__

#include <iostream>
#include "Swimmer.h"
#include "Finite.h"

class Spark : public Swimmer{
public:
    Spark(Vec2f loc, int t);
    void draw();
    void update();

private:
    vector<Finite*> finites;
    int type;
};

#endif /* defined(__Cellv0__Spark__) */
