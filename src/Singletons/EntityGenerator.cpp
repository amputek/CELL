#include "EntityGenerator.hpp"
#include "EntityManager.hpp"

float EntityGenerator ::lineSegmentIntersection(const vec2 &start1, const vec2 &end1, const vec2 &start2, const vec2 &end2 )
{
    float a1 = end1.y - start1.y;
    float b1 = start1.x - end1.x;
    float a2 = end2.y - start2.y;
    float b2 = start2.x - end2.x;
    float det = a1*b2 - a2*b1;
    if(det == 0.0f) return 0.0f;
    float c1 = a1*start1.x+b1*start1.y;
    float c2 = a2*start2.x+b2*start2.y;
    vec2 intersection = vec2( (b2*c1-b1*c2)/det , (a1*c2-a2*c1)/det );
    return glm::distance( vec2(start1.x, start1.y), intersection );
}


//finds a new location in front of a vector
vec2 EntityGenerator :: inFront( int inFrontBy, float randomiseAmount ){
    
    float distToEdgeScreen = 3000.0f;
    
    vec2 direction = glm::normalize(hero->getDestination() - hero->getPosition());
    direction = glm::normalize( direction + randVec2() * randomiseAmount );

    vec2 startGlobal = vec2( hero->getPosition().x,  hero->getPosition().y);
    vec2 endGlobal = hero->getPosition() + direction * 2000.0f;
    
    vec2 startLocal = startGlobal - offset;
    vec2 endLocal = endGlobal - offset;
    
    startLocal += getWindowSize() / 2;
    endLocal += getWindowSize() / 2;
    
    float width = ci::app::getWindowWidth();
    float height = ci::app::getWindowHeight();
    
    float a = lineSegmentIntersection( startLocal, endLocal, vec2(0    ,0)     , vec2(width,0     ) );
    if( a < distToEdgeScreen ) distToEdgeScreen = a;
    float b = lineSegmentIntersection( startLocal, endLocal, vec2(width,0)     , vec2(width,height) );
    if( b < distToEdgeScreen ) distToEdgeScreen = b;
    float c = lineSegmentIntersection( startLocal, endLocal, vec2(width,height), vec2(0    ,height) );
    if( c < distToEdgeScreen ) distToEdgeScreen = c;
    float d = lineSegmentIntersection( startLocal, endLocal, vec2(0    ,height), vec2(0    ,0     ) );
    if( d < distToEdgeScreen ) distToEdgeScreen = d;
    
    distToEdgeScreen += inFrontBy;
    
    return hero->getPosition() + direction * distToEdgeScreen;
}


//Generate a group of plaknton
void EntityGenerator::generatePlankton( int planktonType, const vec2 & position )
{
    entities->push_back( new Plankton( position, planktonType ) );
}

void EntityGenerator::generatePlankton( )
{
    
    if( getElapsedFrames() % Plankton::SPAWN_FREQUENCY != 0 ) return;
    if( Plankton::ENTITY_COUNT > 40) return;
    

    //position of new plankton in front of the hero
    vec2 clusterPosition = inFront(randFloat(100,400), 0.4f);
    

    int planktonCount = randInt(1,8);
    
    for(int i = 0; i < planktonCount; i++){
        int planktonType = randInt(0,5);
        vec2 planktonPosition = clusterPosition + vrand(100);
        generatePlankton(planktonType, planktonPosition );
    }
    
}



void EntityGenerator::generateUrchin( const vec2 & urchinPosition )
{
    entities->push_back( new Urchin( urchinPosition ) );
}

void EntityGenerator::generateUrchin()
{
    //URCHIN - only deep sea
    if(Urchin::TIME_SINCE_ON_SCREEN < Urchin::SPAWN_FREQUENCY || hero->getPosition().y < -1000 || Urchin::ENTITY_COUNT > 0 ) return;
    generateUrchin( inFront(Urchin::SPAWN_OFF_SCREEN_BY, 0.2f ) );
}


