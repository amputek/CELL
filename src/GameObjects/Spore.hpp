//Spores are circular egg-like objects that react to collisions
//After certain number of collisions, spores will hatch eggs
//

#ifndef Spore_hpp
#define Spore_hpp

#include <iostream>
#include "PhysicsObject.hpp"

#include "OSCManager.hpp"
#include "EnvironmentManager.hpp"

#include "Spark.hpp"

using namespace std;

class Spore : public PhysicsObject, public IDrawable, public ICollideable{
public:
    Spore(vec2 loc, float depth, SporeType type) : PhysicsObject(loc, depth, randFloat(20,40) * depth, 4.0f ){
        mHealth = randInt(4,8);
        mSporeType = type;
        mType = EntityType::SPORE;
        ENTITY_COUNT++;
    }
    
    ~Spore()
    {
        ENTITY_COUNT--;
    }

    void update(){
        
        if(mHealth <= 1){
            mPosition += vec2(randFloat(-2,2), randFloat(-2,2));
        }
        PhysicsObject::update();
    }
    

    
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        
        if( abs( mDepth - 1 ) > 0.2f ) return;
        
        if( distance(mPosition, hero->getPosition()) < ( getSize() * 0.75 + 10 )){
            
            
            environment.splash( mPosition, 22 * mDepth, 30 );

            
            addVelocity( (mPosition - hero->getPosition()) * 6.0f );
            mHealth -= 1;
            oscManager.sporeBoop( mHealth );
            //cout << "BOOPIN " << mHealth << endl;
    
            if( mHealth <= 0 )
            {
                mDeleteMe = true;
                environment.bubble( mPosition, 6 );
                environment.splash( mPosition + vrand(10), mRadius, mRadius * 2.5f );
               // if( sparksSpawned == 0 || randFloat() < 0.5f )
               // {
                //    gameObjects.push_back(new Spark(mPosition, mSporeType));
                //    oscManager.newSpark( mSporeType );
                //    sparksSpawned++;
                //}
                
                gameObjects.push_back(new Spark(mPosition, mSporeType));
                oscManager.newSpark( mSporeType );
                sparksSpawned++;
                
            }
            
        }
        
    }
    
    void draw( CellRenderer & renderer )
    {
        renderer.drawSpore( mPosition, mRadius, mDepth, mSporeType );
    }
    

    
    static int SEENCOUNT;
    static int ENTITY_COUNT;
    static int sparksSpawned;
    const static int SPAWN_FREQUENCY = 1400;
    
private:
    int mHealth;
    SporeType mSporeType;      //three different types of Spore
};



#endif
