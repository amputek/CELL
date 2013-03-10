#include "EntityManager.h"

//Constructor initialises the hero, the floor and surface, and some starting entities
EntityManager :: EntityManager( OSCManager* osc){
    
    oscManager = osc;
    image = new Images();
    environment = new EnvironmentManager( image );
    
    hero = new Player(Vec2f(-200,-1000), image->player() ) ;
    
    offset = hero->global;
    
    //friendlies.push_back( new Friendly(Vec2f(100,-100), image->friendly() ) );
    eggs.push_back( new Egg(Vec2f(-200,-1000), image->egg() ) );
    
   // jellies.push_back( new Jelly( hero->global, image->jelly(1) ) );
    
    colliders.push_back(hero);
    
   // colliders.push_back( jellies.back() );
    //colliders.insert( colliders.end(), friendlies.begin() , friendlies.end() );
    colliders.insert( colliders.end(), eggs.begin(),    eggs.end() );
    
//    floor = new Floor();
//    surface = new SeaSurface();
    surfaceY = -5000;

    urchinLastSeen = 0;
    sporeLastSeen = 0;
    starLastSeen = 0;
    jellyLastSeen = 0;
    eggLastSeen = 0;
}


//Update loop called from the Main App class
//Checks for incoming messages from OSC
//Updates entities (environment and NPCs)
//and activates PCG
void EntityManager :: update(){
    
    oscManager->recieveMessage();
    updateEntities();
    updateOffset( );
    environment->update( hero->global );
    entityGenerator();
}


//RECIEVERS FROM OSC MANAGER
void EntityManager :: pulseFriendly(int index){
    environment->splash( friendlies.at(index)->global, 10, 30 );
}

void EntityManager :: pulseSpark(int index){
    sparks.at(index)->radius = 10;
    environment->splash( sparks.at(index)->global, 10, 25 );
}

void EntityManager :: pulseUrchin(int index){
    environment->splash( urchins.at(index)->global, urchins.at(index)->radius, 25 );
}

void EntityManager :: createSpore(){
    int r = int(rand(0,3));
    Vec2f loc = hero->global;
    
    for(int i = 0; i < rand(10,35); i++){
        spores.push_back(new Spore(loc + Vec2f(rand(-200,200), rand(-200,200)), rand(0.1,1.1), r, image->spore(r) ) );
    }
    sporeLastSeen = 0;
    
}

void EntityManager :: createStarfish(){
    cout << "Creating new Starfish" << "\n";
    starfish.push_back( new Starfish( hero->global + Vec2f(100,100)) );
    colliders.push_back( starfish.back() );
    starLastSeen = 0;
}

//Updates the Main Character, moving them towards the mouse position
void EntityManager :: updateHero(Vec2f mousePos){
    
    if(mousePos.x < 0){    mousePos.x = 0;   }
    if(mousePos.x > 800){  mousePos.x = 800; }
    if(mousePos.y < 0){    mousePos.y = 0;   }
    if(mousePos.y > 600){  mousePos.y = 600; }
    
    hero->moveTo( globalise(mousePos, 1) );
    hero->update();
    depth = ((500 - hero->global.y) / 7000);
    if(hero->global.y < -5000){
        if(aboveSurface == false){
            aboveSurface = true;
        //    oscManager->surface(1);
        }
    } else {
        environment->splash( hero->global, hero->radius+2, 12);
        
        if(aboveSurface == true){
            aboveSurface = false;
            for(int i = 0; i < 10; i++){
                environment->splash( hero->global + Vec2f(rand(-20,20), rand(-20,20)), 0, rand(5,40) );
            }
            
         //   oscManager->surface(0);
        }
    }
    
    if(hero->levelling() == true){
        environment->splash(hero->global + Vec2f(rand(-20,20), rand(-20,20)), 0, rand(5,40));
    }
}

//Removes a specified entity from the collider list
void EntityManager :: removeFromColliders(GameObject* collider){
    cout << "erasing " << collider << "\n";
    for( vector<GameObject*>::iterator p = colliders.begin(); p < colliders.end(); ){
        if(collider == (*p)){
            p = colliders.erase(p);
        } else {
            ++p;
        }
    }
}

//Update Entities
void EntityManager :: updateEntities(){
    updatePlankton();
    updateUrchins();
    updateSparks();
    updateSpores();
    updateEggs();
    updateStarfish();
    updateJellies();
    updateFriendlies();
    updateGrass();
}

