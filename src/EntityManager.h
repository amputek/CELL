//
//  EntityManager.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__EntityManager__
#define __Cellv0__EntityManager__

#include <iostream>
#include "cinder/app/AppBasic.h"

//ENTITIIES
#include "Plankton.h"
#include "Egg.h"
#include "Feeler.h"
#include "Jelly.h"
#include "Spore.h"
#include "Urchin.h"
#include "Starfish.h"
#include "Friendly.h"
#include "Player.h"
#include "Spark.h"

// IO + Managers
#include "Images.h"
#include "OscManager.h"
#include "EnvironmentManager.h"


using namespace ci::app;
using namespace std;

class EntityManager {
    
public:
    EntityManager( Images* img );
    
	void update();
    void updateHero(Vec2f mouseLoc);
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
    
    void entityGenerator();
    
    
    //General purpose Functions
    void removeFromColliders(GameObject* collider);
    Vec2f inFront(Vec2f start, float diretion, int inFrontBy);
    bool farFromHero( Vec2f location );
    
    
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

    //environment
    const int surfaceY = -7000;
    
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

#endif /* defined(__Cellv0__EntityManager__) */
