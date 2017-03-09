#ifndef Swimmer_hpp
#define Swimmer_hpp

#include "Braitenberg.hpp"
#include "Feeler.hpp"
#include "Renderer.hpp"

using namespace std;

class Swimmer : public Braitenberg{
    
public:
    Swimmer(vec2 loc, float rad, float speed);
    Swimmer(vec2 loc, float rad, float speed, bool canSlow );
    
    void update();
    void avoidColliders(vector<GameObject*> * movers);

    
    bool inSpace;
    
    
private:
    bool checkInSpace(vec2 loc, vector<GameObject*> * movers);


};




class FeelerCreature : public Swimmer{
public:
    FeelerCreature(vec2 loc);
    
    ~FeelerCreature(){
        cout << "Destructing feeler creature!" << "\n";
        for( vector<Feeler*>::iterator p = mFeelers.begin(); p != mFeelers.end(); ++p){
            delete *p;
        }
    }
    
    void update();
    virtual void draw( CellRenderer & renderer );
    virtual void collide(const vec2 & loc, float colliderSize)
    {
        if( dist(loc,mPosition) > 200) return;
        
        for(int i = 0; i < mFeelers.size(); i++){
            mFeelers.at(i)->collide( loc, colliderSize );
        }
    }
    
    int getContactAmount() const {
        return mContactCount;
    };
    
private:
    vector<Feeler*> mFeelers;
    int mContactCount;
};



#endif
