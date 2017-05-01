#ifndef EntityManager_hpp
#define EntityManager_hpp

#include <iostream>

//ENTITIIES
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

// IO + Managers
#include "OSCManager.hpp"
#include "EntityGenerator.hpp"
#include "EnvironmentManager.hpp"


using namespace ci;
using namespace std;

struct PulseEvent
{
    PulseEvent( EntityType type, int index ) : entityType(type), index(index){ }
    EntityType entityType;
    int index;
};

class EntityManager {
    
public:
    EntityManager(){ }
    EntityManager( CellRenderer * img );
    
    void init();
    
	void update( );
    void updateHero( const vec2 & mouseLoc, bool canMove );
    void drawEntities();
    void printEntityStats();
    void quit();
    
    vector<PulseEvent> pulseEvents;
    
    EnvironmentManager environment;
    
private:
    
    //private update/draw functions
    void updateOffset();
    
    //Reference to player gameObject
    Player * hero;

    //Entity collection -- its a pointer so other subsystems can reference it
    vector<GameObject*> gameObjects;
    
    //Managers etc
    OSCManager oscManager;
    CellRenderer * renderer;
    
    //Subsystems
    EntityGenerator entityGenerator;

};

#endif