void EntityManager :: updatePlankton(){
    
    for( vector<Plankton*>::iterator p = plankton.begin(); p < plankton.end(); ){
        (*p)->update();
        
        bool ended = false;
        
        if(((*p)->global - hero->global).lengthSquared() > 500000.0f){
            delete *p;
            p = plankton.erase(p);
            ended = true;
        } else {
            int i = 0;
            while(ended == false && i < colliders.size()){
                float d = ((*p)->global - colliders.at(i)->global).lengthSquared();
                if(d < 200.0f){
                    environment->bubble( (*p)->local, 4);
                    environment->splash( (*p)->global, 1, 50 );
                    oscManager->eatPlankton(depth, (*p)->type(), (*p)->radius);
                    if(i == 0){
                        hero->incEaten( (*p)->type() );
                        if(hero->getEaten() % 8 == 0){
                            environment->splash( hero->global, 0, 100 );
                            //oscManager->eighthPlankton();
                            hero->levelUp();
                        }
                    }
                    delete *p;
                    p = plankton.erase(p);
                    ended = true;
                }
                i++;
            }
        }
        
        if(ended == false){
            ++p;
        }
    }
}

void EntityManager :: updateUrchins(){
    
    for( vector<Urchin*>::iterator p = urchins.begin(); p != urchins.end(); ){
        
        //if Urchin is far enough away from hero
        if(((*p)->global - hero->global).lengthSquared() > 1000000){
            //remove from collider list, then delete from urchin list and erase object
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = urchins.erase(p);
        } else {
            //Otherwise, go through standard update: collision, target, OSC stuff
            (*p)->collide(hero->global);
            (*p)->update();
            (*p)->updateTarget(colliders);
            if((*p)->inSpace == true){
                if(plankton.size() > 0){
                    if(plankton.at(0)->global.y > -1000 && (plankton.at(0)->global - (*p)->global).lengthSquared() < 100000){
                        (*p)->setDestination( plankton.at(0)->global );
                    } else {
                        (*p)->setDestination( Vec2f((*p)->global.x + rand(-100,100), rand(0,300)) );
                    }
                }
            }
            
           // oscManager->urchin( (*p)->contactAmount(), (*p)->radius, (*p)->feelerLength );
            
            if((*p)->onScreen() == true){
                urchinLastSeen = 0;
            }
            
            ++p;
        }
        
    }
    
    urchinLastSeen++;
    
}

void EntityManager :: updateSpores(){
    
    for( vector<Spore*>::iterator p = spores.begin(); p < spores.end(); ){
        
        (*p)->update();
        (*p)->collide(hero->global);
        
        //sound happens if there's any contact with the field
        if((*p)->contact() == true){
            environment->splash( (*p)->global, 22*(*p)->depth, 30 );
        //    oscManager->sporeBoop( (*p)->health );
        }
        
        //reset field counter if the character is near enough to any field
        if(((*p)->global - hero->global).lengthSquared() < 7000000){
            sporeLastSeen = 0;
        }
        
        
        if((*p)->alive() == false){
            cout << "new Spark" << "\n";
            sparks.push_back(new Spark((*p)->global, (*p)->type() ) );
        //    oscManager->newSpark( (*p)->type );
            colliders.push_back( sparks.back() );
            environment->bubble( (*p)->local, 15 );

            for(int i = 0; i < 10; i++){
                environment->splash( (*p)->global + Vec2f(rand(-10,10), rand(-10,10)), rand(5,20), rand(5,35) );
            }
            
            delete *p;
            p = spores.erase(p);
            
        } else {
            ++p;
        }
    }
    
    sporeLastSeen++;
}

void EntityManager :: updateSparks(){
    for(int i = 0; i < sparks.size(); i++){
        sparks.at(i)->update();
        sparks.at(i)->setDestination( hero->global );
        for(int n = 0; n < urchins.size(); n++){
            if((sparks.at(i)->global - urchins.at(n)->global).length() < 1000){
                sparks.at(i)->setDestination(urchins.at(n)->global);
            }
        }
        if((sparks.at(i)->global - hero->global).length() > 1000){
            sparks.at(i)->setDestination( hero-> global );
        }
        
        environment->splash( sparks.at(i)->global, sparks.at(i)->radius, 12 );
    }
}

