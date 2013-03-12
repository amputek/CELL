#include "EnvironmentManager.h"

EnvironmentManager :: EnvironmentManager( Images* imgs ){
    image = imgs;
    mask = new Mask( *imgs->mask() );    
}



void EnvironmentManager :: splash(Vec2f loc, int start, int end ){
    splashes.push_back( new Splash( loc, start, end, 1) );
}

void EnvironmentManager :: bubble( Vec2f local, int amount){
    for(int i = 0; i < amount; i++){
        float depth = rand(0.8,1.2);
        Vec2f loc = globalise( local, depth );
        bubbles.push_back( new Bubble( loc, vrand(10), depth, image->bubble() ));
    }
}


//Updates Bubbles, Beams, Floor, Surface and Splashes
void EnvironmentManager :: update( Vec2f heroLoc ){
    
    //floor
//    if(hero->global.y > 100){
//        // floor->update(hero);
//    }
//    if(hero->global.y < -4000){
//        // surface->update(hero);
//    }
    
    updateSplashes();
    updateBeams();
    updateBubbles();
    
    //BUBBLES
    if(getElapsedFrames() % 40 == 0){
        bubble( Vec2f( rand(-400,400), 400 ), 2);
    }
    
    
    //BEAMS
    if(getElapsedFrames() % 20 == 0){
        if(beams.size() < 25){
            beams.push_back( new Beam( Vec2f(offset.x + rand(-1000,1000), 500), image->beam() ) );
        }
    }
    
    float depth = (((500 - heroLoc.y) / 5500) + 0.2) * 3.0;

    mask->update(offset.x - heroLoc.x, offset.y - heroLoc.y, depth );
    
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
    //light beams
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
    
    //bubbles
    for(vector<Bubble*>::iterator p = bubbles.begin(); p != bubbles.end(); ){
        (*p)->update();
        (*p)->global.y -= (*p)->radius * 0.3;
        if((*p)->alive() == false){
            delete *p;
            p = bubbles.erase(p);
        } else {
            ++p;
        }
    }
}

//calls every entities' draw function
void EnvironmentManager :: draw(){
    
    for(int i = 0; i < beams.size();      i++){ beams.at(i)->draw();      }
    //for(int i = 0; i < longGrass.size();  i++){ longGrass.at(i)->draw();  }
    for(int i = 0; i < bubbles.size();    i++){ bubbles.at(i)->draw();    }
    for(int i = 0; i < splashes.size();   i++){ splashes.at(i)->draw();   }
}

void EnvironmentManager :: drawMask(){
    glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    mask->draw();
}