#include "EnvironmentManager.hpp"

EnvironmentManager :: EnvironmentManager( CellRenderer* imgs ){
    image = imgs;
    
    
    mask = new Mask();
    imgs->setupMask( mask->mFbo );
    
    for( int i = 0; i < 30; i++)
    {
        float depth = randFloat(0.2, 3.0);
        int img = 0;
        if( depth < 0.8 || depth > 1.2 ) img = 1;
        if( depth < 0.5 || depth > 1.5 ) img = 2;
        
        vec2 pos = vec2(-400,-2000);
        pos.x += randFloat( -getWindowWidth() / 2, getWindowWidth() / 2 );
        pos.y += randFloat( -getWindowHeight() / 2, getWindowHeight() / 2 );
        bubbles.push_back( new Bubble( pos, vrand(300), depth ) );
    }
}


//create a splash at specified point
void EnvironmentManager :: splash(vec2 loc, int start, int end ){
    splashes.push_back( new Splash( loc, start, end, 1  ) );
}

//create a number of bubbles, at random depths - used for eating plankton etc
void EnvironmentManager :: bubble( vec2 global, int amount){
    
    vec2 local = image->toLocal( global, 1 );
    
    for(int i = 0; i < amount; i++){
        float depth = randFloat(0.7,1.3);
        vec2 loc = globalise( local, depth );
        bubbles.push_back( new Bubble( loc, vrand(300), depth ) );
    }
}


//Updates Bubbles, Beams, Floor, Surface and Splashes
void EnvironmentManager :: update( const vec2 & heroGlobal){

    updateSplashes();
    updateBeams();
    updateBubbles();

    float depth = (((500 - heroGlobal.y) / 5500) + 0.2) * 3.0f;
    mask->update( image->toLocal(heroGlobal,1.0), depth );
    
}

void EnvironmentManager :: updateSplashes(){
    for( vector<Splash*>::iterator p = splashes.begin(); p != splashes.end(); ){
        (*p)->update();
        if((*p)->alive() == false){
            delete *p;
            p = splashes.erase(p);
        } else {
            ++p;
        }
    }
}

void EnvironmentManager :: updateBeams(){
    
    //Generate new beams
    if(getElapsedFrames() % 40 == 0){
        if(beams.size() < 25){
            beams.push_back( new Beam( vec2(offset.x + randFloat(-1000,1000), 500) ) );
        }
    }
    
    //Update/Delete existing beams
    for(vector<Beam*>::iterator p = beams.begin(); p != beams.end(); ){
        
        (*p)->update();
        vec2 pos = image->toLocal( (*p)->getPosition(), (*p)->getDepth() );
        
        if( (pos.x < -1000 || pos.x > 1800) )
        {
            delete *p;
            p = beams.erase(p);
        } else {
            ++p;
        }
    }
}

void EnvironmentManager :: updateBubbles(){
    
    //Generate new Bubbles
    if(getElapsedFrames() % 80 == 0){
        float depth = randFloat(0.2,2.0);
        
        if( randFloat() < 0.1 )
        {
            depth = randFloat(0.2,3.5);
        }
        
        bubbles.push_back( new Bubble( globalise( vec2(randFloat(-100,getWindowWidth()+100), getWindowHeight()+100), depth ), vrand(60), depth ) );
    }
    
    //Update/Delete existing bubbles
    for(vector<Bubble*>::iterator p = bubbles.begin(); p != bubbles.end(); ){
        (*p)->update();
        (*p)->addVelocity( vec2( randFloat(-1.5f,1.5f), randFloat(-150.0f,-100.0f) * deltaTime) );
        if( !image->onScreen( (*p)->getPosition(), (*p)->getDepth(), 1000 ) ){
            delete *p;
            p = bubbles.erase(p);
        } else {
            ++p;
        }
    }
}


    
void EnvironmentManager :: draw(){

    gl::ScopedBlendAdditive additive;
    for(int i = 0; i < beams.size();      i++){ beams.at(i)->draw( *image );      }
    for(int i = 0; i < bubbles.size();    i++){ bubbles.at(i)->draw( *image );    }
    for(int i = 0; i < splashes.size();   i++){ splashes.at(i)->draw( *image );   }
    
}

void EnvironmentManager :: drawMask(){
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    mask->draw( *image );
}
