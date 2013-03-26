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
    hero = new Player(Vec2f(-400,-1000), image->player() );
    offset = hero->global;
    
 //   eggs.push_back( new Egg(Vec2f(-200,-1000), image->egg() ) );
    colliders.push_back(hero);
//    colliders.push_back( eggs.back() );
//    
//    friendlies.push_back( new Friendly( Vec2f(-200,-1000), image->friendly() ) );
//    colliders.push_back( friendlies.back() );
//    oscManager->newFriendly();

    //initialise counters
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
        eggs.push_back(new Egg( hero->global, image->egg() ) );
        colliders.push_back( eggs.back() );
        friendlies.push_back( new Friendly( eggs.back()->global, image->friendly() ) );
        colliders.push_back( friendlies.back() );
        oscManager->newFriendly();
        eggLastSeen = 0;
    }
    
    if(species == "jelly"){
        int type = irand(0,3);
        cout << "Creating new Jelly" << type << "\n";
        
        jellies.push_back( new Jelly( hero->global, type, image->jelly( type ) ) );
        colliders.push_back( jellies.back() );
        jellyLastSeen = 0;
    }
    
    if(species == "spores"){
        int r = irand(0,3);
        Vec2f loc = hero->global;
        
        for(int i = 0; i < rand(5,30); i++){
            spores.push_back(new Spore(loc + vrand(100), rand(0.5,1.1), r, image->spore(r)) );
        }
        sporeLastSeen = 0;
    }
    
    if(species == "urchin"){
        cout << "Creating new Urchin" << "\n";
        urchins.push_back( new Urchin( hero->global, image->urchin() ) );
        colliders.push_back( urchins.back() );
        urchinLastSeen = 0;
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
    return ( dist(hero->global, location) > 2000 );
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
                if(dist( (*p)->global, colliders.at(i)->global) < (*p)->radius + hero->radius ){
                    environment->bubble( (*p)->local, 4);
                    environment->splash( (*p)->global, 1, 50 );
                    float pan = ( (*p)->global.x - hero->global.x);
                    oscManager->eatPlankton( (*p)->type(), pan , dist(hero->global, (*p)->global) );
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
            
            oscManager->urchin( dist( (*p)->global, hero->global ), (*p)->contactAmount() );
            
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
        sparks.at(i)->updateTarget( colliders );
        
        for(int n = 0; n < urchins.size(); n++){
            if( dist(sparks.at(i)->global, urchins.at(n)->global) < 100){
                sparks.at(i)->setDestination(urchins.at(n)->global);
            }
        }
        
        for(int n = 0; n < jellies.size(); n++){
            if( dist(sparks.at(i)->global, jellies.at(n)->global) < 100){
                sparks.at(i)->setDestination(jellies.at(n)->global + Vec2f(0,rand(20,70)));
            }
        }
        
        if( sparks.at(i)->inSpace == true){
            if( dist(sparks.at(i)->global, hero->global) > 100){
                sparks.at(i)->setDestination( hero-> global );
            }
        }
        
        environment->splash( sparks.at(i)->global, sparks.at(i)->radius, 10 );
    }
}

