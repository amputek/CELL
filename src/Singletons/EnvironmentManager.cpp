#include "EnvironmentManager.hpp"

EnvironmentManager :: EnvironmentManager( Images* imgs ){
    image = imgs;
    mask = new Mask( image->maskImg );
    
    for( int i = 0; i < 30; i++)
    {
        float depth = rand(0.2, 3.0);
        int img = 0;
        if( depth < 0.8 || depth > 1.2 ) img = 1;
        if( depth < 0.5 || depth > 1.5 ) img = 2;
        
        vec2 pos = vec2(-400,-2000);
        pos.x += randFloat( -getWindowWidth() / 2, getWindowWidth() / 2 );
        pos.y += randFloat( -getWindowHeight() / 2, getWindowHeight() / 2 );
        bubbles.push_back( new Bubble( pos, vrand(300), depth, &image->bubbleImgs[ img ] ) );
    }
}


//create a splash at specified point
void EnvironmentManager :: splash(vec2 loc, int start, int end ){
    splashes.push_back( new Splash( loc, start, end, 1, &image->splashImg ) );
}

//create a number of bubbles, at random depths - used for eating plankton etc
void EnvironmentManager :: bubble( vec2 local, int amount){
    for(int i = 0; i < amount; i++){
        float depth = rand(0.7,1.3);
        vec2 loc = globalise( local, depth );
        
        int img = 0;
        if( depth < 0.8 ) img = 1;
        if( depth > 1.2 ) img = 1;
    
        bubbles.push_back( new Bubble( loc, vrand(300), depth, &image->bubbleImgs[ img ] ) );
    }
}


//Updates Bubbles, Beams, Floor, Surface and Splashes
void EnvironmentManager :: update( const vec2 & heroGlobal, const vec2 & heroLocal ){

    updateSplashes();
    updateBeams();
    updateBubbles();

    float depth = (((500 - heroGlobal.y) / 5500) + 0.2) * 3.0f;
    mask->update(heroLocal, depth );
    
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
            beams.push_back( new Beam( vec2(offset.x + rand(-1000,1000), 500), &image->beamImg ) );
        }
    }
    
    //Update/Delete existing beams
    for(vector<Beam*>::iterator p = beams.begin(); p != beams.end(); ){
        (*p)->update();
        if((*p)->alive() == false){
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
        float depth = rand(0.2,2.0);
        
        if( randFloat() < 0.1 )
        {
            depth = rand(0.2,3.5);
        }
        
        int img = 0;
        if( depth < 0.8 || depth > 1.2 ) img = 1;
        if( depth < 0.5 || depth > 1.5 ) img = 2;
        
        
        bubbles.push_back( new Bubble( globalise( vec2(rand(-100,getWindowWidth()+100), getWindowHeight()+100), depth ), vrand(60), depth, &image->bubbleImgs[ img ] ) );
    }
    
    //Update/Delete existing bubbles
    for(vector<Bubble*>::iterator p = bubbles.begin(); p != bubbles.end(); ){
        (*p)->update();
        (*p)->addVelocity( vec2( randFloat(-1.5f,1.5f), randFloat(-150.0f,-100.0f) * deltaTime) );
        if((*p)->alive() == false){
            delete *p;
            p = bubbles.erase(p);
        } else {
            ++p;
        }
    }
}


    
void EnvironmentManager :: draw(){

    gl::ScopedBlendAdditive additive;
    for(int i = 0; i < beams.size();      i++){ beams.at(i)->draw();      }
    for(int i = 0; i < bubbles.size();    i++){ bubbles.at(i)->draw();    }
    for(int i = 0; i < splashes.size();   i++){ splashes.at(i)->draw();   }
    
}

void EnvironmentManager :: drawMask(){
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    mask->draw();
}
