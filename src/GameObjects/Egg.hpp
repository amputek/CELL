#ifndef Egg_hpp
#define Egg_hpp

#include <iostream>
#include "SpringyObject.hpp"
#include "Player.hpp"
#include "Friendly.hpp"
#include "OSCManager.hpp"
#include "EnvironmentManager.hpp"


class Egg : public GameObject, public IDynamic, public IDrawable, public ICollideable{
public:
    
    static int TIME_SINCE_ON_SCREEN;
    static int ENTITY_COUNT;
    const static int SPAWN_FREQUENCY = 2000;
    const static int SPAWN_OFF_SCREEN_BY = 800;
    const static int DESPAWN_OFF_SCREEN_BY = 1200;
    
    
    Egg(vec2 loc ) : GameObject(loc, 1, 160)
    {
        mDrawSize = mRadius * 2.4;
        
        float damping = 1.2f;
        float stiffness = 5.0f;
        float mass = 10.0f;
        
        springs2 = new SpringyObject[numSprings];
     
        //create new springs with damp, stiffness, mass parameters
        for(int i = 0; i < numSprings; i++){
            float pos =  2 * M_PI * i / numSprings;
            springs2[i] = SpringyObject( loc + vec2(sin(pos) * mRadius, cos(pos) * mRadius ) , 1, stiffness, mass, damping );
        }
        
        //update a few times upon arrival.... shouldn't need to do this I think?
        for(int i = 0; i < numSprings; i++){
            update();
        }
        
        
        GameObject::mType = EGG;
        GameObject::mPermanent = false;
        GameObject::mDespawnOffScreenDist = DESPAWN_OFF_SCREEN_BY;

        
        ENTITY_COUNT++;
        TIME_SINCE_ON_SCREEN = 0;
    }
    
    ~Egg(){
        delete[] springs2;
        ENTITY_COUNT--;
    }
    
    
    
    
    void update()
    {
        

        mPulseCounter += deltaTime * 3.0f;
        
        for(int i = 0; i < numSprings; i++){
            if( randFloat() < 0.2f)
            {
                float pos =  2 * M_PI * i / numSprings;
                springs2[i].moveTowards( mPosition + vec2(sin(pos) * getSize(), cos(pos) * getSize() ) );
            }
            springs2[i].update();
        }
        
        
        
        
        
        //update each spring with it's neighbouring springs
        int j = 0;
        for(int i = randInt( numSprings-1 ); j < numSprings; i++, j++){
            int t = i % numSprings;
            int t2 = (i+1) % numSprings;
            springs2[t].moveTowards( springs2[t2].getPosition() );
        }
        
        j = 0;
        for(int i = numSprings + randInt( numSprings-1); j < numSprings; i--, j++){
            int t = i % numSprings;
            int t2 = (i+1) % numSprings;
            springs2[t2].moveTowards( springs2[t].getPosition() );
        }
        
    }

    
    void collide( vector<GameObject*> * gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        
        for( vector<GameObject*>::iterator itCollider = gameObjects->begin(); itCollider < gameObjects->end(); ++itCollider )
        {
            GameObject * ptrCollider = *itCollider;
            if( ptrCollider == this ) continue;
            ICollideable * isCollider = dynamic_cast<ICollideable*>( ptrCollider );
            if( !isCollider ) continue;
            
            
            
            if( ptrCollider->mType == PLANKTON )
            {
                if( nearby(ptrCollider, this, mRadius) ) ptrCollider->mDeleteMe = true;
                continue;
            }
            
            if( ptrCollider->mType == PLAYER || ptrCollider->mType == FRIENDLY || ptrCollider->mType == SPARK )
            {            
                for(int i = 0; i < numSprings; i++)
                {
                    springs2[i].collide( ptrCollider->getPosition(), ptrCollider->getSize() * 3 );
                }
            }
        }
        
        
        //Check if player is inside
        
        vec2 loc = hero->getPosition();
        bool newInside = dist(loc, mPosition) < mRadius;
        
        if(newInside)
        {
            if(!mPlayerInside)
            {
                mPlayerInside = true;
                oscManager.egg(1);
                static_cast<Player*>(hero)->slow(true);
            }
        } else {
            if(mPlayerInside)
            {
                mPlayerInside = false;
                oscManager.egg(0);
                static_cast<Player*>(hero)->slow(false);
            }
        }

        
    }
    
    void draw( CellRenderer & renderer )
    {
        
        vector<vec2> pos;
        
        for( int n = 0; n < numSprings; n++)
            pos.push_back( springs2[n].getPosition() );
        
        bool drawn = renderer.drawEgg( mPosition, mDrawSize, mPulseCounter, pos );
        if( drawn ) TIME_SINCE_ON_SCREEN = 0;
    }

    
    //getters
    bool playerIsInside() const {
        return mPlayerInside;
    };
    


private:
    
    //vector<SpringyObject> springs;;
    int mDrawSize;          //constant value - ratio between png and shape size
    bool mPlayerInside = false;           //allows EntityManager to check if player is inside
    float mPulseCounter = 0.0f;
    static const int numSprings = 80;
    SpringyObject * springs2;
};

#endif
