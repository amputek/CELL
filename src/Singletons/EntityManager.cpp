#include "EntityManager.hpp"


int Starfish::TIME_SINCE_ON_SCREEN = 1;
int Jelly::TIME_SINCE_ON_SCREEN = 2;
int Urchin::TIME_SINCE_ON_SCREEN = 3;
int Egg::TIME_SINCE_ON_SCREEN = Egg::SPAWN_FREQUENCY - 100;
int Spore::TIME_SINCE_ON_SCREEN = 6;

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

    //set up managers, singletons
    renderer = img;
    environment = * new EnvironmentManager( renderer, gameObjects );
    
    //first entities: the hero (player) and the starting egg
    hero = new Player(vec2(-400,-4000) );
  
    entityGenerator = * new EntityGenerator( gameObjects );
    entityGenerator.hero = hero;
    entityGenerator.image = renderer;
    

    
    gameObjects->push_back( hero );

 
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

    if( getElapsedFrames() > 400 ) entityGenerator.generate();
    


    
    //Temporary collection of collideable-only entities
    vector<GameObject*> colliders;
    for( vector<GameObject*>::iterator itEntity = gameObjects->begin(); itEntity < gameObjects->end(); ++itEntity )
    {
        ICollideable * isCollideable = dynamic_cast<ICollideable*>( *itEntity );
        if( isCollideable ) colliders.push_back( *itEntity );
    }
    
    
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
        for( vector<GameObject*>::iterator itEntity = colliders.begin(); itEntity < colliders.end(); ++itEntity )
        {
            GameObject * ptrEntity = *itEntity;
         
            
            
            
            
            if( ptrEntity->mType == pulse.entityType )
            {
                
                if( pulse.entityType == FRIENDLY )
                {
                    if( static_cast<Friendly*>( ptrEntity )->id == pulse.index )
                    {
                        environment.splash( ptrEntity->getPosition(), ptrEntity->getSize(), ptrEntity->getSize() * 5 );
                    }
                }
                else
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
    }
    
    //Clear events list
    pulseEvents->clear();

    
    //Collisions...
    
    for( vector<GameObject*>::iterator itEntity = colliders.begin(); itEntity < colliders.end(); ++itEntity )
    {
        ICollideable * isCollideable = dynamic_cast<ICollideable*>( *itEntity );
        isCollideable->collide( &colliders, hero, environment, *oscManager );
        
        if( (*itEntity)->mType == SPORE )
        {
            Spore * spore = static_cast<Spore*>(*itEntity);
            if( spore->isDead() )
            {
                gameObjects->push_back(new Spark(spore->getPosition(), spore->getSporeType() ));
                oscManager->newSpark( spore->getSporeType()  );
                Spore::sparksSpawned++;
            }
        }
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

    hero->setDestination( globalise( canMove ? mousePos : vec2( getWindowWidth() / 2, getWindowHeight() / 2 + 50), 1 ) );

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

template< class T >
string printStats()
{
    return to_string(T::ENTITY_COUNT) + " - " + to_string(T::TIME_SINCE_ON_SCREEN) + " / " + to_string(T::SPAWN_FREQUENCY);
}

template< class T >
string printStatsSimple()
{
    return to_string(T::ENTITY_COUNT);
}


void EntityManager :: drawEntities()
{
    
    gl::enableAlphaBlending();

    
    float widthMod = sin(hero->getPosition().x*0.0001)*0.1;
    float depth = (-hero->getPosition().y) / 7000;
    float r = 0.006 + (depth * 0.06 -widthMod);
    float g = 0.03 + (depth * 0.3 + widthMod);
    float b = 0.08 + (depth * 0.8);
    
    gl::clear( Color( r,g,b ) );
    
    
    for( vector<GameObject*>::iterator entity = gameObjects->begin(); entity < gameObjects->end(); ++entity ){
        IDrawable * drawableEntity = dynamic_cast<IDrawable*>(*entity);
        
        if( drawableEntity )
        {
            if( (*entity)->getPosition() == vec2(NAN,NAN) ) continue;
            drawableEntity->draw( *renderer );
        }
    }
    
    
    environment.drawMask();
    

    

}

void EntityManager:: printEntityStats()
{
    gl::enableAlphaBlending();
    gl::color(1,1,1);
    gl::drawString( "Jellyfish: " + printStats<Jelly>(), vec2( 10.0f, 10.0f ) );
    gl::drawString( "Starfish: " + printStats<Starfish>(), vec2( 10.0f, 25.0f ) );
    gl::drawString( "Urchin: " + printStats<Urchin>(), vec2( 10.0f, 40.0f ) );
    gl::drawString( "Spore: " + printStats<Spore>(), vec2( 10.0f, 55.0f ) );
    gl::drawString( "Egg: " + printStats<Egg>(), vec2( 10.0f, 70.0f ) );
    gl::drawString( "Plankton: " + printStatsSimple<Plankton>(), vec2( 10.0f, 85.0f ) );
    gl::drawString( "Friendly: " + printStatsSimple<Friendly>(), vec2( 10.0f, 100.0f ) );
    gl::drawString( "Spark: " + printStatsSimple<Spark>(), vec2( 10.0f, 115.0f ) );
}


