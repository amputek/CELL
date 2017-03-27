#ifndef Spark_hpp
#define Spark_hpp

#include "Swimmer.hpp"
#include "Finite.hpp"


class Spark : public Swimmer, public IDrawable {
public:
    Spark(const vec2 & loc, SporeType sporeType ) : Swimmer(loc, 8.0f, 0.8f)
    {
        GameObject::mType = EntityType::SPARK;
        GameObject::mPermanent = true;
        
        mSporeType = sporeType;
        
        ignore( SPORE );
    
        ENTITY_COUNT++;
        
        cout << "NEW SPARK: " << mPosition << " " << mType << endl;
    }
    
    ~Spark()
    {
        for( vector<Finite*>::iterator p = mFinites.begin(); p != mFinites.end(); ++p){
            delete *p;
        }
        ENTITY_COUNT--;
    }
    
    void update()
    {
        

        life += deltaTime * 60.0f;
        
        //radius returns to 8 after pulse (from SuperCollider)
        if(mRadius > 8.0f){
            mRadius -= deltaTime * 25.0f;
        }
        
        Swimmer::update();
        
        
        //Spark is a collection of "Finites" that reduce in size
        
        if( life > 1.0f )
        {
            mFinites.push_back( new Finite(mPosition, 12.0f, mRadius * 0.8f) );
            life = 0.0f;
        }
        
        for( vector<Finite*>::iterator p = mFinites.begin(); p != mFinites.end(); ){
            (*p)->update();
            if((*p)->alive() == false){
                delete *p;
                p = mFinites.erase(p);
            } else {
                ++p;
            }
        }
        
    }
    
    void collide( vector<GameObject*> & gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager )
    {
   
        Swimmer::collide( gameObjects, hero, environment, oscManager );
        
        for( vector<GameObject*>::iterator itCollider = gameObjects.begin(); itCollider < gameObjects.end(); ++itCollider )
        {
            GameObject * ptrCollider = *itCollider;
            if( ptrCollider == this ) continue; //dont collide with yourself!
            ICollideable * isCollider = dynamic_cast<ICollideable*>( ptrCollider );
            if( !isCollider ) continue; //only collide with collideables
            
            if( dist( mPosition, ptrCollider->getPosition()) > 100) continue;
            
            if( ptrCollider->mType == URCHIN ) setDestination(ptrCollider->getPosition());
            if( ptrCollider->mType == JELLYFISH) setDestination(ptrCollider->getPosition() + vec2(0,randFloat(20,170)));
        }
        
        
        if( mInSpace ){
            if( dist(mPosition, hero->getPosition()) > 50){
                setDestination( hero->getPosition() );
            }
        }
        
        //sparks constantly create splashes
        environment.splash( mPosition, mRadius, 10 );

    }
    
    void draw( CellRenderer & renderer )
    {
        vector<vec2> positions;
        vector<float> sizes;
        
        for( auto f : mFinites )
        {
            positions.push_back( f->getPosition() ) ;
            sizes.push_back( (1.0 - (f->lifeRatio()) * 0.5f) * (f->getSize() ) );
        }
        
        renderer.drawSpark(mPosition, mRadius, mSporeType, positions, sizes);
        debugDraw( renderer );
    }
    
    void pulse()
    {
        mRadius = 24.0f;
    }
    
    static int ENTITY_COUNT;

private:
    vector<Finite*> mFinites;    //collection of positions to draw circles
    SporeType mSporeType;
    
    float life = 0.0f;
};

#endif
