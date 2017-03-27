#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include "Braitenberg.hpp"
#include "Tail.hpp"

using namespace ci::app;

class Player : public Braitenberg, public IDrawable, public IPredates, public ICollideable{
public:
    Player(const vec2 & loc );
    void draw( CellRenderer & renderer );
    void update();
    bool incEaten(int type){
        planktonEaten++;
        if( planktonEaten % 20 == 0 )
        {
            levelUp();
            return true;
        }
        return false;
    };
    void slow(bool t);
    
    //getters
    bool levelling(){ return playerLevelling; };

    
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager ){ }

    vec2 getDestination() const
    {
        return Braitenberg::mTargetDestination;
    }
    
private:
    
    void levelUp();
    
    int bodyType = 0;
    int levelCount;             //a counter for the levelling process
    Tail* longTail;
    Tail* rightTail;
    Tail* leftTail;
    bool playerLevelling;       //true if player is levelling
    bool sideTailsOn = false;    //the shorter side tails that the player has at a high enough level
    int planktonEaten;  //number of each type of plankton eaten
    int level;                  //current level
};



#endif
