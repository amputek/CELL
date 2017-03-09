#include "EntityManager.hpp"

//Constructor initialises the hero, the floor and surface, and some starting entities
EntityManager :: EntityManager( CellRenderer* img){
    
    //set up OSC
    oscManager = new OSCManager();
    oscManager->entities = this;
    
    //set up managers, singletons
    image = img;
    environment = new EnvironmentManager( image );
    
    //first entities: the hero (player) and the starting egg
    hero = new Player(vec2(-400,-1000) );
  //  offset = hero->global;
    
    
    entityGenerator = new EntityGenerator();
    entityGenerator->hero = hero;
    entityGenerator->colliders = &colliders;
    entityGenerator->image = image;
    
    
    for(int i = 0; i < 20; i++)
    {
        vec2 pos = vec2(-400,-2000);
        pos.x += randFloat( -getWindowWidth() / 2, getWindowWidth() / 2 );
        pos.y += randFloat( -getWindowHeight() / 2, getWindowHeight() / 2 );
        
        if( pos.x > -150 || pos.x < -650 || pos.y > -1850 || pos.y < -2150 )
        {
            entityGenerator->generatePlankton( &plankton, randInt(0,5), pos );
        }
    }
    
    
    entityGenerator->generateStarfish( &starfish, vec2(-400,-2000));
    
    colliders.push_back(hero);
    

    insideEgg = false;
}

void EntityManager :: quit(){
    oscManager->quit();
}


//Update loop called from the Main App class
//Checks for incoming messages from OSC
//Updates entities (environment and NPCs)
//and activates PCG
void EntityManager :: update( ){
    
    oscManager->recieveMessage();

    updatePlankton();
    updateUrchins();
    updateSparks();
    updateSpores();
    updateEggs();
    updateStarfish();
    updateJellies();
    updateFriendlies();

    
    updateOffset( );
    environment->update( hero->getPosition() );

    
    if( getElapsedFrames() < 400 ) return;
    
    entityGenerator->generatePlankton( &plankton, &eggs );
    entityGenerator->generateSpores( &spores );
    entityGenerator->generateUrchin( &urchins );
    entityGenerator->generateStarfish( &starfish );
    entityGenerator->generateJellyfish( &jellies );
    if( entityGenerator->generateEgg( &eggs, &friendlies, &plankton ) )
    {
        oscManager->newFriendly();
    }
}

//UPDATE METHODS FOR ENTITY COLLECTIONS

void EntityManager :: updateHero( const vec2 & mousePos, bool canMove ){

    
    if( getElapsedFrames() == 2 )
    {
        for(int i = 0; i < 25; i++)
        {
            hero->levelUp();
        }
    }

    
    hero->moveTo( canMove ? mousePos : vec2( getWindowWidth() / 2, getWindowHeight() / 2 - 50) );

    
    hero->update( deltaTime );
    
    //hero->global = globalise( mousePos, 1.0f );
    
    oscManager->setDepth( (-hero->getPosition().y) / 7000 );
    
    //constantly make splashes around hero
    environment->splash( hero->getPosition(), hero->getSize() * 1.8, hero->getSize() * 2.4);

    if(hero->levelling()){
        environment->splash(hero->getPosition(), hero->getSize() * 1.8, randFloat(hero->getSize() * 2.4, hero->getSize() * 10.0f) );
    }
}

