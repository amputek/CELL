#ifndef FeelerCreature_hpp
#define FeelerCreature_hpp

#include <stdio.h>
#include "Swimmer.hpp"
#include "Feeler.hpp"

using namespace std;


class FeelerCreature : public Swimmer, public IDrawable{
public:
    FeelerCreature(vec2 loc, float rad, float speed) : Swimmer(loc, rad, speed, true)
    { }
    
    
    ~FeelerCreature(){
        for( vector<Feeler*>::iterator p = mFeelers.begin(); p != mFeelers.end(); ++p){
            delete *p;
        }
    }
    
    vector<FeelerStruct> getDrawFeelers()
    {
        vector<FeelerStruct> dFeelers;
        for( Feeler * f : mFeelers )
            dFeelers.push_back( f->getDrawStruct() );
        return dFeelers;
    }
    
    void update()
    {
        for(int i = 0; i < mFeelers.size(); i++){
            mFeelers.at(i)->update();
            
            //random force added, which stops things being static - looks like the flow of underwater currents
            mFeelers.at(i)->addForce( vrand(20.0f) );
        }
        
        Swimmer::update();
    }
    
    //Default collider function
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
        mFeelersInContact = 0;
        
        for( vector<GameObject*>::iterator itCollider = gameObjects.begin(); itCollider < gameObjects.end(); ++itCollider )
        {
            GameObject * ptrCollider = *itCollider;
            if( ptrCollider == this ) continue;
            ICollideable * isCollider = dynamic_cast<ICollideable*>( ptrCollider );
            if( !isCollider ) continue;
            
            if( ptrCollider->mType == PLANKTON ) continue;
            
            for(int i = 0; i < mFeelers.size(); i++){
                if( mFeelers.at(i)->collide(ptrCollider->getPosition(), ptrCollider->getSize() * 4 ) )
                {
                    mFeelersInContact++;
                }
            }
        }
        
        Swimmer::collide(gameObjects, hero, environment, oscManager);
    }
    
    
    
protected:
    
    void addFeeler( int length, float baseWidth, float tipWidth, float stiffness = 3.0f )
    {
        mFeelers.push_back( new Feeler(mPosition, 1, length, baseWidth, tipWidth, stiffness));
    }
    
    void addFeelers( int count, int length, float baseWidth, float tipWidth, float stiffness = 3.0f )
    {
        for( int i = 0; i < count; i++)
        {
            mFeelers.push_back( new Feeler(mPosition, 1, length, baseWidth, tipWidth, stiffness));
        }
    }
    
    vector<Feeler*> mFeelers;
    int mFeelersInContact;
};



#endif /* FeelerCreature_hpp */