void EntityManager :: updateEggs(){
    
    bool inside = false;
    
    for( vector<Egg*>::iterator p = eggs.begin(); p < eggs.end(); ){
        
        if( farFromHero( (*p)->global ) == true ){
            removeFromColliders((GameObject*)(*p));
            delete *p;
            p = eggs.erase(p);
        } else {
            (*p)->update();
            (*p)->collide(hero->global);
            (*p)->setInside(hero->global);
            
            for(int i = 0 ; i < friendlies.size(); i++){
                (*p)->collide( friendlies.at(i)->global );
            }
            
            if( (*p)->inside() == true ){
                inside = true;
            //    hero->slow();
            }
            
            if( (*p)->onScreen() == true){
                eggLastSeen = 0;
            }
            
            ++p;
        }
    }
    
    if(inside == true){
        if(insideEgg == false){
            insideEgg = true;
            oscManager->egg(1);
        }
    } else {
        if(insideEgg == true){
            insideEgg = false;
            oscManager->egg(0);
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
        
        
        for(int i = 0; i < sparks.size(); i++){
            (*j)->collide(sparks.at(i)->global);
        }
        
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
    
    int index = 0;
    for( std::vector<Friendly*>::iterator p = friendlies.begin(); p != friendlies.end(); ){
        
        (*p)->update();
        
        
        float distance = dist( (*p)->global, hero->global );
        
        if( (*p)->born() == false){
            
            if( distance < 40){
                (*p)->birth();
                oscManager->bornFriendly(index);
                (*p)->setDestination( (*p)->global + vrand(500) );
            }
            
            
        } else {
            
            (*p)->updateTarget(colliders);
            
            if( plankton.size() > 0){
                
                (*p)->setDestination( plankton.at(0)->global );
            }
            
            if( distance < 1000){
                float pan = ( (*p)->global.x - hero->global.x);
                oscManager->updateFriendly( index, pan, distance );
            }
        }
        
        ++p;
        index++;
        
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
            oscManager->grass( (*p)->contact() );
            ++p;
        }
    }
}

//Updates the global Offset value depending on the Hero's location
void EntityManager :: updateOffset(){

    float over = 0;
    if(hero->global.y > -200){
        over = hero->global.y + 200;
    }
    offset += 0.03f * (hero->local - Vec2f(getWindowWidth()/2, (getWindowHeight()/2)+over));

}

//Procedural Content Generation (PCG) method
void EntityManager :: entityGenerator(){
    
    //PLANKTON
    if(getElapsedFrames() % 40 == 0 && plankton.size() < 40){
        int index = irand(0,5);
        
        Vec2f loc = inFront(hero->global, hero->direction, rand(500,600));
        
        bool outsideEgg = true;
        for(int i = 0; i < eggs.size();i++){
            if(dist(loc, eggs.at(i)->global) < 60){
                outsideEgg = false;
            }
        }
        
        if(outsideEgg == true){
            for(int i = 0; i < irand(1,8); i++){
                plankton.push_back( new Plankton( loc + vrand(40),  image->plankton(index), index ) );
            }
        }
    }
    

    //LONG GRASS
    if(hero->global.y > -500 && longGrass.size() < 20 && rand(0.0,1.0) > 0.99){
        float nx = inFront(hero->global, hero->direction, 600).x;
        cout << "Creaing new Grass" << "\n";
        for(int i = 0; i < irand(3,6); i++){
            longGrass.push_back( new Feeler( Vec2f(nx + irand(-40,40), 0 ), irand(5,10), rand(0.9,1.15)) );
            longGrass.back()->update();
        }
    }
    
    
    if(urchinLastSeen >= 800 && hero->global.y > -1000){
        cout << "Creating new Urchin" << "\n";
        urchins.push_back( new Urchin( inFront(hero->global, hero->direction, 600), image->urchin() ) );
        colliders.push_back( urchins.back() );
        urchinLastSeen = 0;
    }
    
    if(starLastSeen >= 1800 && hero->global.y > -3000){
        cout << "Creating new Puff" << "\n";
        starfish.push_back( new Starfish( inFront(hero->global, hero->direction, 600) ) );
        colliders.push_back( starfish.back() );
        starLastSeen = 0;
    }
    
    if(jellyLastSeen == 1000){
        
        cout << "Creating new Jelly" << "\n";
        int type = irand(0,2);
        jellies.push_back( new Jelly( inFront(hero->global, hero->direction, 600), type, image->jelly( type ) ) );
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
    
    
    if(eggLastSeen == 1200){
        cout << "Creating new Egg" << "\n";
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


Vec2f EntityManager :: inFront(Vec2f start, float direction, int inFrontBy){
    float x = start.x + sin(direction)*inFrontBy;
    float y = start.y + cos(direction)*inFrontBy;
    if(y > 0){
        y = -100;
        x += 1000;
    };
    if(y < -6500){
        y = -6000;
        x -= 1000;
    };
    
    
    //make sure new entitiy is off screen
    Vec2f newLoc = Vec2f(x,y);
    while( dist(newLoc, hero->global) < 450){
        x += rand(-200,200);
        y += rand(-200,200);
        newLoc = Vec2f(x,y);
    }
    
    return newLoc;
}

//calls every entities' draw function
void EntityManager :: drawEntities(){
    
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    float widthMod = sin(hero->global.x*0.0001)*0.1;
    
    float depth = (-hero->global.y) / 7000;
        
    float r = depth*0.06 -widthMod;
    float g = depth*0.3 + widthMod;
    float b = depth*0.8;
    r+=0.006;
    g+=0.03;
    b+=0.08;
    
    gl::clear( Color( r,g,b ) );

    
    if(hero->global.y > -500){
    //    floor->draw();
    }
    
    environment->draw();
    drawGrass();
    
    for(int i = 0; i < urchins.size();   i++){ urchins.at(i)->draw();   }
    for(int i = 0; i < starfish.size();    i++){ starfish.at(i)->draw();    }
    for(int i = 0; i < eggs.size();     i++){ eggs.at(i)->draw();     }
    for(int i = 0; i < plankton.size(); i++){ plankton.at(i)->draw(); }
    for(int i = 0; i < spores.size();   i++){ spores.at(i)->draw();   }
    for(int i = 0; i < jellies.size();  i++){ jellies.at(i)->draw();  }
    for(int i = 0; i < friendlies.size();    i++){ friendlies.at(i)->draw();  }
    for(int i = 0; i < sparks.size();    i++){ sparks.at(i)->draw();  }
    hero->draw();

    
    environment->drawMask();
}


void EntityManager :: drawGrass(){
    for(int i = 0; i < longGrass.size(); i++){
        gl::color( ColorA8u( 90,110,15, 200) );
        gl::draw( longGrass.at(i)->getPath() );
        vector<Vec2f> points = longGrass.at(i)->getPoints();
        gl::color( ColorA8u( 100,100,100, 100 ) );
        for(int n = 0; n < points.size(); n++){
            gl::drawSolidCircle( points.at(n), 2 );
        }
    }
}