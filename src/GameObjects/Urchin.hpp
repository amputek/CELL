#ifndef Urchin_hpp
#define Urchin_hpp

#include <iostream>
#include "FeelerCreature.hpp"

#include "OSCManager.hpp"
#include "EnvironmentManager.hpp"

class Urchin : public FeelerCreature{
public:
    
    Urchin(vec2 loc) : FeelerCreature(loc, randFloat(2,35), 0.05f )
    {
        GameObject::mType = URCHIN;
        
        addFeelers(randInt(10,30), randInt(4,15), 6.0f, 0.2f);
        
        ENTITY_COUNT++;
    }
    
    ~Urchin(){
        ENTITY_COUNT--;
    }
    
    void update()
    {
        
        if( randFloat() < 0.001f )
        {
            setDestination( mPosition + vrand(400.0f) );
        }
        
        int i = 0;
        for( auto f : mFeelers )
        {
            //distribute feelers equally around the Urchin
            float angle = 2 * M_PI * i / mFeelers.size();
            f->setHomePosition( mPosition + vec2(sin(angle) * mRadius, cos(angle) * mRadius ) );
            f->addForce( (f->getPosition() - mPosition) * 1.0f );
            i++;
        }
        
        FeelerCreature::update();
    }
    
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        //Use default collision behaviour
        FeelerCreature::collide(gameObjects, hero, environment, oscManager);
        oscManager.urchin( dist( mPosition, hero->getPosition() ), mFeelersInContact );
    }
    
    void draw( CellRenderer & renderer )
    {
        renderer.drawUrchin(mPosition, mRadius, getDrawFeelers() );
        debugDraw( renderer );
    }

    static int SEENCOUNT;
    static int ENTITY_COUNT;
    const static int SPAWN_FREQUENCY = 1000;

};

#endif
