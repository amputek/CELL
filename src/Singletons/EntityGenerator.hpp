#ifndef EntityGenerator_hpp
#define EntityGenerator_hpp

#include <stdio.h>

class EntityManager;

#include "Plankton.hpp"
#include "Egg.hpp"
#include "Feeler.hpp"
#include "Jelly.hpp"
#include "Spore.hpp"
#include "Urchin.hpp"
#include "Starfish.hpp"
#include "Friendly.hpp"
#include "Player.hpp"
#include "Spark.hpp"
#include "Renderer.hpp"


using namespace std;
using namespace ci;

class EntityGenerator{
public:
    EntityGenerator(){ };

    
    void generatePlankton( vector<GameObject*> & entities );
    void generatePlankton( vector<GameObject*> & entities, int planktonType, const vec2 & position );
    void generateStarfish( vector<GameObject*> & entities );
    void generateStarfish( vector<GameObject*> & entities, const vec2 & position );
    void generateUrchin( vector<GameObject*> & entities );
    void generateUrchin( vector<GameObject*> & entities, const vec2 & position );
    void generateSpores( vector<GameObject*> & entities );
    void generateSpores( vector<GameObject*> & entities, int sporeType, const vec2 & position );
    void generateJellyfish( vector<GameObject*> & entities );
    void generateJellyfish( vector<GameObject*> & entities, int jellyType, const vec2 & position );
    bool generateEgg( vector<GameObject*> & entities );
    bool generateEgg( vector<GameObject*> & entities, bool withFriendly, const vec2 & position );
    
    //Referenes to collections, player
    Player * hero;
    CellRenderer * image;
    
private:
    vec2 inFront(int inFrontBy, float randomiseAmount);
    float lineSegmentIntersection(const vec2 &start1, const vec2 &end1, const vec2 &start2, const vec2 &end2 );
//    
//    const int urchinFrequency = 1000;
//    const int sporeFrequency = 1400;
//    const int starFrequency = 1400;
//    const int jellyFrequency = 2000;
//    const int eggFrequency = 2000;
//    const int planktonFrequency = 100;
    


};

#endif /* EntityGenerator_hpp */
