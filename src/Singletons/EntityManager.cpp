#include "EntityManager.hpp"

//Constructor initialises the hero, the floor and surface, and some starting entities
EntityManager :: EntityManager( Images* img){
    
    //set up OSC
    oscManager = new OSCManager();
    oscManager->entities = this;
    
    //set up managers, singletons
    image = img;
    environment = new EnvironmentManager( image );
    
    //first entities: the hero (player) and the starting egg
    hero = new Player(vec2(-400,-1000), image->player() );
    offset = hero->global;
    
    eggs.push_back( new Egg(vec2(-400,-1000), image->egg() ) );
    colliders.push_back(hero);
    colliders.push_back( eggs.back() );

    //initialise counters and states
    urchinLastSeen = 0;
    sporeLastSeen  = 0;
    starLastSeen   = 0;
    jellyLastSeen  = 0;
    eggLastSeen    = 0;
    aboveSurface = false;
    insideEgg = false;
}

void EntityManager :: quit(){
    oscManager->quit();
}

//Procedural Content Generation (PCG) method
void EntityManager :: entityGenerator(){
    
    //PLANKTON
    if(getElapsedFrames() % 40 == 0 && plankton.size() < 40){
        int index = irand(0,7);
        
        //position of new plankton in front of the hero
        vec2 loc = inFront(hero->global, hero->direction, rand(500,600));
        
        //make sure plankton do not spawn inside egg
        bool outsideEgg = true;
        for(int i = 0; i < eggs.size();i++){
            if(dist(loc, eggs.at(i)->global) < 60){
                outsideEgg = false;
            }
        }
    
        //add plankton to collection
        if(outsideEgg == true){
            for(int i = 0; i < irand(1,8); i++){
                plankton.push_back( new Plankton( loc + vrand(40),  image->plankton(index), index ) );
            }
        }
    }
    
    //GRASS
    if(hero->global.y > -500 && longGrass.size() < 20 && rand(0.0,1.0) > 0.99){
        float nx = inFront(hero->global, hero->direction, 600).x;
        for(int i = 0; i < irand(3,6); i++){
            longGrass.push_back( new Feeler( vec2(nx + irand(-40,40), 0 ), irand(5,10), rand(0.9,1.15)) );
            longGrass.back()->update();
        }
    }
    
    //URCHIN - only deep sea
    if(urchinLastSeen >= 800 && hero->global.y > -1000){
        urchins.push_back( new Urchin( inFront(hero->global, hero->direction, 600), image->urchin() ) );
        colliders.push_back( urchins.back() );
        urchinLastSeen = 0;
    }

    //STARFISH - not shallow waters
    if(starLastSeen >= 1800 && hero->global.y > -3000){
        starfish.push_back( new Starfish( inFront(hero->global, hero->direction, 600) ) );
        colliders.push_back( starfish.back() );
        starLastSeen = 0;
    }

    //JELLYFISH - anywhere
    if(jellyLastSeen == 1000){ //1000
        int type = irand(0,2);
        jellies.push_back( new Jelly( inFront(hero->global, hero->direction, 600), type, image->jelly( type ) ) );
        colliders.push_back( jellies.back() );
        jellyLastSeen = 0;
    }

    //SPORES - anywhere, more common (every 600 frames)
    if(sporeLastSeen == 600){
        int r = irand(0,3); //type
        vec2 loc = inFront(hero->global, hero->direction, 1600);
        
        for(int i = 0; i < rand(5,30); i++){
            spores.push_back(new Spore(loc + vrand(100), rand(0.5,1.1), r, image->spore(r)) );
            spores.push_back(new Spore(loc + vrand(100), rand(0.1,1.1), r, image->spore(r)) );
        }
        sporeLastSeen = 0;
    }

    //EGG
    if(eggLastSeen == 1200){
        eggs.push_back(new Egg( inFront(hero->global, hero->direction, 800), image->egg() ) );
        colliders.push_back( eggs.back() );
        friendlies.push_back( new Friendly( eggs.back()->global, image->friendly() ) );
        colliders.push_back( friendlies.back() );
        oscManager->newFriendly();
        eggLastSeen = 0;
        
        //delete any plankton that are inside the egg
        for( vector<Plankton*>::iterator p = plankton.begin(); p < plankton.end(); ){
            if( dist( (*p)->global, eggs.back()->global ) < eggs.back()->radius ){
                delete *p;
                p = plankton.erase(p);
            } else {
                ++p;
            }
        }
    }
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

//UPDATE METHODS FOR ENTITY COLLECTIONS

void EntityManager :: updateHero(vec2 mousePos){
    
    hero->moveTo(mousePos);
    hero->update();
    
    oscManager->setDepth( (-hero->global.y) / 7000 );
    
    //Check if Hero is above or below surface
    if(hero->global.y < -7000){
        if(aboveSurface == false){
            //triggers when hero jumps out of water
            aboveSurface = true;
            oscManager->surface(1);
        }
    } else {
        //constantly make splashes around hero
        environment->splash( hero->global, hero->radius+2, 12);
        
        if(aboveSurface == true){
            //when hero re-enters the water, create some splashes and tell SuperCollider
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

void EntityManager :: updateGrass(){
    
    for( std::vector<Feeler*>::iterator p = longGrass.begin(); p != longGrass.end(); ){
        
        if(farFromHero( (*p)->global ) == true ){
            delete *p;
            p = longGrass.erase(p);
        } else {
            (*p)->update();
            (*p)->collide(hero->global);
            (*p)->addForce(vec2(rand(-4,4), -15.0));
            oscManager->grass( (*p)->contact() );
            ++p;
        }
    }
}

void EntityManager :: updatePlankton(){
    
    for( vector<Plankton*>::iterator p = plankton.begin(); p < plankton.end(); ){
        (*p)->update();
        
        //true if plankton should be deleted (either from being eaten, or from being too far from the hero)
        bool ended = false;
        
        if( farFromHero( (*p)->global ) == true ){
            ended = true;       //if plankton is far enough away from hero, delete
        } else {
            
            //cycle through colliders, see if any have eaten the plankton
            int i = 0;
            while(ended == false && i < colliders.size()){
                
                //check if collider has eaten plankton
                if(dist( (*p)->global, colliders.at(i)->global) < (*p)->radius + colliders.at(i)->radius ){
                    
                    //do plankton eating business (bubbles, splashes, osc message)
                    environment->bubble( (*p)->local, 4);
                    environment->splash( (*p)->global, 1, 50 );
                    float pan = ( (*p)->global.x - hero->global.x);
                    oscManager->eatPlankton( (*p)->type(), pan , dist(hero->global, (*p)->global) );
                    
                    //if collider is the HERO ( collider #0 ) and not any other entity
                    if(i == 0){
                        //hero levels up etc
                        hero->incEaten( (*p)->type() );
                        if(hero->getEaten() % 20 == 0){
                            hero->levelUp();
                            environment->splash( hero->global, 0, 100 );
                            oscManager->eighthPlankton();
                        }
                    }
                    ended = true;
                }
                i++;
            }
        }
        
        //if plankton needs to be deleted (through being eaten, or being too far away from hero), delete
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
            //Otherwise, go through standard update: collision, target, OSC messages
            (*p)->collide(hero->global);
            (*p)->update();
            (*p)->updateTarget(colliders);
            if((*p)->inSpace == true){
                if(plankton.size() > 0){
                    if(plankton.at(0)->global.y > -1000 && dist(plankton.at(0)->global, (*p)->global ) < 800 ){
                        (*p)->setDestination( plankton.at(0)->global );
                    } else {
                        (*p)->setDestination( vec2((*p)->global.x + rand(-100,100), rand(0,300)) );
                    }
                }
            }
            
            //friendlies also cause collision
            for(int i = 0 ; i < friendlies.size(); i++){
                (*p)->collide( friendlies.at(i)->global );
            }
        
            //OSC message - distance and collision amount
            oscManager->urchin( dist( (*p)->global, hero->global ), (*p)->contactAmount() );
            
            if((*p)->onScreen() == true){
                urchinLastSeen = 0;
            }
            
            ++p;
        }
    }
    
    urchinLastSeen++;
    
}

void EntityManager :: updateSparks(){
    for(int i = 0; i < sparks.size(); i++){
        
        //standard update pattern
        sparks.at(i)->update();
        sparks.at(i)->updateTarget( colliders );
        
        //interacts with urchin if near enough
        for(int n = 0; n < urchins.size(); n++){
            if( dist(sparks.at(i)->global, urchins.at(n)->global) < 100){
                sparks.at(i)->setDestination(urchins.at(n)->global);
            }
        }
        
        //interacts with jellies if near enough
        for(int n = 0; n < jellies.size(); n++){
            if( dist(sparks.at(i)->global, jellies.at(n)->global) < 100){
                sparks.at(i)->setDestination(jellies.at(n)->global + vec2(0,rand(20,70)));
            }
        }
        
        //but if the player gets too far away, follow the player
        if( sparks.at(i)->inSpace == true){
            if( dist(sparks.at(i)->global, hero->global) > 100){
                sparks.at(i)->setDestination( hero-> global );
            }
        }
        
        //sparks constantly create splashes
        environment->splash( sparks.at(i)->global, sparks.at(i)->radius, 10 );
    }
}

void EntityManager :: updateSpores(){
    
    for( vector<Spore*>::iterator p = spores.begin(); p < spores.end(); ){
        
        bool ended = false;
        
        if( farFromHero( (*p)->global ) == true ){
            //true if spore is ended for any reason (either via the player collision, or from being too far away from the player)
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
            
            //checks if health is low enough
            if((*p)->alive() == false){
                
                ended = true;
                
                //do spark-creating business (new Spark, bubbles, splashes, OSC message)
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

void EntityManager :: updateEggs(){
    
    bool inside = false;
    
    for( vector<Egg*>::iterator p = eggs.begin(); p < eggs.end(); ){
        
        if( farFromHero( (*p)->global ) == true ){
            //delete if too far from hero
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = eggs.erase(p);
        } else {
            
            //update
            (*p)->update();
            (*p)->collide(hero->global);
            (*p)->setInside(hero->global);
            
            //friendlies can also cause collision with egg
            for(int i = 0 ; i < friendlies.size(); i++){
                (*p)->collide( friendlies.at(i)->global );
            }
            
            //change the global 'inside' state - true if the hero is inside ANY of the eggs
            if( (*p)->inside() == true ){
                inside = true;
            }
            
            if( (*p)->onScreen() == true){
                eggLastSeen = 0;
            }
            
            ++p;
        }
    }
    
    //if player is inside, send osc message, slow hero down
    if(inside == true){
        if(insideEgg == false){
            insideEgg = true;
            oscManager->egg(1);
            hero->slow(true);
        }
    } else {
        if(insideEgg == true){
            insideEgg = false;
            oscManager->egg(0);
            hero->slow(false);
        }
    }
    
    eggLastSeen++;
    
}

void EntityManager :: updateJellies(){
    
    for( vector<Jelly*>::iterator j = jellies.begin(); j < jellies.end(); ){
        
        (*j)->update();
        (*j)->collide(hero->global);
        (*j)->setDestination( (*j)-> global );
        
        //jellyfish also collide with sparks
        for(int i = 0; i < sparks.size(); i++){
            (*j)->collide(sparks.at(i)->global);
        }
        
        oscManager->jelly( (*j)->contacts(), dist( (*j)->global, hero->global) );
        
        if((*j)->onScreen() == true){
            jellyLastSeen = 0;
        }
        
        //delete if too far away
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
    
    int index = 0;
    for( std::vector<Friendly*>::iterator p = friendlies.begin(); p != friendlies.end(); ){
        
        (*p)->update();
        
        float distance = dist( (*p)->global, hero->global );
        
        if( (*p)->born() == false){
            
            //friendly is 'born' if hero is near enough
            if( distance < 40){
                (*p)->birth();
                oscManager->bornFriendly(index);
                (*p)->setDestination( (*p)->global + vrand(500) );
            }
            
        } else {
            
            //collision, AI
            (*p)->updateTarget(colliders);
            
            //move towards plankton #0
            if( plankton.size() > 0){
                (*p)->setDestination( plankton.at(0)->global );
            }
            
            //if friendly is near enough, send OSC messages
            if( distance < 1000){
                float pan = ( (*p)->global.x - hero->global.x);
                oscManager->updateFriendly( index, pan, distance );
            }
        }
        
        ++p;
        index++;
        
    }
}

void EntityManager :: updateStarfish(){
    
    for( vector<Starfish*>::iterator p = starfish.begin(); p < starfish.end(); ){
        
        if( farFromHero( (*p)->global ) == true ){
            //delete if too far away
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = starfish.erase(p);
            
        } else {
            
            //update standardly
            (*p)->collide(hero->global);
            (*p)->update();
            (*p)->updateTarget(colliders);
            (*p)->setDestination( (*p)->global + vrand(10) );
            
            //chord change if active
            if( (*p)->activated() == true){
                oscManager->changeChord();
            }
            
            if( (*p)->onScreen() == true){
                starLastSeen = 0;
            }
            
            //periodically creates bubbles
            if( getElapsedFrames() % 30 == 0){
                environment->bubble( (*p)->local, 1 );
            }
            
            ++p;
        }
    }
    
    starLastSeen++;
    
}




//RECIEVE FROM OSC MANAGER (SUPERCOLLIDER)
void EntityManager :: pulse(string species, int index){
    if(species == "friendly"){
//        environment->splash( friendlies.at(index)->global, 10, 30 );
    }
    if(species == "spark"){
        sparks.at(index)->radius = 10;
        environment->splash( sparks.at(index)->global, 10, 25 );
    }
    if(species == "urchin"){
        environment->splash( urchins.at(index)->global, urchins.at(index)->radius, 25 );
    }
}

void EntityManager :: create(string species){
    //URCHIN - only deep sea
    if(species == "urchin"){
        urchins.push_back( new Urchin( inFront(hero->global, hero->direction, 200), image->urchin() ) );
        colliders.push_back( urchins.back() );
    }
    
    //STARFISH - not shallow waters
    if(species == "starfish"){
        starfish.push_back( new Starfish( inFront(hero->global, hero->direction, 200) ) );
        colliders.push_back( starfish.back() );
    }
    
    //JELLYFISH - anywhere
    if(species == "jellyfish"){
        int type = irand(0,2);
        jellies.push_back( new Jelly( inFront(hero->global, hero->direction, 200), type, image->jelly( type ) ) );
        colliders.push_back( jellies.back() );
    }
    
    //SPORES - anywhere, more common (every 600 frames)
    if(species == "spores"){
        int r = irand(0,3); //type
        vec2 loc = inFront(hero->global, hero->direction, 500);
        
        for(int i = 0; i < rand(5,30); i++){
            spores.push_back(new Spore(loc + vrand(100), rand(0.5,1.1), r, image->spore(r)) );
        }
        sporeLastSeen = 0;
    }
    
    //EGG
    if(species == "egg"){
        eggs.push_back(new Egg( inFront(hero->global, hero->direction, 200), image->egg() ) );
        colliders.push_back( eggs.back() );
        friendlies.push_back( new Friendly( eggs.back()->global, image->friendly() ) );
        colliders.push_back( friendlies.back() );
        oscManager->newFriendly();
        eggLastSeen = 0;
        
//        delete any plankton that are inside the egg
        for( vector<Plankton*>::iterator p = plankton.begin(); p < plankton.end(); ){
            if( dist( (*p)->global, eggs.back()->global ) < eggs.back()->radius ){
                delete *p;
                p = plankton.erase(p);
            } else {
                ++p;
            }
        }
    }
    
}





//Updates the global Offset value depending on the Hero's location
void EntityManager :: updateOffset(){
    
    vec2 offsetPos = hero->local;
    
    //if hero is inside an egg
    if(insideEgg == true){
        //find the egg that the player is inside
        for(int i = 0; i < eggs.size(); i++){
            if( dist(hero->global, eggs.at(i)->global) < eggs.at(i)->radius){
                //position that the 'camera' focuses on is the center of the egg
                offsetPos = eggs.at(i)->local;
            }
        }
    }

    //otherwise, the center of focus is the hero (with easing (0.03f) )
    float over = 0;
    
    //if the hero is near the floor, the camera stops following the hero completely, and hovers above the sea floor
    if(hero->global.y > -200){
        over = hero->global.y + 200;
    }
    offset += 0.03f * (offsetPos - vec2(getWindowWidth()/2, (getWindowHeight()/2)+over));

}


//FUNCTIONS

//finds a new location in front of a vector
vec2 EntityManager :: inFront(vec2 start, float direction, int inFrontBy){
    float x = start.x + sin(direction)*inFrontBy;
    float y = start.y + cos(direction)*inFrontBy;
    
    //make sure new location is within game boundaries
    if(y > 0){
        y = -100;
    };
    if(y < -6500){
        y = -6000;
    };
    
    vec2 newLoc = vec2(x,y);
    
    //make sure new entitiy is off screen
    while( dist(newLoc, start) < inFrontBy){
        x += rand(0,200);
        y += rand(0,200);
        newLoc = vec2(x,y);
    }
    
    return newLoc;
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

//returns true if a given location is far enough away from the hero (entity to be deleted)
bool EntityManager :: farFromHero( vec2 location ){
    return ( dist(hero->global, location) > 2000 );
}


//DRAWING

void EntityManager :: drawEntities(){
    
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    drawBackground();
    
    environment->draw();
    drawGrass();
    
    for(int i = 0; i < urchins.size();    i++){ urchins.at(i)->draw();   }
    for(int i = 0; i < starfish.size();   i++){ starfish.at(i)->draw();    }
    for(int i = 0; i < eggs.size();       i++){ eggs.at(i)->draw();     }
    for(int i = 0; i < plankton.size();   i++){ plankton.at(i)->draw(); }
    for(int i = 0; i < spores.size();     i++){ spores.at(i)->draw();   }
    for(int i = 0; i < jellies.size();    i++){ jellies.at(i)->draw();  }
    for(int i = 0; i < friendlies.size(); i++){ friendlies.at(i)->draw();  }
    for(int i = 0; i < sparks.size();     i++){ sparks.at(i)->draw();  }
    hero->draw();
    
    environment->drawMask();
}

//no "Grass" class because it's just a Feeler
void EntityManager :: drawGrass(){
    for(int i = 0; i < longGrass.size(); i++){
        gl::color( ColorA8u( 90,110,15, 200) );
        gl::draw( longGrass.at(i)->getPath() );
        vector<vec2> points = longGrass.at(i)->getPoints();
        gl::color( ColorA8u( 100,100,100, 100 ) );
        for(int n = 0; n < points.size(); n++){
            gl::drawSolidCircle( points.at(n), 2 );
        }
    }
}

void EntityManager :: drawBackground(){
    //background colour changes depending on X and Y position
    float widthMod = sin(hero->global.x*0.0001)*0.1;
    float depth = (-hero->global.y) / 7000;
    
    float r = depth*0.06 -widthMod;
    float g = depth*0.3 + widthMod;
    float b = depth*0.8;
    r+=0.006;
    g+=0.03;
    b+=0.08;
    
    gl::clear( Color( r,g,b ) );

}


