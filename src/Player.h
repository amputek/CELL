//
//  Player.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__Player__
#define __Cellv0__Player__

#include <iostream>
#include "Braitenberg.h"
#include "Tail.h"

using namespace ci::app;

class Player : public Braitenberg{
public:
    Player(Vec2f loc, vector<gl::Texture*> texs);
    void draw();
    void update();
    
    void moveTo(Vec2f mousePos);
    void incEaten(int type);
    int getEaten(){ return planktonEaten.at(0); };
    bool levelling(){ return playerLevelling; };
    
private:
    void levelUp();
    
    gl::Texture* img;
    vector<gl::Texture*> imgs;
    int levelCount;
    Tail* longTail;
    bool falling;
    bool playerLevelling;
    vector<int> planktonEaten;
    int level;
    
    // Tail* leftTail;
    // Tail* rightTail;
    
};



#endif /* defined(__Cellv0__Player__) */