void EntityManager :: updateEggs(){
    
    for(int i = 0; i < eggs.size(); i++){
        eggs.at(i)->update();
        eggs.at(i)->collide(hero->global);
        // oscManager->egg(eggs.at(i)->inside());
        if(eggs.at(i)->inside() == true){
            if(hero->speed > 0.005){
                hero->speed -= 0.0005;
            }
        } else {
            
            hero->speed = 0.015;
        }
        
        if(eggs.at(i)->onScreen() == true){
            eggLastSeen = 0;
        }
    }

    eggLastSeen++;    
}

void EntityManager :: updateStarfish(){
    
    for(int i = 0; i < starfish.size(); i++){
        starfish.at(i)->collide(hero->global);
        starfish.at(i)->update();
        starfish.at(i)->updateTarget(colliders);
        starfish.at(i)->setDestination( starfish.at(i)->global + Vec2f(rand(-10,10), rand(-10,10)) );

        if(starfish.at(i)->onScreen() == true){
            starLastSeen = 0;
        }
        
        if( getElapsedFrames() % 30 == 0){
            environment->bubble( starfish.at(i)->local, 1 );
        }
    }
    
    starLastSeen++;
    
}

void EntityManager :: updateJellies(){
    
    for( vector<Jelly*>::iterator j = jellies.begin(); j < jellies.end(); ){
        (*j)->update();
        (*j)->collide(hero->global);
//        oscManager->jelly((*j)->contacts() );

        
        if((*j)->onScreen() == true){
            jellyLastSeen = 0;
        }
        
        (*j)->setDestination( (*j)-> global );
        
        if( ((*j)->global - hero->global).lengthSquared() > 1000000){
            cout << "Erasing Jellyfish" << "\n";
            removeFromColliders((GameObject*)(*j));
            delete *j;
            j = jellies.erase(j);
        } else {
            ++j;
        }
    }
    
    jellyLastSeen++;
    
}

void EntityManager :: updateFriendlies(){
    for(int i = 0; i < friendlies.size(); i++){
        friendlies.at(i)->update();
        friendlies.at(i)->updateTarget(colliders);
        if(friendlies.at(i)->inSpace == true){
            if(plankton.size() > 0){
                if(plankton.at(0)->global.y < 0){
                    friendlies.at(i)->setDestination( plankton.at(0)->global );
                } else {
                    friendlies.at(i)->setDestination( Vec2f(friendlies.at(i)->global.x + rand(-100,100), rand(-100,100)) );
                }
            }
        }
    }
}

void EntityManager :: updateGrass(){
    // cout << longGrass.size() << "\n";
    for( std::vector<Feeler*>::iterator p = longGrass.begin(); p != longGrass.end(); ){
        
        (*p)->update();
        (*p)->collide(hero->global);
        (*p)->addForce(Vec2f(rand(-4,4), -15.0));
        
    //    oscManager->feeler( (*p)->contact(), (*p)->length() );
        
        if(abs((*p)->global.x - hero->global.x) > 5000){
            //  cout << "deleting grass" << "\n";
            delete *p;
            p = longGrass.erase(p);
        } else {
            ++p;
        }
        
    }
}

//Updates the global Offset value depending on the Hero's location
void EntityManager :: updateOffset(){

    bool egg = false;
//    
//    for(int i = 0; i < eggs.size(); i++){
//        if( eggs.at(i)->inside() == true ){
//            insideEgg = true;
//            egg = true;
//            offset += 0.05f * (eggs.at(i)->local - Vec2f(getWindowWidth()/2, (getWindowHeight()/2) ) );
//        }
//    }
    

        float over = 0;
        if(hero->global.y > 300){
            over = hero->global.y - 300;
        }
        offset += 0.03f * (hero->local - Vec2f(getWindowWidth()/2, (getWindowHeight()/2)+over));

}


int EntityManager :: distribute(){
    vector<float> probabilities;
    probabilities.push_back(10);
    probabilities.push_back(8);
    probabilities.push_back(6);
    probabilities.push_back(5);
    probabilities.push_back(3);
    
    float total = 0;
    for(int i = 0; i < probabilities.size(); i++){
        total+= probabilities.at(i);
    }
    float random = rand(0.0, total);
    
    for(int i = 0; i < probabilities.size(); i++){
        if(random < probabilities.at(i)){
            return i;
        }
        random -= probabilities.at(i);
    }
    return 0;
}

