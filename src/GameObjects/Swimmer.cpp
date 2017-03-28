#include "Swimmer.hpp"

Swimmer :: Swimmer(vec2 loc, float rad, float speed) : Braitenberg(loc, false, rad, speed){ }

Swimmer :: Swimmer(vec2 loc, float rad, float speed, bool canSlow) : Braitenberg(loc, canSlow, rad, speed){ }


//AI to avoid enemies and move towards a target
void Swimmer :: collide( vector<GameObject*> * gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
{
    bool inSpaceTemp = true;
    bool targetFound = false;

    for( auto mover : *gameObjects )
    {
        if( mover == this ) continue;
        if( !dynamic_cast<ICollideable*>( mover ) ) continue;
        if( find(mIgnoreTypes.begin(), mIgnoreTypes.end(), mover->mType) != mIgnoreTypes.end()) continue;
        
        if(inSpaceTemp){
            
            //distance between swimmer and another mover
            vec2 toCollider = mover->getPosition() - mPosition;

            
            float minDist = (mRadius + mover->getSize()) * 2.0f;
            
            if( length(toCollider) > minDist ) continue;
            
            vec2 newTargetDestination =  mPosition - glm::normalize(toCollider) * minDist;
            
            //iterations means AI will not get stuck in a loop, trying to find a new position for too long
            int iterations = 0;
            
            while(targetFound == false && iterations < 3){
                if(checkInSpace(newTargetDestination, gameObjects) == true){
                    targetFound = true;
                    setDestination( newTargetDestination );
                } else {
                    newTargetDestination += vec2(randFloat(-200,200),randFloat(-200,200));
                    iterations++;
                }
            }
            
            if(inSpaceTemp){
                inSpaceTemp = false;
            }
        }
    }
    
    mInSpace = inSpaceTemp;
}


//checks if a particular location is in the vicinity of any entities
bool Swimmer :: checkInSpace(const vec2 & newPosition, const vector<GameObject*> * entities)
{
    
    for( auto mover : *entities )
    {
        if( mover == this ) continue;
        if( !dynamic_cast<ICollideable*>( mover ) ) continue;
        
        if (mover->getPosition() != mPosition) {
            if ( length(mover->getPosition() - newPosition) < 40) {
                return false;
            }
        }
    }
    
    return true;
}
