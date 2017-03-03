//
//  EntityGenerator.cpp
//  Cell
//
//  Created by Rob Dawson on 19/02/2017.
//
//

#include "EntityGenerator.hpp"




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
vec2 EntityGenerator :: inFront( int inFrontBy){
    
    float distToEdgeScreen = 3000.0f;
    
    
    float x = hero->global.x + sin(hero->direction) * 2000;
    float y = hero->global.y + cos(hero->direction) * 2000;
    
    
    vec2 startGlobal = vec2( hero->global.x,  hero->global.y);
    vec2 endGlobal = vec2(x,y);
    
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
    
    x = hero->global.x + sin(hero->direction) * distToEdgeScreen;
    y = hero->global.y + cos(hero->direction) * distToEdgeScreen;
    
    return vec2(x,y);
}


//Generate a group of plaknton
void EntityGenerator::generatePlankton( vector<Plankton*> * plankton, int planktonType, const vec2 & position )
{
    plankton->push_back( new Plankton( position, &image->planktonImgs[planktonType], planktonType ) );
}

void EntityGenerator::generatePlankton( vector<Plankton*> * plankton, vector<Egg*> * eggs )
{
    
    if( getElapsedFrames() % planktonFrequency != 0 ) return;
    if( plankton->size() > 40) return;
    

    
    //position of new plankton in front of the hero
    vec2 clusterPosition = inFront(rand(100,400));
    
    //make sure plankton do not spawn inside egg
    for(int i = 0; i < eggs->size();i++)
        if(dist(clusterPosition, eggs->at(i)->global) < eggs->at(i)->radius + 50)
            return;
    
    
    int planktonCount = randInt(1,8);
    
    for(int i = 0; i < planktonCount; i++){
        int planktonType = randInt(0,5);
        vec2 planktonPosition = clusterPosition + vrand(100);
        generatePlankton( plankton, planktonType, planktonPosition );
    }
    
}



void EntityGenerator::generateUrchin( vector<Urchin*> * urchins, const vec2 & urchinPosition )
{

    urchins->push_back( new Urchin( urchinPosition, &image->urchinImg ) );
    
    colliders->push_back( urchins->back() );
    
    urchinLastSeen = 0;
}

void EntityGenerator::generateUrchin( vector<Urchin*> * urchins )
{
    //URCHIN - only deep sea
    if(urchinLastSeen < urchinFrequency || hero->global.y < -1000) return;
    
    generateUrchin( urchins, inFront(200) );
}


void EntityGenerator::generateStarfish( vector<Starfish*> * starfish, const vec2 & position )
{
    starfish->push_back( new Starfish( position, &image->splashImg ) );
    colliders->push_back( starfish->back() );
    starLastSeen = 0;
}

void EntityGenerator::generateStarfish( vector<Starfish*> * starfish )
{
    //STARFISH - not shallow waters
    
    if( starLastSeen < starFrequency ) return;
    if( hero->global.y > -3000 ) return;
    
    generateStarfish( starfish, inFront(400) );
    
}


void EntityGenerator::generateJellyfish( vector<Jelly*> * jellies, int type, const vec2 & position )
{
    jellies->push_back( new Jelly( position, type, &image->jellyImgs[ type ] ) );
    
    colliders->push_back( jellies->back() );
    
    jellyLastSeen = 0;
}

void EntityGenerator::generateJellyfish( vector<Jelly*> * jellies )
{
    //JELLYFISH - anywhere
    if(jellyLastSeen < jellyFrequency) return;
    
    generateJellyfish( jellies, randInt(0,3), inFront(300) );
}

void EntityGenerator::generateSpores( vector<Spore*> * spores, int sporeType, const vec2 & clusterPosition )
{
    //SPORES - anywhere, more common (every 600 frames)

    int clusterSize = rand(10,20);
    float startDepth = randFloat(0.4,0.6);
    float endDepth = 1.1;
    float depthDelta = (endDepth - startDepth) / clusterSize;
    

    for(int i = 0; i < clusterSize; i++){
        
        vec2 sporePosition = clusterPosition + vrand(100);
        
        float sporeDepth = startDepth + (i * depthDelta);
        
        
        int blurLevel = 2;
        if( sporeDepth > 0.5) blurLevel = 1;
        if( sporeDepth > 0.8) blurLevel = 0;
                
        if( sporeDepth > 0.8 )
        {
            bool spaceFound = false;
            while( !spaceFound )
            {
                spaceFound = true;
                
                for(vector<Spore*>::iterator it = spores->begin(), end = spores->end(); it != end; ++it)
                {
                    if( (*it)->depth > 0.8 )
                    {
                        if( glm::distance( (*it)->global, sporePosition ) < (*it)->radius * 2.0f )
                        {
                            spaceFound = false;
                            sporePosition += vrand(20.0f);
                            break;
                        }
                    }
                }
            }
        }

        spores->push_back(new Spore(sporePosition, sporeDepth, sporeType, &image->sporeImgs[sporeType][blurLevel] ) );
    }
    
    
    sporeLastSeen = 0;

}

void EntityGenerator::generateSpores( vector<Spore*> * spores )
{
    if(sporeLastSeen < sporeFrequency ) return;
    generateSpores( spores, randInt(0,3), inFront(800) );
}





bool EntityGenerator::generateEgg( vector<Egg*> * eggs, vector<Friendly*> * friendlies, vector<Plankton*> * plankton, bool withFriendly, const vec2 & eggPosition )
{
    eggs->push_back(new Egg( eggPosition, &image->eggImg ) );
    
    colliders->push_back( eggs->back() );
    
    if( withFriendly )
    {
        friendlies->push_back( new Friendly( eggs->back()->global, &image->friendlyImg ) );
        colliders->push_back( friendlies->back() );
    }

    //delete any plankton that are inside the egg
    for( vector<Plankton*>::iterator p = plankton->begin(); p < plankton->end(); ){
        if( dist( (*p)->global, eggs->back()->global ) < eggs->back()->radius ){
            delete *p;
            p = plankton->erase(p);
        } else {
            ++p;
        }
    }
    
    eggLastSeen = 0;
    
    return true;

}

bool EntityGenerator::generateEgg( vector<Egg*> * eggs, vector<Friendly*> * friendlies, vector<Plankton*> * plankton )
{
    //EGG
    if(eggLastSeen < eggFrequency) return false;
    return generateEgg( eggs, friendlies, plankton, true, inFront(800) );
}


