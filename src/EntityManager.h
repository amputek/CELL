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

// IO
#include "Images.h"
#include "OscManager.h"
#include "EnvironmentManager.h"


using namespace ci::app;
using namespace std;

class EntityManager {
    
public:
    EntityManager( OSCManager* osc );
    
	void update();
    void updateHero(Vec2f mouseLoc);

    void drawEntities();
    
    //getters
    float getY(){ return hero->global.y; };
    float getX(){ return hero->global.x; };
    float getDepth(){ return depth; };
    
    //pulsers (triggers sent from OSC)
    void pulseFriendly(int index);
    void pulseUrchin(int index);
    void pulseSpark(int index);
    
    void createSpore();
    void createStarfish();
    
private:
    Images* image;
    void updateOffset();
    
    void updateEntities();
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
    
    
    int distribute();
    
    void removeFromColliders(GameObject* collider);
    
    Vec2f inFront(Vec2f start, float diretion, int inFrontBy);
    
    
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
    int surfaceY;
    
    //game values + counters
    float depth;
    int urchinLastSeen;
    int sporeLastSeen;
    int starLastSeen;
    int jellyLastSeen;
    int eggLastSeen;
    bool aboveSurface;
    bool insideEgg;
    
    //OSC stuff
    OSCManager* oscManager;
    EnvironmentManager* environment;
};

#endif /* defined(__Cellv0__EntityManager__) */
