#ifndef Swimmer_hpp
#define Swimmer_hpp

#include "Braitenberg.hpp"

using namespace std;

class Swimmer : public Braitenberg{
    
public:
    Swimmer(vec2 loc, float rad, float speed);
    Swimmer(vec2 loc, bool canSlow, float rad, float speed );
    
    void update();
    void avoidColliders(vector<GameObject*> * movers);

    
    bool inSpace;
    
    
private:
    bool checkInSpace(vec2 loc, vector<GameObject*> * movers);


};



#endif
