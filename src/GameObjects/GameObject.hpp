//
//  Highest Superclass for all game objects
//  Has a global location in gameworld, and local location on screen
//  Has a depth and a radius
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <iostream>
#include "Globals.hpp"
#include "Renderer.hpp"

class EnvironmentManager;
class OSCManager;

using namespace globals;
using namespace ci;

enum EntityType
{
    PLAYER,
    PLANKTON,
    JELLYFISH,
    STARFISH,
    URCHIN,
    FRIENDLY,
    SPARK,
    SPORE,
    EGG
};


enum SporeType
{
    ORANGE,
    PURPLE,
    GREEN
};


class GameObject
{
public:
    GameObject(vec2 startWorldPosition, float entityDepth, float entitySize, bool permanent = false )
        : mPosition(startWorldPosition), mDepth(entityDepth), mRadius(entitySize), mPermanent(permanent) { }
    
    virtual ~GameObject(){
        
    }

    float getSize() const { return mRadius; }
    float getDepth() const { return mDepth; }
    vec2 getPosition() const { return mPosition; }
    
    void offScreenBy( CellRenderer * renderer )
    {
        if( mPermanent ) return;
        if( !mDeleteMe ){
            mDeleteMe = !renderer->onScreen( mPosition, mDepth, mDespawnOffScreenDist );
        }
    }
    
    float mDespawnOffScreenDist = 400; //despawn off screen
    
    EntityType mType;
    
    bool mDeleteMe = false;
    
    static bool nearby( const GameObject * a, const GameObject * b, float d )
    {
        return dist( a->getPosition(), b->getPosition() ) < d;
    }
    
    static bool inContact( const GameObject * a, const GameObject * b )
    {
        return distance( a->getPosition(), b->getPosition() ) < a->getSize() + b->getSize();
    }
    
protected:
    float mRadius;
    float mDepth;
    vec2 mPosition;
    bool mPermanent = false;

};


class IDrawable{
public:
    virtual void draw( CellRenderer & renderer ) = 0;
};

class IDynamic{
public:
    virtual void update() = 0;
};

class IPredates
{
};

class ICollideable
{
public:
    virtual void collide( vector<GameObject*> * gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager ) = 0;
};


#endif
