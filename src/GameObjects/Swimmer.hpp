#ifndef Swimmer_hpp
#define Swimmer_hpp

#include "Braitenberg.hpp"
#include "Feeler.hpp"

using namespace std;

//Autonomous Braitenberg
class Swimmer : public Braitenberg, public ICollideable {
    
public:
    Swimmer(vec2 loc, float rad, float speed);
    Swimmer(vec2 loc, float rad, float speed, bool canSlow );
    

    void avoidColliders( const vector<GameObject*> & gameObjects );

    
    void collide( vector<GameObject*> * gameObjects, GameObject * hero, EnvironmentManager & environment, OSCManager & oscManager );
    
protected:

    bool mInSpace = true;
    
    void ignore( EntityType type )
    {
        mIgnoreTypes.push_back(type);
    }
    
    bool shouldIgnore( GameObject * entity )
    {
        return find(mIgnoreTypes.begin(), mIgnoreTypes.end(), entity->mType) != mIgnoreTypes.end();
    }
    
    
private:
    bool checkInSpace(const vec2 & loc, const vector<GameObject*> * movers);
    vector<EntityType> mIgnoreTypes;


};




#endif
