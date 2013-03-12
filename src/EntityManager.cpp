#include "EntityManager.h"

//Constructor initialises the hero, the floor and surface, and some starting entities
EntityManager :: EntityManager( Images* img){
    
    //set up OSC
    oscManager = new OSCManager();
    oscManager->entities = this;
    
    //set up managers
    image = img;
    environment = new EnvironmentManager( image );
    
    //first entities
    hero = new Player(Vec2f(-400,-1200), image->player() );
    offset = hero->global;
    
    eggs.push_back( new Egg(Vec2f(-200,-1000), image->egg() ) );
    colliders.push_back(hero);

    //initialise counters
    urchinLastSeen = 0;
    sporeLastSeen  = 0;
    starLastSeen   = 0;
    jellyLastSeen  = 0;
    eggLastSeen    = 0;
}

void EntityManager :: quit(){
    oscManager->quit();
}

//Update loop called from the Main App class
//Checks for incoming messages from OSC
//Updates entities (environment and NPCs)
//and activates PCG
void EntityManager :: update(){
    
    oscManager->recieveMessage();

    updatePlankton();
    updateUrchins();
    updateSparks();
    updateSpores();
    updateEggs();
    updateStarfish();
    updateJellies();
    updateFriendlies();
    updateGrass();
    
    updateOffset( );
    environment->update( hero->global );
    entityGenerator();
}


void EntityManager :: create(string species){
    if(species == "star"){
        starfish.push_back( new Starfish(hero->global) );
        colliders.push_back( starfish.back() );
    }
    
    if(species == "egg"){
        
    }
    
    if(species == "jelly"){
        
    }
    
    if(species == "spores"){
        
    }
}

//RECIEVERS FROM OSC MANAGER
void EntityManager :: pulse(string species, int index){
    if(species == "friendly"){
        environment->splash( friendlies.at(index)->global, 10, 30 );
    }
    if(species == "spark"){
        sparks.at(index)->radius = 10;
        environment->splash( sparks.at(index)->global, 10, 25 );
    }
    if(species == "urchin"){
        environment->splash( urchins.at(index)->global, urchins.at(index)->radius, 25 );
    }
}

//Updates the Main Character, moving them towards the mouse position
void EntityManager :: updateHero(Vec2f mousePos){

    hero->moveTo(mousePos);
    hero->update();
    
    oscManager->setDepth( (-hero->global.y) / 7000 );
    
    
    //Check if Hero is above or below surface
    if(hero->global.y < surfaceY){
        if(aboveSurface == false){
            aboveSurface = true;
            oscManager->surface(1);
        }
    } else {
        environment->splash( hero->global, hero->radius+2, 12);
        
        if(aboveSurface == true){
            aboveSurface = false;
            for(int i = 0; i < 10; i++){
                environment->splash( hero->global + vrand(20), 0, rand(5,40) );
            }
            
            oscManager->surface(0);
        }
    }
    
    if(hero->levelling() == true){
        environment->splash(hero->global + vrand(20), 0, rand(5,40));
    }
}

//Removes a specified entity from the collider list
void EntityManager :: removeFromColliders(GameObject* collider){
    for( vector<GameObject*>::iterator p = colliders.begin(); p < colliders.end(); ){
        if(collider == (*p)){
            p = colliders.erase(p);
        } else {
            ++p;
        }
    }
}


bool EntityManager :: farFromHero( Vec2f location ){
    return ( dist(hero->global, location) > 10000 );
}

