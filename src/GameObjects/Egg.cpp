#include "Egg.hpp"
#include "cinder/Triangulate.h"

Egg :: Egg(vec2 loc) : GameObject(loc, 1, 160){
    ratio = getSize() * 2.4;
    

    float damping = 1.2f;
    float stiffness = 5.0f;
    float mass = 10.0f;
    
    
    for(int i = 0; i < numSprings; i++){
        float pos =  2 * M_PI * i / numSprings;
        //create new spring with damp, stiffness, mass parameters
        
        springs.push_back( new Spring( loc + vec2(sin(pos) * getSize(), cos(pos) * getSize() ) , 1, stiffness, mass, damping  ) );
    }

    ins = false;
    
    //update a few times upon arrival
    for(int i = 0; i < numSprings; i++){
        update();
    }
}


//collide egg with a location (generally the player)
void Egg :: collide(const vec2 & loc, float radius){
    for(int i = 0; i < numSprings; i++){
        springs.at(i)->collide(loc, radius);
    }
}

//true if a location (hero) is inside radius
void Egg :: setInside(vec2 loc){
    ins = dist(loc, mPosition) < getSize();
}


void Egg :: update(){
    
    counter += deltaTime * 3.0f;
    
    for(int i = 0; i < numSprings; i++){
        if( randFloat() < 0.2f)
        {
            float pos =  2 * M_PI * i / numSprings;
            springs.at(i)->moveTowards( mPosition + vec2(sin(pos) * getSize(), cos(pos) * getSize() ) );
        }
        springs.at(i)->update();
    }

    
    


    //update each spring with it's neighbouring springs
    int j = 0;
    for(int i = randInt( numSprings-1 ); j < numSprings; i++, j++){
        int t = i % numSprings;
        int t2 = (i+1) % numSprings;
        springs.at( t )->moveTowards( springs.at( t2 )->getPosition() );
    }

    j = 0;
    for(int i = numSprings + randInt( numSprings-1); j < numSprings; i--, j++){
        int t = i % numSprings;
        int t2 = (i+1) % numSprings;
        springs.at( t2 )->moveTowards( springs.at( t )->getPosition() );
    }

    
//    drawPositions.clear();
//    for(int i = 0; i < numSprings; i++)
//    {
//        int after = i + 1;
//        if( after == numSprings ) after = 0;
//        drawPositions.push_back( springs.at(i)->local );
//        drawPositions.push_back( (springs.at(i)->local + springs.at(after)->local) * 0.5f);
//    }
//    
//    //Average twice
//    for(int n = 0; n < 2; n++)
//    {
//        for(int i = 0; i < drawPositions.size(); i++)
//        {
//            int before = i - 1;
//            if( before < 0 ) before = drawPositions.size()-1;
//            int after = i + 1;
//            if( after == drawPositions.size() ) after = 0;
//            vec2 p = (drawPositions.at(before) + drawPositions.at(after)) * 0.5f;
//            drawPositions.at(i) = (drawPositions.at(i) + p) * 0.5f;
//        }
//    }
//    

}


void Egg :: draw( CellRenderer & renderer ){

    vector<vec2> pos;
    for( int n = 0; n < springs.size(); n++)
        pos.push_back( springs[n]->getPosition() );

    renderer.drawEgg( mPosition, ratio, counter, pos );

}
