#include "Spark.hpp"

Spark :: Spark(const vec2 & loc, int sparkType ) : Swimmer(loc, 8.0f, 0.8f){
    type = sparkType;
}

void Spark :: update(){

    
    life += deltaTime * 60.0f;
    
    //radius returns to 8 after pulse (from SuperCollider)
    if(mRadius > 8.0f){
        mRadius -= deltaTime * 25.0f;
    }
    
    Swimmer::update();
    
    
    //Spark is a collection of "Finites" that reduce in size
    
    if( life > 1.0f )
    {
        finites.push_back( new Finite(mPosition, 12.0f, mRadius * 0.8f) );
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

void Spark :: draw( CellRenderer & renderer ){
    
  
    vector<vec2> positions;
    vector<float> sizes;
    
    for(int i = 0; i < finites.size(); i++){
        positions.push_back( finites[i]->getPosition() ) ;
        sizes.push_back( (1.0 - (finites.at(i)->lifeRatio()) * 0.5f) * (finites.at(i)->getSize() ) );
    }
    
    renderer.drawSpark(mPosition, mRadius, type, positions, sizes);
   

}