void EntityManager :: updatePlankton(){
    
    for( vector<Plankton*>::iterator p = plankton.begin(); p < plankton.end(); ){
        (*p)->update();
        
        bool ended = false;
        
        if( farFromHero( (*p)->global ) == true ){
            ended = true;       //if plankton is far enough away from hero, delete
        } else {
            int i = 0;
            while(ended == false && i < colliders.size()){
                if(dist( (*p)->global, colliders.at(i)->global) < 10 ){
                    environment->bubble( (*p)->local, 4);
                    environment->splash( (*p)->global, 1, 50 );
                    oscManager->eatPlankton( 0.0, (*p)->type(), (*p)->radius);
                    if(i == 0){
                        hero->incEaten( (*p)->type() );
                        if(hero->getEaten() % 8 == 0){
                            environment->splash( hero->global, 0, 100 );
                            oscManager->eighthPlankton();
                        }
                    }
                    ended = true;
                }
                i++;
            }
        }
        
        if(ended == true){
            delete *p;
            p = plankton.erase(p);
        } else {
            ++p;
        }
    }
}

void EntityManager :: updateUrchins(){
    
    for( vector<Urchin*>::iterator p = urchins.begin(); p != urchins.end(); ){
        
        //if Urchin is far enough away from hero
        if( farFromHero( (*p)->global ) == true){
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
                    if(plankton.at(0)->global.y > -1000 && dist(plankton.at(0)->global, (*p)->global ) < 800 ){
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
        
        bool ended = false;
        
        if( farFromHero( (*p)->global ) == true ){
            ended = true;
        } else {
    
            (*p)->update();
            (*p)->collide(hero->global);
            
            //sound happens if there's any contact with the field
            if((*p)->contact() == true){
                environment->splash( (*p)->global, 22*(*p)->depth, 30 );
                oscManager->sporeBoop( (*p)->health );
            }
            
            //reset field counter if the character is near enough to any field
            if(((*p)->onScreen() == true)){
                sporeLastSeen = 0;
            }
            
            if((*p)->alive() == false){
                cout << "Creating new Spark" << "\n";
                
                ended = true;

                sparks.push_back(new Spark((*p)->global, (*p)->type() ) );
            
                oscManager->newSpark( (*p)->type() );
                colliders.push_back( sparks.back() );
                
                environment->bubble( (*p)->local, 15 );
                for(int i = 0; i < 10; i++){
                    environment->splash( (*p)->global + vrand(10), rand(5,20), rand(5,35) );
                }
            }
        }
        
        
        if(ended == true){
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
            if( dist(sparks.at(i)->global, urchins.at(n)->global) < 100){
                sparks.at(i)->setDestination(urchins.at(n)->global);
            }
        }
        if( dist(sparks.at(i)->global, hero->global) > 100){
            sparks.at(i)->setDestination( hero-> global );
        }
        
        environment->splash( sparks.at(i)->global, sparks.at(i)->radius, 12 );
    }
}

void EntityManager :: updateEggs(){
    
    
    for( vector<Egg*>::iterator p = eggs.begin(); p < eggs.end(); ){
        
        if( farFromHero( (*p)->global ) == true ){
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = eggs.erase(p);
        } else {
            (*p)->update();
            (*p)->collide(hero->global);
            
            oscManager->egg( (*p)->inside() );
            
            if( (*p)->inside() == true ){
            //    hero->slow();
            }
            
            if( (*p)->onScreen() == true){
                eggLastSeen = 0;
            }
            
            ++p;
        }
    }
    
    eggLastSeen++;
   
}

void EntityManager :: updateStarfish(){
    
    
    for( vector<Starfish*>::iterator p = starfish.begin(); p < starfish.end(); ){
    
        if( farFromHero( (*p)->global ) == true ){
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = starfish.erase(p);
            
        } else {
            (*p)->collide(hero->global);
            (*p)->update();
            (*p)->updateTarget(colliders);
            (*p)->setDestination( (*p)->global + vrand(10) );
            
            if( (*p)->activated() == true){
                cout << "activating " << "\n";
                oscManager->changeChord();
            }

            if( (*p)->onScreen() == true){
                starLastSeen = 0;
            }
            
            if( getElapsedFrames() % 30 == 0){
                environment->bubble( (*p)->local, 1 );
            }
            
            ++p;
        }
    }
    
    starLastSeen++;
    
}

void EntityManager :: updateJellies(){
    
    for( vector<Jelly*>::iterator j = jellies.begin(); j < jellies.end(); ){

        (*j)->update();
        (*j)->collide(hero->global);
        (*j)->setDestination( (*j)-> global );
        oscManager->jelly( (*j)->contacts() );

        if((*j)->onScreen() == true){
            jellyLastSeen = 0;
        }
        
        if( farFromHero( (*j)->global ) == true ){
            removeFromColliders( (GameObject*)(*j) );
            delete *j;
            j = jellies.erase(j);
        } else {
            ++j;
        }
        
    }
    
    jellyLastSeen++;
    
}

void EntityManager :: updateFriendlies(){
    
    for( vector<Friendly*>::iterator p = friendlies.begin(); p < friendlies.end(); ){
    
        (*p)->update();
        (*p)->updateTarget(colliders);
        
        if( (*p)->inSpace == true && plankton.size() > 10){
            (*p)->setDestination( plankton.at(9)->global );
        }
    }
}

void EntityManager :: updateGrass(){
    
    for( std::vector<Feeler*>::iterator p = longGrass.begin(); p != longGrass.end(); ){
        
        if(farFromHero( (*p)->global ) == true ){
            delete *p;
            p = longGrass.erase(p);
        } else {
            (*p)->update();
            (*p)->collide(hero->global);
            (*p)->addForce(Vec2f(rand(-4,4), -15.0));
        
            //oscManager->feeler( (*p)->contact(), (*p)->length() );
            ++p;
        }

    }
}


//Updates the global Offset value depending on the Hero's location
void EntityManager :: updateOffset(){

    float over = 0;
    if(hero->global.y > 300){
        over = hero->global.y - 300;
    }
    offset += 0.03f * (hero->local - Vec2f(getWindowWidth()/2, (getWindowHeight()/2)+over));

}


//Procedural Content Generation (PCG) method
void EntityManager :: entityGenerator(){
    
    //PLANKTON
    if(getElapsedFrames() % 150 == 0 && plankton.size() < 25){
        int index = irand(0,5);
        
        if(rand(0.0,1.0) < 0.5){
            for(int i = 0; i < irand(0,10); i++){
                plankton.push_back( new Plankton( inFront(hero->global, hero->direction, 600) + vrand(50),  image->plankton(index), index ) );
            }
        }
        plankton.push_back( new Plankton( inFront(hero->global, hero->direction, 600), image->plankton(index), index) );
    }
    

    //LONG GRASS
    if(hero->global.y > 0 && longGrass.size() < 10 && rand(0.0,1.0) > 0.99){
        float nx = inFront(hero->global, hero->direction, 600).x;
        cout << "Creaing new Grass" << "\n";
        for(int i = 0; i < irand(3,6); i++){
            longGrass.push_back( new Feeler( Vec2f(nx + irand(-40,40), 500 ), irand(5,10), rand(0.9,1.15)) );
            longGrass.back()->update();
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
        jellies.push_back( new Jelly( inFront(hero->global, hero->direction, 600), image->jelly( 2 ) ) );
        colliders.push_back( jellies.back() );
        jellyLastSeen = 0;
    }
    
    if(sporeLastSeen == 600){
        cout << "Creating new Field" << "\n";
        int r = irand(0,3);
        Vec2f loc = inFront(hero->global, hero->direction, 1600);
        
        for(int i = 0; i < rand(5,30); i++){
            spores.push_back(new Spore(loc + vrand(100), rand(0.5,1.1), r, image->spore(r)) );
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
    if(y > 0){
        y = 100;
        x += 1000;
    };
    if(y < -6500){
        y = -6000;
        x -= 1000;
    };
    
    return Vec2f(x,y);
}

//calls every entities' draw function
void EntityManager :: drawEntities(){
    
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    float widthMod = sin(hero->global.x*0.0001)*0.1;
    
    float depth = (-hero->global.y) / 7000;
        
    float r = depth*0.06 -widthMod;
    float g = depth*0.3 + widthMod;
    float b = depth*0.8;
    
    gl::clear( Color( r,g,b ) );

    
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
    
    environment->drawMask();
}