//Procedural Content Generation (PCG) method
void EntityManager :: entityGenerator(){
    
    //PLANKTON
    if(getElapsedFrames() % 150 == 0){
        if(plankton.size() < 25){
        
            int index = distribute();
            
            if(rand(0.0,1.0) < 0.5){
                for(int i = 0; i < int(rand(0,10)); i++){
                    plankton.push_back( new Plankton( inFront(hero->global, hero->direction, 600) + Vec2f(rand(-50,50), rand(-50,50)),  image->plankton(index), index ) );
                }
            }
            plankton.push_back( new Plankton( inFront(hero->global, hero->direction, 600), image->plankton(index), index) );
        }
    }
    

    //LONG GRASS
    if(hero->global.y > 0){
        if(longGrass.size() < 10){
            if(rand(0.0,1.0) > 0.99){
                float nx = inFront(hero->global, hero->direction, 600).x;
                cout << "new grass" << "\n";
                //   if(abs(nx - hero->global.x) > 450){
                for(int i = 0; i < int(rand(3,6)); i++){
                    longGrass.push_back( new Feeler( Vec2f(nx + int(rand(-40,40)), 500 ), int(rand(5,10)), rand(0.9,1.15)) );
                    longGrass.back()->update();
                }
                //  }
            }
        }
    }
    
    
    if(urchinLastSeen >= 200 && hero->global.y > 0){
        cout << "Creating new Urchin" << "\n";
        urchins.push_back( new Urchin( inFront(hero->global, hero->direction, 600), image->urchin() ) );
        colliders.push_back( urchins.back() );
        urchinLastSeen = 0;
    }
    
    if(starLastSeen >= 1000 && hero->global.y > -3000){
        cout << "Creating new Puff" << "\n";
        starfish.push_back( new Starfish( inFront(hero->global, hero->direction, 600) ) );
        colliders.push_back( starfish.back() );
        starLastSeen = 0;
    }
    
    if(jellyLastSeen == 1500){
        cout << "Creating new Jelly" << "\n";
        jellies.push_back( new Jelly( inFront(hero->global, hero->direction, 600), image->jelly(0) ) );
        colliders.push_back( jellies.back() );
        jellyLastSeen = 0;
    }
    
    if(sporeLastSeen == 600){
        cout << "Creating new Field" << "\n";
        int r = int(rand(0,3));
        Vec2f loc = inFront(hero->global, hero->direction, 1600);
        
        for(int i = 0; i < rand(5,30); i++){
            spores.push_back(new Spore(loc + Vec2f(rand(-100,100), rand(-100,100)),
                                       rand(0.5,1.1), r, image->spore(r)) );
        }
        sporeLastSeen = 0;
    }
    
    
    if(eggLastSeen == 1000){
        cout << "Creating new Egg" << "\n";
        eggs.push_back(new Egg( inFront(hero->global, hero->direction, 800), image->egg() ) );
        colliders.push_back( eggs.back() );
        eggLastSeen = 0;
    }
    
}


Vec2f EntityManager :: inFront(Vec2f start, float direction, int inFrontBy){
    float x = start.x + sin(direction)*inFrontBy;
    float y = start.y + cos(direction)*inFrontBy;
    if(y > 0){     y = 0; };
    if(y < -4500){ y = 4500; };
    
    return Vec2f(x,y);
}

//calls every entities' draw function
void EntityManager :: drawEntities(){
    
    if(hero->global.y > 100){
    //    floor->draw();
    }
    
    environment->draw();
    
    for(int i = 0; i < urchins.size();   i++){ urchins.at(i)->draw();   }
    for(int i = 0; i < starfish.size();    i++){ starfish.at(i)->draw();    }
    for(int i = 0; i < eggs.size();     i++){ eggs.at(i)->draw();     }
    for(int i = 0; i < longGrass.size();  i++){ longGrass.at(i)->draw();  }
    for(int i = 0; i < plankton.size(); i++){ plankton.at(i)->draw(); }
    for(int i = 0; i < spores.size();   i++){ spores.at(i)->draw();   }
    for(int i = 0; i < jellies.size();  i++){ jellies.at(i)->draw();  }
    for(int i = 0; i < friendlies.size();    i++){ friendlies.at(i)->draw();  }
    for(int i = 0; i < sparks.size();    i++){ sparks.at(i)->draw();  }
    hero->draw();
    
    if(hero->global.y < -2000){
   //     surface->draw();
    }
}