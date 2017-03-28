#ifndef Friendly_hpp
#define Friendly_hpp

#include <iostream>
#include "Swimmer.hpp"
#include "Tail.hpp"


class Friendly : public Swimmer, public IDrawable, public IPredates {
public:
    
    
    static int TIME_SINCE_SPAWN;
    static int ENTITY_COUNT;
    static const int DESPAWN_OFF_SCREEN_BY = 1200;

    
    Friendly(vec2 loc) : Swimmer(loc, 6.0f, randFloat(0.4f,0.6f) )
    {
        GameObject::mType = FRIENDLY;
        GameObject::mPermanent = false;
        GameObject::mDespawnOffScreenDist = DESPAWN_OFF_SCREEN_BY;
        
        tail = new Tail( 2, false, 0.6f, false );
        
        id = randInt(0, 1024);
        
        ENTITY_COUNT++;
    }
    
    ~Friendly()
    {
        ENTITY_COUNT--;
        delete tail;
    }
    
    
    void update()
    {
    
        Swimmer::update();
        
        //level up automatically, every 500 frames
        if(mBorn && mRadius < 20.0f ){
            levelCounter += deltaTime * 60.0f;
            if(levelCounter > 800.0f)
            {
                mRadius++;
                levelCounter = 0.0f;
                tail->incLength(1.0);
            }
        }
        
        tail->update(vec2(mPosition.x - sin(mDirection) * mRadius, mPosition.y - cos(mDirection) * mRadius), mDirection);
        
        if(mBorn && mBirthCounter < 40.0f)
        {
            mBirthCounter += deltaTime * 60.0f;
        }
    }
    
    void collide( vector<GameObject*> * gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        
        float distToHero = dist( hero->getPosition(), mPosition );
        
        if( mBorn )
        {
            Swimmer::collide( gameObjects, hero, environment, oscManager );
            
            
            int planktonCounter = 0;
            for( vector<GameObject*>::iterator itCollider = gameObjects->begin(); itCollider < gameObjects->end(); ++itCollider )
            {
                GameObject * ptrCollider = *itCollider;
                if( ptrCollider == this ) continue; //dont collide with yourself!
                ICollideable * isCollider = dynamic_cast<ICollideable*>( ptrCollider );
                if( !isCollider ) continue; //only collide with collideables
                if( ptrCollider->mType != PLANKTON ) continue;
                planktonCounter++;
                setDestination( ptrCollider->getPosition() );
            }
            
            if( distToHero < 1000 ){
                float pan = ( mPosition.x - hero->getPosition().x);
                oscManager.updateFriendly( id, pan, distToHero );
            }
            
        }
        else
        {
            

            if( distToHero < 40)
            {
                birth();
                oscManager.bornFriendly(id);
                setDestination( mPosition + vrand(500) );
            }
        }
        
    }

    void draw( CellRenderer & renderer )
    {
        renderer.drawFriendly( mPosition, mDirection, mRadius );
        if( mBorn ) tail->draw( renderer );
        
        debugDraw(renderer);
    }
    
    void birth(){
        if(!mBorn){
            mBorn = true;
            mBirthCounter = 0.0f;
            GameObject::mPermanent = true; //It becomes permanent
        }
    }
    

    
    //getters
    bool isBorn(){ return mBorn; };
    
    int id;
    
private:
    float levelCounter = 0.0f;
    Tail* tail;
    bool mBorn = false;
    bool mBirthing = false;
    float mBirthCounter = 0.0f;
};


#endif
