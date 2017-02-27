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

//add Fin to a sepcified point in the tail. Fins are Path2Ds
void Tail :: addFin( int top, int mid, int point ){
    Finite* finTopPoint = tail.at(top);
    Finite* finMidPoint = tail.at(mid);
    Finite* finPoint = tail.at(point);
    
    float finDirection = atan2( tail.at(point+1)->local.y - finPoint->local.y, tail.at(point+1)->local.x - finPoint->local.x );
    float finSize = width*3;
    
    vec2 finLeft = vec2(finPoint->local.x + cos(finDirection + M_PI*0.5) * finSize, finPoint->local.y + sin(finDirection + M_PI*0.5) * finSize);
    vec2 finRight = vec2(finPoint->local.x + cos(finDirection - M_PI*0.5) * finSize, finPoint->local.y + sin(finDirection - M_PI*0.5) * finSize);
    
    Path2d fin;
    fin.moveTo(finLeft);
    fin.curveTo(finTopPoint->local, finTopPoint->local, finRight);
    fin.curveTo(finMidPoint->local, finMidPoint->local, finLeft);
    
    finPaths.push_back( fin );
}

void Tail :: draw(){
    
    
    updateTailPaths();

    gl::color(ColorA8u(255,255,255,255));
    gl::draw(mPath);
    
    //draw side paths if tail is wide
    if(wideTail == true){
        gl::draw(lPath);
        gl::draw(rPath);
        for(int i = 0; i < ridgePaths.size(); i++){
            gl::draw(ridgePaths.at(i) );
        }
    }
    
    //draw fins
    if(fins == true){
        for(int i =0 ; i < finPaths.size(); i++){
            gl::draw( finPaths.at(i) );
        }
    }
}

void Tail :: updateTailPaths(){
    
    mPath.clear();
    lPath.clear();
    rPath.clear();
    ridgePaths.clear();
    
    //initilase the path2Ds
    if(tail.size() > 0){
        mPath.moveTo(tail.at(0)->local);
        if( wideTail == true){
            lPath.moveTo(vec2(tail.at(0)->local.x, tail.at(0)->local.y));
            rPath.moveTo(vec2(tail.at(0)->local.x, tail.at(0)->local.y));
        }
    }
    
    
    //draw Line to each part of the tail
    int index = 0;
    
    for( vector<Finite*>::iterator p = tail.begin(); p != tail.end(); ++p ){
        mPath.lineTo((*p)->local);
        
        if(wideTail == true){
            //width of the tail increases towards body
            //points on the 'wide' parts of the tail are points on the tail + a sin/cos vector
            float sizeMod = width * index / tail.size();
            lPath.lineTo(vec2((*p)->local.x + sin(direction + rotMod) * sizeMod, (*p)->local.y + cos(direction + rotMod) * sizeMod));
            rPath.lineTo(vec2((*p)->local.x + sin(direction - rotMod) * sizeMod, (*p)->local.y + cos(direction - rotMod) * sizeMod));
            
            
            //add ridges along the tail, keep these in a collection
            Path2d sPath;
            sPath.moveTo(lPath.getCurrentPoint());
            sPath.curveTo(mPath.getCurrentPoint(), mPath.getCurrentPoint(), rPath.getCurrentPoint());
            ridgePaths.push_back( sPath );
            
        
        }
        index++;
    }
    
    //add Fins
    if(fins == true){
        finPaths.clear();
        if(tail.size() > 5){
            addFin(3,2,1);
        }
        if(tail.size() > 18){
            addFin(15,14,12);
        }
    }
    
      
}
