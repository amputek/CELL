#include "EntityManager.hpp"


int Starfish::SEENCOUNT = 1;
int Jelly::SEENCOUNT = 2;
int Urchin::SEENCOUNT = 3;
int Egg::SEENCOUNT = 1800;
int Friendly::SEENCOUNT = 5;
int Spore::SEENCOUNT = 6;

int Starfish::ENTITY_COUNT = 0;
int Jelly::ENTITY_COUNT = 0;
int Urchin::ENTITY_COUNT = 0;
int Egg::ENTITY_COUNT = 0;
int Friendly::ENTITY_COUNT = 0;
int Spore::ENTITY_COUNT = 0;
int Plankton::ENTITY_COUNT = 0;
int Spark::ENTITY_COUNT = 0;
int Bubble::ENTITY_COUNT = 0;
int Beam::ENTITY_COUNT = 0;

int Spore::sparksSpawned = 0;

//Constructor initialises the hero, the floor and surface, and some starting entities
EntityManager :: EntityManager( CellRenderer * img ){
    
    //set up OSC
    oscManager = new OSCManager();
    oscManager->entities = this;
    
    gameObjects = new vector<GameObject*>();
    cout << "Init: " << &gameObjects << endl;

    //set up managers, singletons
    renderer = img;
    environment = * new EnvironmentManager( renderer, gameObjects );
    
    //first entities: the hero (player) and the starting egg
    hero = new Player(vec2(-400,-1500) );
  //  offset = hero->global;
//    
//    
    entityGenerator = * new EntityGenerator( gameObjects );
    entityGenerator.hero = hero;
    entityGenerator.image = renderer;
    

    
    gameObjects->push_back( hero );
  //  gameObjects.push_back(new Spark( vec2(-400,-2000), SporeType::ORANGE ) );


 
    for(int i = 0; i < 20; i++)
    {
        vec2 pos = vec2(-400,-2000);
        pos.x += randFloat( -getWindowWidth() / 2, getWindowWidth() / 2 );
        pos.y += randFloat( -getWindowHeight() / 2, getWindowHeight() / 2 );
        
        if( pos.x > -150 || pos.x < -650 || pos.y > -1850 || pos.y < -2150 )
        {
            entityGenerator.generatePlankton( randInt(0,5), pos );
        }
    }
    
   

    pulseEvents = new vector<PulseEvent>();
    

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

    entityGenerator.generate();

    
    //Update
    
    for( vector<GameObject*>::iterator itEntity = gameObjects->begin(); itEntity < gameObjects->end(); ++itEntity )
    {
        IDynamic * isDyanmic = dynamic_cast<IDynamic*>( *itEntity );
        if( isDyanmic ) isDyanmic->update();
    }

    
    //Handle Events from OSC
    
    for( vector<PulseEvent>::iterator itPulse = pulseEvents->begin(); itPulse < pulseEvents->end(); ++itPulse )
    {
        PulseEvent pulse = *itPulse;
        int count = 0;
        for( vector<GameObject*>::iterator itEntity = gameObjects->begin(); itEntity < gameObjects->end(); ++itEntity )
        {
            GameObject * ptrEntity = *itEntity;
            if( ptrEntity->mType == pulse.entityType )
            {
                if( count == (*itPulse).index )
                {
                    environment.splash( ptrEntity->getPosition(), ptrEntity->getSize(), ptrEntity->getSize() * 2 );
                    if( pulse.entityType == SPARK ) static_cast<Spark*>(ptrEntity)->pulse();
                    break;
                }
                
                count++;
                
            }
        }
    }
    
    //Clear events list
    pulseEvents->clear();

    
    //Collisions...
    
    for( vector<GameObject*>::iterator itEntity = gameObjects->begin(); itEntity < gameObjects->end(); ++itEntity )
    {
        ICollideable * isCollideable = dynamic_cast<ICollideable*>( *itEntity );
        if( isCollideable ) isCollideable->collide( *gameObjects, hero, environment, *oscManager );
    }
    



    //Delete...
        
    for( vector<GameObject*>::iterator itEntity = gameObjects->begin(); itEntity < gameObjects->end(); )
    {
        GameObject * ptrEntity = *itEntity;
        
        ptrEntity->offScreenBy( renderer );
        
        //if entity needs to be deleted (eg through being eaten, or being too far off-screen), delete
        if(ptrEntity->mDeleteMe)
        {
            delete ptrEntity;
            itEntity = gameObjects->erase( itEntity );
        } else {
            ++itEntity;
        }
    }

    


   
    updateOffset( );
    environment.update( hero->getPosition() );

    

    
}

