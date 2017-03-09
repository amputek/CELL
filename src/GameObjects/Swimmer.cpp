#include "Swimmer.hpp"

Swimmer :: Swimmer(vec2 loc, float rad, float speed) : Braitenberg(loc, false, rad, speed){
    targetDestination = loc;
    inSpace = true;
}

Swimmer :: Swimmer(vec2 loc, float rad, float speed, bool canSlow) : Braitenberg(loc, canSlow, rad, speed){
    targetDestination = loc;
    inSpace = true;
}


//The Swimmer will constantly move towards it's target
void Swimmer :: update(){
    //swimmer moves to target
    moveTo(targetDestination);
}

//AI to avoid enemies and move towards a target
void Swimmer :: avoidColliders( vector<GameObject*> * movers ) {
    bool inSpaceTemp = true;
    bool targetFound = false;

    for(int i = 0; i < movers->size(); i++ ) {
        if(inSpaceTemp == true){
            //distance between swimmer and another mover
            vec2 glo = movers->at(i)->getPosition() - mPosition;
            
            //make sure it's not targetting itself
            if ( length(glo) > 0) {
                
                //if collision is happening
                if( length(glo) < (mRadius + movers->at(i)->getSize()) * 2) {
                    vec2 newLoc = mPosition - glo;
                    //iterations means AI will not get stuck in a loop, trying to find a new position for too long
                    int iterations = 0;
                    while(targetFound == false && iterations < 3){
                        if(checkInSpace(newLoc, movers) == true){
                            targetFound = true;
                            setDestination( newLoc );
                        } else {
                            newLoc += vec2(randFloat(-200,200),randFloat(-200,200));
                            iterations++;
                        }
                    }
                    if(inSpaceTemp == true){
                        inSpaceTemp = false;
                    }
                }
            }
        }

    }
    inSpace = inSpaceTemp;
}


//checks if a particular location is in the vicinity of any entities
bool Swimmer :: checkInSpace(vec2 loc, vector<GameObject*> * movers) {
    for (int i = 0; i < movers->size(); i++) {
        GameObject * m = movers->at(i);
        if (m->getPosition() != mPosition) {
            if ( length(m->getPosition() - loc) < 40) {
                return false;
            }
        }
    }
    return true;
}
