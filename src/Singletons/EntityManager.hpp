#ifndef EntityManager_hpp
#define EntityManager_hpp

#include <iostream>
//#include "cinder/app/AppBasic.hpp"

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
#include "Grass.hpp"

// IO + Managers
#include "Images.hpp"
#include "OscManager.hpp"
#include "EntityGenerator.hpp"
#include "EnvironmentManager.hpp"


using namespace ci;
using namespace std;

class EntityManager {
    
public:
    EntityManager( Images* img );
    
	void update( );
    void updateHero( const vec2 & mouseLoc, bool canMove );
    void drawEntities();
    
    //pulsers (triggers sent from OSC)
    void pulse(string species, int index);
    void quit();
    
    EnvironmentManager* environment;
    
private:
    
    //private update functions
    void updateOffset();

    void updatePlankton();
    void updateUrchins();
    void updateSpores();
    void updateSparks();
    void updateEggs();
    void updateStarfish();
    void updateJellies();
    void updateFriendlies();
    void updateGrass();
    void drawGrass();
    void drawBackground();
    
    
    //General purpose Functions
    void removeFromColliders(GameObject* collider);
    bool farFromHero( const vec2 & location );

    //Collections
    Player* hero;
    vector<Friendly*> friendlies;
    vector<Grass*> longGrass;
    vector<Plankton*> plankton;
    vector<Jelly*> jellies;
    vector<Egg*> eggs;
    vector<Spore*> spores;
    vector<Urchin*> urchins;
    vector<Starfish*> starfish;
    vector<Spark*> sparks;
    
    //colliders can eat plankton, collide with jellyfish and urchins
    vector<GameObject*> colliders;

    
    //game values + counters
    bool insideEgg;
    
    //Managers etc
    OSCManager* oscManager;

    Images* image;
    EntityGenerator * entityGenerator;
};

#endif