void EntityGenerator::generateStarfish(  const vec2 & position )
{
    entities->push_back( new Starfish( position ) );
}

void EntityGenerator::generateStarfish( )
{
    //STARFISH - not shallow waters
    if( Starfish::TIME_SINCE_ON_SCREEN < Starfish::SPAWN_FREQUENCY || Starfish::ENTITY_COUNT > 2 ) return;
    generateStarfish(inFront(Starfish::SPAWN_OFF_SCREEN_BY, 0.2f) );
}


void EntityGenerator::generateJellyfish( int type, const vec2 & position )
{
    entities->push_back( new Jelly( position, type ) );
}

void EntityGenerator::generateJellyfish( )
{
    if(Jelly::TIME_SINCE_ON_SCREEN < Jelly::SPAWN_FREQUENCY || Jelly::ENTITY_COUNT > 0 ) return;
    generateJellyfish( randInt(0,3), inFront(Jelly::SPAWN_OFF_SCREEN_BY, 0.2f) );
}


void EntityGenerator::generateSpores( int sporeType, const vec2 & clusterPosition )
{
    //SPORES - anywhere, more common (every 600 frames)

    int clusterSize = randFloat(10,20);
    float startDepth = randFloat(0.4,0.6);
    float endDepth = 1.1;
    float depthDelta = (endDepth - startDepth) / clusterSize;
    

    for(int i = 0; i < clusterSize; i++){
        
        vec2 sporePosition = clusterPosition + vrand(100);
        
        float sporeDepth = startDepth + (i * depthDelta);
        
        if( sporeDepth > 0.8 )
        {
            bool spaceFound = false;
            while( !spaceFound )
            {
                spaceFound = true;
                
//                for(vector<GameObject*>::iterator it = gameObjects->begin(), end = gameObjects->end(); it != end; ++it)
//                {
//                    if( (*it)->getDepth() > 0.8 )
//                    {
//                        if( glm::distance( (*it)->getPosition(), sporePosition ) < (*it)->getSize() * 2.0f )
//                        {
//                            spaceFound = false;
//                            sporePosition += vrand(20.0f);
//                            break;
//                        }
//                    }
//                }
            }
        }

        entities->push_back(new Spore(sporePosition, sporeDepth, SporeType(sporeType) ) );
    }
    
}

void EntityGenerator::generateSpores( )
{
    if(Spore::TIME_SINCE_ON_SCREEN < Spore::SPAWN_FREQUENCY ) return;
    generateSpores( randInt(0,3), inFront(800, 0.2f) );
}


bool EntityGenerator::generateEgg( bool withFriendly, const vec2 & eggPosition )
{
    entities->push_back( new Egg( eggPosition ) );

    if( withFriendly ) entities->push_back( new Friendly( eggPosition ) );

    return true;
}

bool EntityGenerator::generateEgg( )
{
    //EGG
    if(Egg::TIME_SINCE_ON_SCREEN < Egg::SPAWN_FREQUENCY || Egg::ENTITY_COUNT > 0) return false;
    return generateEgg( true, inFront(Egg::SPAWN_OFF_SCREEN_BY, 0.3f) );
}

void EntityGenerator::generateBeams()
{
    if(getElapsedFrames() % 40 == 0){
        if(Beam::ENTITY_COUNT < 25){
            entities->push_back( new Beam( vec2(offset.x + randFloat(-1000,1000), 500) ) );
        }
    }
}

void EntityGenerator::generateBubbles()
{
    if(getElapsedFrames() % 80 == 0){
        float depth = randFloat(0.2,2.0);
        if( randFloat() < 0.1 ) depth = randFloat(0.2,3.5);
        entities->push_back( new Bubble( globalise( vec2(randFloat(-100,getWindowWidth()+100), getWindowHeight()+100), depth ), vrand(60), depth ) );
    }
}

