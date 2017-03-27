#ifndef Tail_hpp
#define Tail_hpp

#include <iostream>
#include "Finite.hpp"

using namespace std;
using namespace ci;

class Tail : public IDrawable{
    
public:
    Tail( int startLength, bool isWide, float startWidth, bool hasFins )
    : mLength(startLength), mWideTail(isWide), mWidth(startWidth), mHasFins(hasFins){ }

    
    ~Tail(){
        for( vector<Finite*>::iterator p = segments.begin(); p != segments.end(); ++p){
            delete *p;
        }
    }
    
    
    void update( const vec2 & parentPosition , float parentDirection )
    {
        
 
        //add a new Finite to the tail collection. The 'length' of the tail is equal to the finite's life.
        mDirection = parentDirection;
        
        segments.push_back( new Finite(parentPosition, mLength, mDirection ) );
        
        //update all finites, checking they are still 'alive'
        for( vector <Finite*>::iterator tailSegment = segments.begin(); tailSegment != segments.end(); ){
            (*tailSegment)->update();
            if((*tailSegment)->mDeleteMe )
            {
                delete *tailSegment;
                segments.erase(tailSegment);
            } else {
                ++tailSegment;
            }
        }
    }
    
    
    void draw( CellRenderer & renderer )
    {
        
        vector<vec2> positions;
        for( auto segment : segments )
            positions.push_back( segment->getPosition() );
        
        renderer.drawTail(positions, mWideTail, mHasFins, mWidth, mDirection);

    }

    void incLength( float a ){ mLength += a; };
    void incWidth( float a ){ mWidth += a; };
    void setWide(){ mWideTail = true; };
    void setFins(){ mHasFins = true; };
    
private:

    vector<Finite*> segments;
    
    float mDirection;

    
    //attributes
    float mLength = 2;
    float mWidth = 0.3;
    bool mWideTail = false;
    bool mHasFins = false;
    

};


#endif
