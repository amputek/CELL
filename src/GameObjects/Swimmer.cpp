#include "Swimmer.hpp"

Swimmer :: Swimmer(vec2 loc, float rad, float speed) : Braitenberg(loc, false, rad, speed){ }

Swimmer :: Swimmer(vec2 loc, float rad, float speed, bool canSlow) : Braitenberg(loc, canSlow, rad, speed){ }


//AI to avoid enemies and move towards a target
void Swimmer :: collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
{
    bool inSpaceTemp = true;
    bool targetFound = false;

    for( auto mover : gameObjects )
    {
        if( mover == this ) continue;
        if( !dynamic_cast<ICollideable*>( mover ) ) continue;
        if( find(mIgnoreTypes.begin(), mIgnoreTypes.end(), mover->mType) != mIgnoreTypes.end()) continue;
        
        if(inSpaceTemp){
            //distance between swimmer and another mover
            vec2 glo = mover->getPosition() - mPosition;
            
            //make sure it's not targetting itself
            if ( length(glo) > 0) {
                
                //if collision is happening
                if( length(glo) < (mRadius + mover->getSize()) * 2) {
                    vec2 newLoc = mPosition - glo;
                    //iterations means AI will not get stuck in a loop, trying to find a new position for too long
                    int iterations = 0;
                    while(targetFound == false && iterations < 3){
                        if(checkInSpace(newLoc, gameObjects) == true){
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
    mInSpace = inSpaceTemp;
}


//checks if a particular location is in the vicinity of any entities
bool Swimmer :: checkInSpace(const vec2 & newPosition, const vector<GameObject*> & movers) {
    for (int i = 0; i < movers.size(); i++) {
        GameObject * m = movers.at(i);
        if (m->getPosition() != mPosition) {
            if ( length(m->getPosition() - newPosition) < 40) {
                return false;
            }
        }
    }
    return true;
}
