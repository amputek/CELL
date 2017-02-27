#include "Spark.hpp"

Spark :: Spark(const vec2 & loc, int sparkType, gl::TextureRef * tex ) : Swimmer(loc){
    Swimmer::speed = 0.8f;
    GameObject::radius = 8.0f;
    type = sparkType;
    img = tex;
}

void Spark :: update(){

    
    life += deltaTime * 60.0f;
    
    //radius returns to 5 after pulse (from SuperCollider)
    if(radius > 8.0f){
        radius -= deltaTime *5.0f;
    }
    
    Swimmer::update();
    
    
    //Spark is a collection of "Finites" that reduce in size
    
    if( life > 1.0f )
    {
        finites.push_back( new Finite(global, 12.0f, radius * 0.8f) );
        life = 0.0f;
    }
    
    for( vector<Finite*>::iterator p = finites.begin(); p != finites.end(); ){
        (*p)->update();
        if((*p)->alive() == false){
            delete *p;
            p = finites.erase(p);
        } else {
            ++p;
        }
    }
    
}

void Spark :: draw(){
    
    if( !onScreen() ) return;
    entityDrawCount++;
    
    if(type == 0){ gl::color(ColorA8u( 255         , 80+radius*3  , 20+radius*4,     220 ) ); }
    if(type == 1){ gl::color(ColorA8u( 108+radius*3, 5+radius*3   , 255        ,     220 ) ); }
    if(type == 2){ gl::color(ColorA8u( 75+radius*4 , 228	      , 178	       ,     220 ) ); }
    
    //draw a few circles at each Finite position
    for(int i = 0; i < finites.size(); i++){
        gl::ScopedModelMatrix modelScope;
        gl::translate( finites.at(i)->local );
        float r = (1.0 - (finites.at(i)->currentLife / finites.at(i)->maxLife) * 0.5f) * (finites.at(i)->radius);
        gl::draw( *img, Rectf(-r, -r, r, r ));
    }
    
   // gl::color(Color(1.0,1.0,1.0));
   // gl::drawLine( local, localise(targetDestination, 1.0f) );
    
    gl::ScopedModelMatrix modelScope;
    gl::translate( local );
    gl::draw( *img, Rectf(-radius, -radius, radius, radius ));
    gl::draw( *img, Rectf(-radius/2, -radius/2, radius/2, radius/2 ));



}
