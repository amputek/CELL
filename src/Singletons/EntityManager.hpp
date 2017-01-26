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

// IO + Managers
#include "Images.hpp"
#include "OscManager.hpp"
#include "EnvironmentManager.hpp"


using namespace ci;
using namespace std;

class EntityManager {
    
public:
    EntityManager( Images* img );
    
	void update();
    void updateHero(vec2 mouseLoc);
    void drawEntities();
    
    //pulsers (triggers sent from OSC)
    void pulse(string species, int index);
    void create(string species);
    void quit();
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
    
    //PCG
    void entityGenerator();
    
    
    //General purpose Functions
    void removeFromColliders(GameObject* collider);
    vec2 inFront(vec2 start, float diretion, int inFrontBy);
    bool farFromHero( vec2 location );
    
    
    //Collections
    Player* hero;
    vector<Friendly*> friendlies;
    vector<Feeler*> longGrass;
    vector<Plankton*> plankton;
    vector<Jelly*> jellies;
    vector<Egg*> eggs;
    vector<Spore*> spores;
    vector<Urchin*> urchins;
    vector<Starfish*> starfish;
    vector<Spark*> sparks;
    vector<GameObject*> colliders;

    
    //game values + counters
    int urchinLastSeen;
    int sporeLastSeen;
    int starLastSeen;
    int jellyLastSeen;
    int eggLastSeen;
    bool aboveSurface;
    bool insideEgg;
    
    //Managers etc
    OSCManager* oscManager;
    EnvironmentManager* environment;
    Images* image;
};

#endif
