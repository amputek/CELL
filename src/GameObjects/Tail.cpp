#include "Tail.hpp"

Tail :: Tail( int startLength, bool isWide, float startWidth, bool hasFins ){
    length = startLength;
    wideTail = isWide;
    width = startWidth;
    fins = hasFins;
}

void Tail :: update(vec2 location, float d){
    
    newFiniteCounter += deltaTime * 60.0f;
    
    //add a new Finite to the tail collection. The 'length' of the tail is equal to the finite's life.
    direction = d;

    tail.push_back( new Finite(location, length, d ) );

    //update all finites, checking they are still 'alive'
    for( vector <Finite*>::iterator p = tail.begin(); p != tail.end(); ){
        (*p)->update();
        if((*p)->alive() == false){
            delete *p;
            tail.erase(p);
        } else {
            ++p;
        }
    }
}

void Tail :: draw( CellRenderer & renderer ){
    
    vector<vec2> positions;
    for( int n = 0; n < tail.size(); n++)
        positions.push_back( tail[n]->getPosition() );
    
    renderer.drawTail(positions, wideTail, fins, width, direction);
}
