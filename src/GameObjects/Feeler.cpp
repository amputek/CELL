#include "Feeler.hpp"

Feeler :: Feeler(vec2 loc, int joints, float base, float tip, float depth ) : GameObject(loc, depth ){
    
    baseWidth = base;
    tipWidth = tip;
    
    float stiffness = 2.0f;
    float mass = 5.0f;
    float damping = 7.0f;
    
    for(int i = 0; i < joints; i++){
        
        springs.push_back( new Spring(loc + vec2(i * 2.0f,i * 2.0f), i == 0 ? 10.0f : stiffness, mass, damping, depth) );
    }

    update();
    feelerContact = false;
};



void Feeler :: collide(const vec2 & heroLoc, float colliderSize ){
    for(int i = 1; i < springs.size(); i++){
        springs.at(i)->collide(heroLoc, colliderSize);
        if(springs.at(i)->contact() == true){
            feelerContact = true;
        }
    }
}

bool Feeler :: feelTowards( const vec2 & colliderPos, float minDist, float force ){
    
    bool activated = false;
    
    
    
    for(int i = 1; i < springs.size(); i++){
    
        float alongRatio = (float)i / springs.size();
        
        Spring * spring = springs.at(i);
        if( glm::distance( spring->global, colliderPos ) < minDist )
        {
            activated = true;
        }
        if( activated )
        {
            spring->addForce( (colliderPos - spring->global) * force * alongRatio );
        }
    }
    return activated;
}


//add force - for environmental affects (gravity, flow etc)
void Feeler :: addForce(vec2 force){
    for(int i = 1; i < springs.size(); i++){
        springs.at(i)->addForce(force);
    }
}


void Feeler :: update(){
    feelerContact = false;
    
    //a Feeler's 'global' position is it's BASE point, the first spring gets updated agains this position
    if(springs.size() > 0){
        //springs.at(0)->update(global);
        springs.at(0)->global = global;
        springs.at(0)->update();
    }
    
    //all other springs gets updated against the prior spring
    for(int i = 1; i < springs.size(); i++){
        springs.at(i)->moveTowards(springs.at(i-1)->global);
    }
    
    
    updateDrawPositions();
    

    GameObject::update();
}


void Feeler :: updateDrawPositions(){
    
    
    drawPositions.clear();
    for(int i = 0; i < springs.size() - 1; i++)
    {
        drawPositions.push_back( springs.at(i)->local );
        drawPositions.push_back( (springs.at(i)->local + springs.at(i + 1)->local) * 0.5f);
    }
    
    //Average twice
    for(int n = 0; n < 2; n++)
    {
        for(int i = 1; i < drawPositions.size() - 1; i++)
        {
            vec2 p = (drawPositions.at(i - 1) + drawPositions.at(i + 1)) * 0.5f;
            drawPositions.at(i) = (drawPositions.at(i) + p) * 0.5f;
        }
    }
    
    
    vec2 original1 = drawPositions.at(0);
    vec2 next1 = drawPositions.at(1);
    vec2 toNext1 = glm::normalize(next1 - original1);
    vec2 mod1 = vec2( -toNext1.y, toNext1.x ) * baseWidth * 0.5f;
    vec2 right1= original1 - mod1;
    

    float currentWidth = tipWidth;
    for(int n = drawPositions.size() - 1; n >= 1; n--)
    {
        vec2 original = drawPositions.at(n);
        vec2 next = drawPositions.at(n-1);
        vec2 toNext = glm::normalize(next - original);
        vec2 mod = vec2( -toNext.y, toNext.x ) * currentWidth * 0.5f;
        vec2 right = original + mod;
        
        currentWidth += ( (baseWidth - tipWidth) / drawPositions.size());
        
        drawPositions.at(n) -= mod;
        drawPositions.push_back( right );
        
    }
    

    drawPositions.push_back( right1 );
    drawPositions.at(0) += mod1;
    
}
