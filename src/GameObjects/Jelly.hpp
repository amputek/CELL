#ifndef Jelly_hpp
#define Jelly_hpp

#include <iostream>
#include "FeelerCreature.hpp"

class Jelly : public FeelerCreature{
public:
    
    
    Jelly(vec2 loc, int type) : FeelerCreature(loc, randFloat(18,40), 0.04f ){
        
        
        GameObject::mType = JELLYFISH;
        
        jellyType = type;
        
        
        //3 different types of Jellyfish
        if(type == 0){ feelerStartRadius = mRadius * 0.25f; }
        if(type == 1){ feelerStartRadius = mRadius * 0.6f; }
        if(type == 2){ feelerStartRadius = mRadius * 0.6f; }
        
        //add a few feelers
        for(int i = 0; i < randInt(5,10); i++)
        {
            addFeeler( randInt(10,30), randFloat(3.0f, 6.0f), 0.2f );
        }
        
        ENTITY_COUNT++;
    }
    
    ~Jelly(){
        ENTITY_COUNT--;
    }
    


    void update(){
        
        FeelerCreature::update();
        
        counter += deltaTime * 7.0f;
        
        for(int i = 0; i < mFeelers.size(); i++){
            mFeelers.at(i)->addForce( vec2(randFloat(-15,15), randFloat(18,34)) );
            float pos = (0.5*M_PI * i / (mFeelers.size()-1)) - 0.25*M_PI;
            mFeelers.at(i)->setHomePosition( mPosition + vec2(sin(pos) * feelerStartRadius, cos(pos) * feelerStartRadius ) );// + vec2(0, cos(counter)*3) * 1.0f;
        }
    }

    void draw( CellRenderer & renderer ){
        renderer.drawJellyfish( mPosition, mRadius, getDrawFeelers(), jellyType, counter, 4.0f );
        debugDraw( renderer );        
    }
    
    
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        FeelerCreature::collide( gameObjects, hero, environment, oscManager );
        oscManager.jelly( mFeelersInContact, dist( mPosition, hero->getPosition() ) );
    }
    
    
    static int SEENCOUNT;
    static int ENTITY_COUNT;
    const static int SPAWN_FREQUENCY = 400;

private:
    float counter = 0.0f;
    float feelerStartRadius = 0.5f; //modifies the radius of the feeler positions (depends on the png being used)
    int jellyType = 0;

};

#endif
