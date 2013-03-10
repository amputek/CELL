#include "Tail.h"

Tail :: Tail( int ln, bool wt, float wd, bool fn ){
    length = ln;
    wideTail = wt;
    width = wd;
    fins = fn;    
}

void Tail :: update(Vec2f location, float d){
    
    direction = d;
    tail.push_back( new Finite(location, length, d ) );
    
    updateTailPaths();
    
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
    
    float finDirection = finPoint->direction;
    float finSize = width*3;
    
    Vec2f finLeft = Vec2f(finPoint->local.x + sin(finDirection + M_PI*0.5) * finSize, finPoint->local.y + cos(finDirection + M_PI*0.5) * finSize);
    Vec2f finRight = Vec2f(finPoint->local.x + sin(finDirection - M_PI*0.5) * finSize, finPoint->local.y + cos(finDirection - M_PI*0.5) * finSize);
    
    Path2d fin;
    fin.moveTo(finLeft);
    fin.curveTo(finTopPoint->local, finTopPoint->local, finRight);
    fin.curveTo(finMidPoint->local, finMidPoint->local, finLeft);
    
    finPaths.push_back( fin );
}

void Tail :: draw(){
    glLineWidth(0.1);
    gl::color(ColorA8u(150,200,255,100));
    gl::draw(mPath);
    if(wideTail == true){
        gl::draw(lPath);
        gl::draw(rPath);
    }
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
    
    //initilase the path2Ds
    if(tail.size() > 0){
        mPath.moveTo(tail.at(0)->local);
        if( wideTail == true){
            lPath.moveTo(Vec2f(tail.at(0)->local.x, tail.at(0)->local.y));
            rPath.moveTo(Vec2f(tail.at(0)->local.x, tail.at(0)->local.y));
        }
    }
    
    
    //draw Line to each part of the tail
    int j = 0;
    for( vector<Finite*>::iterator p = tail.begin(); p != tail.end(); ++p ){
        mPath.lineTo((*p)->local);
        
        if(wideTail == true){
            float sizeMod = width * j / tail.size();
            lPath.lineTo(Vec2f((*p)->local.x + sin(direction + rotMod) * sizeMod, (*p)->local.y + cos(direction + rotMod) * sizeMod));
            rPath.lineTo(Vec2f((*p)->local.x + sin(direction - rotMod) * sizeMod, (*p)->local.y + cos(direction - rotMod) * sizeMod));
            
            Path2d sPath;
            sPath.moveTo(lPath.getCurrentPoint());
            sPath.curveTo(mPath.getCurrentPoint(), mPath.getCurrentPoint(), rPath.getCurrentPoint());
            gl::draw( sPath );
            
            if(j > 4){
                Path2d tPath;
                tPath.moveTo(lPath.getPoint(lPath.getNumPoints()-4));
                tPath.curveTo(mPath.getCurrentPoint(), mPath.getCurrentPoint(), rPath.getPoint(rPath.getNumPoints()-4));
                gl::draw( tPath );
            }
            
            j++;
        }
        
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