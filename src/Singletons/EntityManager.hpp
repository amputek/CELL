#ifndef EntityManager_hpp
#define EntityManager_hpp

#include <iostream>

//ENTITIIES
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

// IO + Managers
#include "OscManager.hpp"
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
    
	void update( );
    void updateHero( const vec2 & mouseLoc, bool canMove );
    void drawEntities();
    void printEntityStats();
    void quit();
    
    vector<PulseEvent> * pulseEvents;
    
    EnvironmentManager environment;
    
private:
    
    //private update/draw functions
    void updateOffset();
    
    //Reference to player gameObject
    Player * hero;

    //Entity collection -- its a pointer so other subsystems can reference it
    vector<GameObject*> * gameObjects;
    
    //Managers etc
    OSCManager * oscManager;
    CellRenderer * renderer;
    
    //Subsystems
    EntityGenerator entityGenerator;

};

#endif
