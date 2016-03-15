#include "Swimmer.hpp"

Swimmer :: Swimmer(vec2 loc) : Braitenberg(loc, false){
    targetLocation = loc;
    targetDestination = loc;
    inSpace = true;
}

//The Swimmer will constantly move towards it's target
void Swimmer :: update(){
    
    //target location eases around...
    if( dist(targetLocation, targetDestination) > 0.2){
        targetLocation += (targetDestination - targetLocation) * 0.2f;
    }
    
    //swimmer moves to target
    moveTo(targetLocation);
    Braitenberg::update();
}

//AI to avoid enemies and move towards a target
void Swimmer :: updateTarget(vector<GameObject*> movers) {
    bool inSpaceTemp = true;
    bool targetFound = false;
    bool debug = true;
    if(debug == true){
        for(int i = 0; i < movers.size(); i++ ) {
            if(inSpaceTemp == true){
                //distance between swimmer and another mover
                vec2 glo = movers.at(i)->global - global;
                
                //make sure it's not targetting itself
                if ( length(glo) > 0) {
                    
                    //if collision is happening
                    if( length(glo) < (radius + movers.at(i)->radius) * 2) {
                        vec2 newLoc = global - glo;
                        //iterations means AI will not get stuck in a loop, trying to find a new position for too long
                        int iterations = 0;
                        while(targetFound == false && iterations < 3){
                            if(checkInSpace(newLoc, movers) == true){
                                targetFound = true;
                                targetDestination = newLoc;
                            } else {
                                newLoc += vec2(rand(-200,200),rand(-200,200));
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
    }
    inSpace = inSpaceTemp;
}


//checks if a particular location is in the vicinity of any entities
bool Swimmer :: checkInSpace(vec2 loc, vector<GameObject*> movers) {
    for (int i = 0; i < movers.size(); i++) {
        GameObject m = *movers.at(i);
        if (m.global != global) {
            if ( length(m.global - loc) < 40) {
                return false;
            }
        }
    }
    return true;
}