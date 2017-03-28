#include "EnvironmentManager.hpp"



EnvironmentManager :: EnvironmentManager( CellRenderer * r, vector<GameObject*> * e ) : entities(e), renderer(r) {

    mask = new Mask();
    renderer->setupMask( mask->mFbo );
    
    for( int i = 0; i < 30; i++)
    {
        float depth = randFloat(0.2, 3.0);
        int img = 0;
        if( depth < 0.8 || depth > 1.2 ) img = 1;
        if( depth < 0.5 || depth > 1.5 ) img = 2;
        
        vec2 pos = vec2(-400,-2000);
        pos.x += randFloat( -getWindowWidth() / 2, getWindowWidth() / 2 );
        pos.y += randFloat( -getWindowHeight() / 2, getWindowHeight() / 2 );
        entities->push_back( new Bubble( pos, vrand(300), depth ) );
    }
    
    for( int i = 0 ; i < 5; i ++)
    {
        entities->push_back( new Beam( vec2(offset.x - (getWindowWidth()/2) + randFloat(-600,600), 500) ) );
    }
    
}


//create a splash at specified point
void EnvironmentManager :: splash( const vec2 & loc, int start, int end ){
    entities->push_back( new Splash( loc, start, end, 1  ) );
}

//create a number of bubbles, at random depths - used for eating plankton etc
void EnvironmentManager :: bubble( const vec2 & global, int amount){
    
    vec2 local = renderer->toLocal( global, 1 );
    
    for(int i = 0; i < amount; i++){
        float depth = randFloat(0.7,1.3);
        vec2 loc = globalise( local, depth );
        entities->push_back( new Bubble( loc, vrand(300), depth ) );
    }
}


//Updates Bubbles, Beams, Floor, Surface and Splashes
void EnvironmentManager :: update( const vec2 & heroPosition ){

    float depth = (((500 - heroPosition.y) / 5500) + 0.2) * 3.0f;
    mask->update( renderer->toLocal(heroPosition,1.0), depth );
    
}

void EnvironmentManager :: drawMask(){
    renderer->drawDust();
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    mask->draw( *renderer );
}