//UPDATE METHODS FOR ENTITY COLLECTIONS

void EntityManager :: updateHero( const vec2 & mousePos, bool canMove ){

    
    
    
    hero->setDestination( globalise( canMove ? mousePos : vec2( getWindowWidth() / 2, getWindowHeight() / 2 - 50), 1 ) );
//    hero->update();

    oscManager->setDepth( (-hero->getPosition().y) / 7000 );
    
    //constantly make splashes around hero
    environment.splash( hero->getPosition(), hero->getSize() * 1.8, hero->getSize() * 2.4);

    if(hero->levelling()){
        environment.splash(hero->getPosition(), hero->getSize() * 1.8, randFloat(hero->getSize() * 2.4, hero->getSize() * 10.0f) );
    }
}



//Updates the global Offset value depending on the Hero's location
void EntityManager :: updateOffset(){
    
    
    if( getElapsedFrames() < 400 ) return;

    vec2 offsetPos = renderer->toLocal(hero->getPosition(),1);
    
    for( vector<GameObject*>::iterator itEntity = gameObjects->begin(); itEntity < gameObjects->end(); ++itEntity )
    {
        GameObject * ptrEntity = *itEntity;
        if( ptrEntity->mType == EGG )
        {
            if( static_cast<Egg*>( ptrEntity )->playerIsInside() )
            {
                offsetPos = renderer->toLocal( ptrEntity->getPosition(), 1);
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



//DRAWING

void EntityManager :: drawEntities()
{
    
    gl::enableAlphaBlending();

    drawBackground();
    
    
    string entityString = "";
    
    
    for( vector<GameObject*>::iterator entity = gameObjects->begin(); entity < gameObjects->end(); ++entity ){
        
        entityString = entityString + to_string((*entity)->mType);
        
        IDrawable * drawableEntity = dynamic_cast<IDrawable*>(*entity);
        if( drawableEntity )
        {
            if( (*entity)->getPosition() == vec2(NAN,NAN) ) continue;
            drawableEntity->draw( *renderer );
        }
    }
    
    
    
    gl::color(1,1,1);
    gl::drawString( "Plankton: " + to_string(Plankton::ENTITY_COUNT), vec2( 10.0f, 10.0f ) );
    gl::drawString( "Jellyfish: " + to_string(Jelly::ENTITY_COUNT), vec2( 10.0f, 25.0f ) );
    gl::drawString( "Starfish: " + to_string(Starfish::ENTITY_COUNT), vec2( 10.0f, 40.0f ) );
    gl::drawString( "Urchin: " + to_string(Urchin::ENTITY_COUNT), vec2( 10.0f, 55.0f ) );
    gl::drawString( "Friendly: " + to_string(Friendly::ENTITY_COUNT), vec2( 10.0f, 70.0f ) );
    gl::drawString( "Spark: " + to_string(Spark::ENTITY_COUNT), vec2( 10.0f, 85.0f ) );
    gl::drawString( "Spore: " + to_string(Spore::ENTITY_COUNT), vec2( 10.0f, 100.0f ) );
    gl::drawString( "Egg: " + to_string(Egg::ENTITY_COUNT), vec2( 10.0f, 115.0f ) );
    

    
}


void EntityManager :: drawBackground(){

    //background colour changes depending on X and Y position
    float widthMod = sin(hero->getPosition().x*0.0001)*0.1;
    float depth = (-hero->getPosition().y) / 7000;
    
    float r = 0.006 + (depth * 0.06 -widthMod);
    float g = 0.03 + (depth * 0.3 + widthMod);
    float b = 0.08 + (depth * 0.8);

    gl::clear( Color( r,g,b ) );

}


