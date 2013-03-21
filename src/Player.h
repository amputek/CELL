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
    
    //getters
    int getEaten(){ return planktonEaten.at(0); };
    bool levelling(){ return playerLevelling; };
    
private:
    void levelUp();
    
    gl::Texture* img;           //current image
    vector<gl::Texture*> imgs;  //all possible images a player could be
    int levelCount;             //a counter for the levelling process
    Tail* longTail;
    bool falling;               //true if player is above surface
    bool playerLevelling;       //true if player is levelling
    vector<int> planktonEaten;  //number of each type of plankton eaten
    int level;                  //current level
};



#endif /* defined(__Cellv0__Player__) */
