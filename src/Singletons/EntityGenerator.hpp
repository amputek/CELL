#ifndef EntityGenerator_hpp
#define EntityGenerator_hpp

#include <stdio.h>

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
#include "Grass.hpp"
#include "Images.hpp"


using namespace std;
using namespace ci;

class EntityGenerator{
public:
    EntityGenerator(){ };

    
    void generatePlankton( vector<Plankton*> * plankton, vector<Egg*> * eggs );
    void generatePlankton( vector<Plankton*> * plankton, int planktonType, const vec2 & position );
    void generateStarfish( vector<Starfish*> * starfish );
    void generateStarfish( vector<Starfish*> * starfish, const vec2 & position );
    void generateUrchin( vector<Urchin*> * urchins );
    void generateUrchin( vector<Urchin*> * urchins, const vec2 & position );
    void generateSpores( vector<Spore*> * spores );
    void generateSpores( vector<Spore*> * spores, int sporeType, const vec2 & position );
    void generateJellyfish( vector<Jelly*> * jellies );
    void generateJellyfish( vector<Jelly*> * jellies, int jellyType, const vec2 & position );
    bool generateEgg( vector<Egg*> * eggs, vector<Friendly*> * friendlies, vector<Plankton*> * plankton);
    bool generateEgg( vector<Egg*> * eggs, vector<Friendly*> * friendlies, vector<Plankton*> * plankton, bool withFriendly, const vec2 & position );
    
    //Referenes to collections, player
    Player * hero;
    vector<GameObject*> * colliders;
    Images * image;
    
    
    //Counters
    int urchinLastSeen;
    int sporeLastSeen;
    int starLastSeen;
    int jellyLastSeen;
    int eggLastSeen;
    
    
    
private:
    vec2 inFront(int inFrontBy);
    float lineSegmentIntersection(const vec2 &start1, const vec2 &end1, const vec2 &start2, const vec2 &end2 );
    
    const int urchinFrequency = 500;
    const int sporeFrequency = 700;
    const int starFrequency = 1200;
    const int jellyFrequency = 1000;
    const int eggFrequency = 800;
    const int planktonFrequency = 80;

};

#endif /* EntityGenerator_hpp */
