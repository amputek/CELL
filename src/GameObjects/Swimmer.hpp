#ifndef Swimmer_hpp
#define Swimmer_hpp

#include "Braitenberg.hpp"

using namespace std;

class Swimmer : public Braitenberg{
    
public:
    Swimmer(vec2 loc);
    Swimmer(vec2 loc, bool canSlow);
    
    void update();
    void avoidColliders(vector<GameObject*> * movers);
    void setDestination(vec2 dest){
        targetDestination = dest;
    };
    
    bool inSpace;
    
    vec2 targetDestination;
    

    
private:
    bool checkInSpace(vec2 loc, vector<GameObject*> * movers);


};



#endif
