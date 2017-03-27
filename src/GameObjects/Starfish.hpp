// Reacts to player proximity
// Changes colours and 'curls up'

#ifndef Starfish_hpp
#define Starfish_hpp

#include <iostream>
#include "FeelerCreature.hpp"

#include "OSCManager.hpp"
#include "EnvironmentManager.hpp"



class Starfish : public FeelerCreature{
public:

    Starfish(vec2 loc) : FeelerCreature(loc, randFloat(20,30), 0.5f )
    {
        GameObject::mType = STARFISH;
        
        for(int i = 0; i < 30; i++){
            int feelerLength = randInt(4,8);
            addFeeler( feelerLength, 20.0f, 0.3f );
        }
        
        ENTITY_COUNT++;

    }
    
    ~Starfish(){
        ENTITY_COUNT--;
    }
    
    void update()
    {
        
        mSpinCounter += 0.2f * deltaTime;
        if( mContactAmount > 750 ) mSpinCounter += (mContactAmount-750) * deltaTime * 0.005f;
        
        //reduce amount of contact
        if( mContactAmount > 0 ) mContactAmount -= deltaTime * 120.0f;
        
        if( mFleeing )
        {
            if( glm::distance( mTargetDestination, mPosition ) < 20.0f ) mFleeing = false;
        }
        else
        {
            setDestination( mPosition + vrand(15.0f) );
        }
        
        //update feelers
        int i = 0;
        for( auto f : mFeelers )
        {
            float angleToFeeler = (i / (float)mFeelers.size()) * 2.0f * M_PI;
            angleToFeeler += mSpinCounter;
            vec2 offsetPosition = vec2( sin(angleToFeeler), cos(angleToFeeler ) );
            f->setHomePosition( mPosition + offsetPosition * 8.0f );
            f->addForce( offsetPosition * (20.5f + sin(mSpinCounter + (angleToFeeler)) * 9.0f) );
            i++;
        }
        
        FeelerCreature::update();
    }
    
    
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        if( mFleeing ) return;
        
        vec2 heroPos = hero->getPosition();
        if( dist(heroPos, mPosition) > 200) return;
        
        
        vec2 d = heroPos - mPosition;
        float angleToHero = atan2(d.y, d.x);
        
        for(int i = 0; i < mFeelers.size(); i++){
            
            vec2 feelerToHero = mFeelers.at(i)->getPosition() - mPosition;
            float angleToFeeler = atan2(feelerToHero.y, feelerToHero.x);
            float angleDifference = atan2(sin(angleToFeeler-angleToHero), cos(angleToFeeler-angleToHero));
            
            if( abs(angleDifference) > M_PI * 0.2f ) continue;
            
            if( !mFeelers.at(i)->feelTowards( heroPos, 50.0f, 2.0f ) ) continue;
            
            mContactAmount += deltaTime * 65.0f;
            
            if(mContactAmount >= 1000){
                vec2 n = vrand(300.0f, 400.0f);
                setDestination( mPosition + n );
                mFleeing = true;
            }
        }
        
        
        if( mFleeing )
        {
            oscManager.changeChord();
            environment.splash( mPosition, 20.0f, 100.0f );
            environment.bubble( mPosition, 5 );
        }
        
        
    }
    

    void draw( CellRenderer & renderer ){
        renderer.drawStarfish( mPosition, getDrawFeelers(), mContactAmount );
        debugDraw( renderer );
    }
    
    
    
    bool isFleeing() const{ return mFleeing; }

    //getters
    static int SEENCOUNT;
    static int ENTITY_COUNT;
    const static int SPAWN_FREQUENCY = 1400;
    
private:
    float mSpinCounter = 0.0f;
    bool mFleeing = false;
    float mContactAmount = 0.0f;

};



#endif