void EntityManager :: updatePlankton(){
    
    for( vector<Plankton*>::iterator p = plankton.begin(); p < plankton.end(); ){
        
        //true if plankton should be deleted (either from being eaten, or from being too far from the hero)
        bool deleteThisPlankton = false;
        
        if( !image->onScreen( (*p)->getPosition(), (*p)->getDepth(), 400 ) ){
           
            deleteThisPlankton = true;       //if plankton is far enough away from hero, delete
        } else {
            
            //cycle through colliders, see if any have eaten the plankton
            int i = 0;
            while(!deleteThisPlankton && i < colliders.size()){
                
                //check if collider has eaten plankton
                if(dist( (*p)->getPosition(), colliders.at(i)->getPosition()) < (*p)->getSize() + colliders.at(i)->getSize() ){
                    
                    //do plankton eating business (bubbles, splashes, osc message)
                    environment->bubble( (*p)->getPosition(), 3);
                    environment->splash( (*p)->getPosition(), 1, 75 );
                    float pan = ( (*p)->getPosition().x - hero->getPosition().x);
                    oscManager->eatPlankton( (*p)->type(), pan , dist(hero->getPosition(), (*p)->getPosition()) );
                    
                    //if collider is the HERO ( collider #0 ) and not any other entity
                    if(i == 0){
                        //hero levels up etc
                        hero->incEaten( (*p)->type() );
                        if(hero->getEaten() % 20 == 0){
                            hero->levelUp();
                            environment->splash( hero->getPosition(), 0, 100 );
                            oscManager->eighthPlankton();
                        }
                    }
                    deleteThisPlankton = true;
                }
                i++;
            }
        }
        
        //if plankton needs to be deleted (through being eaten, or being too far away from hero), delete
        if(deleteThisPlankton){
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
        if( farFromHero( (*p)->getPosition() ) == true){
            //remove from collider list, then delete from urchin list and erase object
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = urchins.erase(p);
        } else {
            //Otherwise, go through standard update: collision, target, OSC messages
            (*p)->collide(hero->getPosition(), hero->getSize() + 10.0f);
            (*p)->update();
            (*p)->avoidColliders(&colliders);
            if((*p)->inSpace == true){
                if(plankton.size() > 0){
                    if(plankton.at(0)->getPosition().y > -1000 && dist(plankton.at(0)->getPosition(), (*p)->getPosition() ) < 800 ){
                        (*p)->setDestination( plankton.at(0)->getPosition() );
                    } else {
                        (*p)->setDestination( vec2((*p)->getPosition().x + randFloat(-100,100), randFloat(-100,100)) );
                    }
                }
            }
            
            //friendlies also cause collision
            for(int i = 0 ; i < friendlies.size(); i++){
                (*p)->collide( friendlies.at(i)->getPosition(), 30.0f );
            }
        
            //OSC message - distance and collision amount
            oscManager->urchin( dist( (*p)->getPosition(), hero->getPosition() ), (*p)->getContactAmount() );
            
            ++p;
        }
    }
    
    entityGenerator->urchinLastSeen++;

}

void EntityManager :: updateSparks(){
    for(int i = 0; i < sparks.size(); i++){
        
        //standard update pattern
        sparks.at(i)->update();
        sparks.at(i)->avoidColliders( &colliders );
        
        //interacts with urchin if near enough
        for(int n = 0; n < urchins.size(); n++){
            if( dist(sparks.at(i)->getPosition(), urchins.at(n)->getPosition()) < 100){
                sparks.at(i)->setDestination(urchins.at(n)->getPosition());
            }
        }
        
        //interacts with jellies if near enough
        for(int n = 0; n < jellies.size(); n++){
            if( dist(sparks.at(i)->getPosition(), jellies.at(n)->getPosition()) < 100){
                sparks.at(i)->setDestination(jellies.at(n)->getPosition() + vec2(0,randFloat(20,70)));
            }
        }
        
        //but if the player gets too far away, follow the player
        if( sparks.at(i)->inSpace == true){
            if( dist(sparks.at(i)->getPosition(), hero->getPosition()) > 50){
                sparks.at(i)->setDestination( hero->getPosition() );
            }
        }
        
        //sparks constantly create splashes
        environment->splash( sparks.at(i)->getPosition(), sparks.at(i)->getSize(), 10 );
    }
}

void EntityManager :: updateSpores(){
    
    for( vector<Spore*>::iterator p = spores.begin(); p < spores.end(); ){
        
        bool ended = false;
        
        if( farFromHero( (*p)->getPosition() ) == true ){
            //true if spore is ended for any reason (either via the player collision, or from being too far away from the player)
            ended = true;
        } else {
            
            (*p)->update();
            
            if( abs((*p)->getDepth() - 1.0) < 0.2f )
            {
                (*p)->collide(hero->getPosition());
            }
            
            //sound happens if there's any contact with the field
            if( (*p)->isInContact() ){
                environment->splash( (*p)->getPosition(), 22*(*p)->getDepth(), 30 );
                oscManager->sporeBoop( (*p)->getHealth() );
            }
            
            //reset field counter if the character is near enough to any field

            //checks if health is low enough
            if( !(*p)->isAlive() ){
                
                ended = true;
                
                //do spark-creating business (new Spark, bubbles, splashes, OSC message)
                sparks.push_back(new Spark((*p)->getPosition(), (*p)->getType()  ) );
                
                oscManager->newSpark( (*p)->getType() );
                colliders.push_back( sparks.back() );
                
                environment->bubble( (*p)->getPosition(), 6 );
                for(int i = 0; i < 10; i++){
                    environment->splash( (*p)->getPosition() + vrand(10), randFloat(5,20), randFloat(5,35) );
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
    
    entityGenerator->sporeLastSeen++;
}

void EntityManager :: updateEggs(){
    
    bool inside = false;
    
    for( vector<Egg*>::iterator p = eggs.begin(); p < eggs.end(); ){
        
        if( farFromHero( (*p)->getPosition() ) == true ){
            //delete if too far from hero
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = eggs.erase(p);
        } else {
            
            //update
            (*p)->update();
            (*p)->collide(hero->getPosition(), hero->getSize() + 15.0f);
            (*p)->setInside(hero->getPosition());
            
            //friendlies can also cause collision with egg
            for(int i = 0 ; i < friendlies.size(); i++){
                (*p)->collide( friendlies.at(i)->getPosition(), friendlies.at(i)->getSize() + 15.0f );
            }
            
            //change the global 'inside' state - true if the hero is inside ANY of the eggs
            if( (*p)->inside() == true ){
                inside = true;
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
    
    entityGenerator->eggLastSeen++;
    
}

void EntityManager :: updateJellies(){
    
    for( vector<Jelly*>::iterator j = jellies.begin(); j < jellies.end(); ){
        
        (*j)->update();
        (*j)->collide(hero->getPosition(), 30.0f);
        (*j)->setDestination( (*j)-> getPosition() );
        
        //jellyfish also collide with sparks
        for(int i = 0; i < sparks.size(); i++){
            (*j)->collide(sparks.at(i)->getPosition(), 30.0f);
        }
        
        oscManager->jelly( (*j)->contacts(), dist( (*j)->getPosition(), hero->getPosition()) );
        

        
        //delete if too far away
        if( farFromHero( (*j)->getPosition() ) == true ){
            removeFromColliders( (GameObject*)(*j) );
            delete *j;
            j = jellies.erase(j);
        } else {
            ++j;
        }
        
    }
    
    entityGenerator->jellyLastSeen++;
    
}

void EntityManager :: updateFriendlies(){
    
    int index = 0;
    for( std::vector<Friendly*>::iterator p = friendlies.begin(); p != friendlies.end(); ){
        
        (*p)->update();
        
        float distance = dist( (*p)->getPosition(), hero->getPosition() );
        
        if( (*p)->born() == false){
            
            //friendly is 'born' if hero is near enough
            if( distance < 40){
                (*p)->birth();
                oscManager->bornFriendly(index);
                (*p)->setDestination( (*p)->getPosition() + vrand(500) );
            }
            
        } else {
            
            //collision, AI
            (*p)->avoidColliders(&colliders);
            
            //move towards plankton #0
            if( plankton.size() > index ){
                (*p)->setDestination( plankton.at(index)->getPosition() );
            }
            
            //if friendly is near enough, send OSC messages
            if( distance < 1000){
                float pan = ( (*p)->getPosition().x - hero->getPosition().x);
                oscManager->updateFriendly( index, pan, distance );
            }
            
//            if( (*p)->onScreen() )
//            {
//                environment->splash( (*p)->global, (*p)->radius+4, (*p)->radius+16);
//            }
        }
        
        ++p;
        index++;
        
    }
}

void EntityManager :: updateStarfish(){
    
    for( vector<Starfish*>::iterator p = starfish.begin(); p < starfish.end(); ){
        
        if( farFromHero( (*p)->getPosition() ) == true ){
            //delete if too far away
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = starfish.erase(p);
            
        } else {
            
            //update standardly
            (*p)->collide(hero->getPosition());
            (*p)->update();
            
            //chord change if active
            if( (*p)->reachedContactThreshold() ){
                oscManager->changeChord();
                environment->splash( (*p)->getPosition(), 20.0f, 100.0f );
                environment->bubble( (*p)->getPosition(), 5 );
            }
 
            
            //periodically creates bubbles
            if( (*p)->isFleeing() )
            {
                if( getElapsedFrames() % 15 == 0) environment->bubble( (*p)->getPosition(), 1 );
                if( getElapsedFrames() % 3  == 0) environment->splash( (*p)->getPosition(), 30.0f, 40.0f);
            }
            
            ++p;
        }
    }
    
    entityGenerator->starLastSeen++;
    
}




//RECIEVE FROM OSC MANAGER (SUPERCOLLIDER)
void EntityManager :: pulse(string species, int index){
    if(species == "friendly"){
        environment->splash( friendlies.at(index)->getPosition(), 10, 30 );
    }
    if(species == "spark"){
        sparks.at(index)->pulse();
        environment->splash( sparks.at(index)->getPosition(), 10, 25 );
    }
    if(species == "urchin"){
        environment->splash( urchins.at(index)->getPosition(), urchins.at(index)->getSize(), 25 );
    }
}





//Updates the global Offset value depending on the Hero's location
void EntityManager :: updateOffset(){
    
    
    if( getElapsedFrames() < 400 ) return;

    vec2 offsetPos = image->toLocal(hero->getPosition(),1);
    
    //if hero is inside an egg
    if(insideEgg == true){
        //find the egg that the player is inside
        for(int i = 0; i < eggs.size(); i++){
            if( dist(hero->getPosition(), eggs.at(i)->getPosition()) < eggs.at(i)->getSize() ){
                //position that the 'camera' focuses on is the center of the egg
                offsetPos = image->toLocal(eggs.at(i)->getPosition(), 1);
            }
        }
    }


    float surfaceMod = 1.0f;
    
    
    if( hero->getPosition().y < -6600 )
    {
        surfaceMod = 1.0f + ( hero->getPosition().y + 6600 ) * 0.005f;
        if( surfaceMod < 0.0f ) surfaceMod = 0.0f;
    }
    else if( hero->getPosition().y > -350 )
    {
        surfaceMod = 1.0f - ( hero->getPosition().y + 350 ) * 0.005f;
        if( surfaceMod < 0.0f ) surfaceMod = 0.0f;
    }

    vec2 halfWindowSize =  vec2(getWindowWidth()/2, getWindowHeight()/2);
    
    offset += deltaTime * 2.0f * (offsetPos - halfWindowSize) * vec2(1,surfaceMod);

    if( offset.y < -7000 ) offset.y = -7000;
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
bool EntityManager :: farFromHero( const vec2 & location ){
    return ( dist(hero->getPosition(), location) > 2000 );
}


//DRAWING

void EntityManager :: drawEntities(){
    
    

    gl::enableAlphaBlending();

    
    drawBackground();
    
    environment->draw();
    

    gl::enableAlphaBlending();
    for(int i = 0; i < spores.size();     i++){ spores.at(i)->draw( image );   }

    gl::enableAdditiveBlending();
    for(int i = 0; i < urchins.size();    i++){ urchins.at(i)->draw( *image );   }
    for(int i = 0; i < starfish.size();   i++){ starfish.at(i)->draw( image );    }


    for(int i = 0; i < plankton.size();   i++){ plankton.at(i)->draw( *image ); }

    for(int i = 0; i < jellies.size();    i++){ jellies.at(i)->draw( *image );  }
    
    for(int i = 0; i < friendlies.size(); i++){ friendlies.at(i)->draw( *image );  }
    
    for(int i = 0; i < sparks.size();     i++){ sparks.at(i)->draw( *image );  }

    hero->draw( *image );

    for(int i = 0; i < eggs.size();       i++){ eggs.at(i)->draw( *image );     }
    
}


void EntityManager :: drawBackground(){
    //background colour changes depending on X and Y position
    float widthMod = sin(hero->getPosition().x*0.0001)*0.1;
    float depth = (-hero->getPosition().y) / 7000;
    
    float r = depth*0.06 -widthMod;
    float g = depth*0.3 + widthMod;
    float b = depth*0.8;
    r+=0.006;
    g+=0.03;
    b+=0.08;
    
    gl::clear( Color( r,g,b ) );

}


