#ifndef EntityGenerator_hpp
#define EntityGenerator_hpp

#include <stdio.h>

class EntityManager;

#include "../GameObjects/Plankton.hpp"
#include "../GameObjects/Egg.hpp"
#include "../GameObjects/Feeler.hpp"
#include "../GameObjects/Jelly.hpp"
#include "../GameObjects/Spore.hpp"
#include "../GameObjects/Urchin.hpp"
#include "../GameObjects/Starfish.hpp"
#include "../GameObjects/Friendly.hpp"
#include "../GameObjects/Player.hpp"
#include "../GameObjects/Spark.hpp"
#include "Renderer.hpp"


using namespace std;
using namespace ci;

class EntityGenerator{
public:
    EntityGenerator(){ }
    EntityGenerator( vector<GameObject*> * entities ) : entities(entities){ }

    
    bool generate()
    {
        Starfish::TIME_SINCE_ON_SCREEN++;
        Spore::TIME_SINCE_ON_SCREEN++;
        Jelly::TIME_SINCE_ON_SCREEN++;
        Urchin::TIME_SINCE_ON_SCREEN++;
        Egg::TIME_SINCE_ON_SCREEN++;
        
        generateBubbles();
        generateBeams();
        generatePlankton();
        generateStarfish();
        generateSpores();
        generateJellyfish();
        generateUrchin();
        if(generateEgg()) return true;
    
        return false;
    }
    
    //For debugging, allow entities to be generated in specified locations
    void generatePlankton( int planktonType, const vec2 & position );
    void generateStarfish( const vec2 & position );
    void generateUrchin( const vec2 & position );
    void generateSpores(  int sporeType, const vec2 & position );
    void generateJellyfish( int jellyType, const vec2 & position );
    bool generateEgg( bool withFriendly, const vec2 & position );
    
    //Referenes to collections, player
    Player * hero;
    CellRenderer * image;
    
private:
    vec2 inFront(int inFrontBy, float randomiseAmount);
    float lineSegmentIntersection(const vec2 &start1, const vec2 &end1, const vec2 &start2, const vec2 &end2 );

    //Reference to Entity Collection
    vector<GameObject*> * entities;
    
    void generatePlankton();
    void generateStarfish();
    void generateUrchin();
    void generateSpores();
    void generateJellyfish();
    bool generateEgg();

    void generateBubbles();
    void generateBeams();
    
};

#endif /* EntityGenerator_hpp */
