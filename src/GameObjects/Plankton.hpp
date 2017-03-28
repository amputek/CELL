#ifndef Plankton_hpp
#define Plankton_hpp

#include <iostream>
#include "GameObject.hpp"
#include "OSCManager.hpp"
#include "EnvironmentManager.hpp"
#include "Player.hpp"

using namespace ci;

class Plankton : public GameObject, public IDrawable, public ICollideable{
public:
    
    
    
    static int ENTITY_COUNT;
    const static int SPAWN_FREQUENCY = 100;
    const static int SPAWN_OFF_SCREEN_BY = 100;
    const static int DESPAWN_OFF_SCREEN_BY = 400;

    
    Plankton( vec2 loc, int t) : GameObject(loc, randFloat(0.9,1.1), randFloat(6,13) )
    {
        
        mPlanktonType = t;
        mRotation = randFloat(0, M_PI * 2);

        GameObject::mType = PLANKTON;
        GameObject::mDespawnOffScreenDist = DESPAWN_OFF_SCREEN_BY;
        
        
        ENTITY_COUNT++;
    }
    
    ~Plankton(){
        ENTITY_COUNT--;
    }
    
    void draw( CellRenderer & renderer )
    {
        renderer.drawPlankton(mPosition, mDepth, mRadius, mPlanktonType, mRotation );
    }
    
    void collide( vector<GameObject*> * gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        
        for( vector<GameObject*>::iterator itPredator = gameObjects->begin(); itPredator < gameObjects->end(); ++itPredator )
        {
            GameObject * ptrPredator = *itPredator;
            if( ptrPredator == this ) continue;            
            IPredates * isPred = dynamic_cast<IPredates*>( ptrPredator );
            if( !isPred ) continue;

            
            if( inContact( this, ptrPredator ) )
            {
                
                environment.bubble( mPosition, 3);
                environment.splash( mPosition, 1, 75 );
                oscManager.eatPlankton( mPlanktonType, (mPosition.x - hero->getPosition().x) , dist(hero->getPosition(), mPosition) );
                if( ptrPredator == hero )
                {
                    if( static_cast<Player*>(hero)->incEaten( mType )  )
                    {
                        environment.splash( hero->getPosition(), 0, 100 );
                        oscManager.eighthPlankton();
                    }
                }
                
                mDeleteMe = true;
                
            }
            
        }
    }

    
private:
    int mPlanktonType;
    float mRotation = 0.0f;
};

#endif

