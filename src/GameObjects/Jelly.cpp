#include "Jelly.hpp"

Jelly :: Jelly(vec2 loc, int type) : Swimmer(loc, randFloat(18,40), 0.002f ){
    
    
    jellyType = type;
    

    //3 different types of Jellyfish
    if(type == 0){ feelerStartRadius = mRadius * 0.25f; }
    if(type == 1){ feelerStartRadius = mRadius * 0.6f; }
    if(type == 2){ feelerStartRadius = mRadius * 0.6f; }
    
    //add a few feelers
    for(int i = 0; i < randInt(5,10); i++){
        
        int feelerLength = randInt(10,30);
        float tentacleStartWidth = randFloat(3.0f, 6.0f);
        float tentacleEndWidth = 0.2f;
        
        feelers.push_back( new Feeler(mPosition, 1, feelerLength, tentacleStartWidth, tentacleEndWidth));

    }
}


void Jelly :: collide(const vec2 & loc, float colliderSize){
    for(int i = 0; i < feelers.size(); i++){
        feelers.at(i)->collide(loc, colliderSize);
    }
}

void Jelly :: draw( CellRenderer & renderer ){
    
    vector< vector< vec2 > > positions;
    
    for(int i = 0; i < feelers.size(); i++){
        vector<vec2> pos;
        for( int n = 0; n < feelers[i]->springs.size(); n++)
            pos.push_back( feelers[i]->springs[n]->getPosition() );
        positions.push_back( pos );
    }

    
    renderer.drawJellyfish( mPosition, mRadius, positions, jellyType, counter, 4.0f );

}


void Jelly :: update(){
    
    Swimmer::update();
    
    counter += deltaTime * 7.0f;
    
    for(int i = 0; i < feelers.size(); i++){
        feelers.at(i)->addForce( vec2(randFloat(-15,15), randFloat(18,34)) );
        float pos = (0.5*M_PI * i / (feelers.size()-1)) - 0.25*M_PI;
        feelers.at(i)->setHomePosition( mPosition + vec2(sin(pos) * feelerStartRadius, cos(pos) * feelerStartRadius ) );// + vec2(0, cos(counter)*3) * 1.0f;
        feelers.at(i)->update();  
    }
}
