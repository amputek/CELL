#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include "Braitenberg.hpp"
#include "Tail.hpp"

using namespace ci::app;

class Player : public Braitenberg{
public:
    Player(vec2 loc, vector<gl::TextureRef> * texs);
    void draw();
    void update();
    void moveTo(vec2 mousePos);
    void incEaten(int type){ planktonEaten.at(type)++; };
    void slow(bool t);
    
    //getters
    int getEaten();
    bool levelling(){ return playerLevelling; };
    void levelUp();
    
private:
    
    gl::TextureRef* img;           //current image
    vector<gl::TextureRef> * imgs;  //all possible images a player could be
    int levelCount;             //a counter for the levelling process
    Tail* longTail;
    Tail* rightTail;
    Tail* leftTail;
    bool falling;               //true if player is above surface
    bool playerLevelling;       //true if player is levelling
    bool sideTailsOn = false;    //the shorter side tails that the player has at a high enough level
    vector<int> planktonEaten;  //number of each type of plankton eaten
    int level;                  //current level
};



#endif /* defined(__Cellv0__Player__) */
