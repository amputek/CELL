#include "Starfish.hpp"

Starfish :: Starfish(vec2 loc, gl::TextureRef* tex) : Swimmer(loc, true){
    
    GameObject::radius = int(rand(20,30));
    
    Braitenberg::speed = 0.5f;
    
    float tentacleStartWidth = 20.0f;
    float tentacleEndWidth = 0.3f;
    int feelerCount = 30;
    
    for(int i = 0; i < feelerCount; i++){
        float angleToFeeler = (i / (float)feelerCount) * 4.0f * M_PI;
        vec2 pos = loc + vec2( sin(angleToFeeler), cos(angleToFeeler ) ) * 2.0f;
        feelers.push_back(new Feeler(pos, randInt(4,8), tentacleStartWidth, tentacleEndWidth ));
    }
    
    spinCounter = 0;
    
    contacts = 0;
    
    img = tex;
}

//feelers move away from Player
void Starfish:: collide(const vec2 & loc){
    
    if( dist(loc,global) < 160){

        vec2 d = loc - global;
        float angleToHero = atan2(d.y, d.x);
    
        for(int i = 0; i < feelers.size(); i++){
            
            vec2 d2 = feelers.at(i)->global - global;
            float angleToFeeler = atan2(d2.y, d2.x);

            
            
            float angleDifference = atan2(sin(angleToFeeler-angleToHero), cos(angleToFeeler-angleToHero));
            
            if( abs(angleDifference) < M_PI * 0.2f )
            {
                if( feelers.at(i)->feelTowards( loc, 50.0f, 2.0f ) )
                {
                    if(!fleeing) contacts += deltaTime * 55.0f;
                }
            }
        }
    }
}

// true if enough contact has happened
bool Starfish :: activated(){
    if(fleeing == false && contacts >= 1000){
        vec2 n = vrand(300.0f, 400.0f);
        setDestination( global + n );
        fleeing = true;
    
        cout << "Starfish activated - moving away! " << n << "\n";
        return true;
    }
    return false;
}

void Starfish :: update(){
    
    //spin amount
    
    
    spinCounter += 0.2f * deltaTime;

    if( contacts > 750 )
    {
        spinCounter += (contacts-750) * deltaTime * 0.05f;
    }
    
    //reduce amount of contact
    if(contacts > 0){
        contacts -= deltaTime * 90.0f;
    }
    
    if( fleeing == true )
    {
        if( glm::distance( targetDestination, global ) < 20.0f )
        {
            fleeing = false;
        }
    }
    else
    {
        vec2 n = vrand(15.0f);
        setDestination( global + n );
    }
    
        

    //update feelers
    for(int i = 0; i < feelers.size(); i++){
        
        
        float angleToFeeler = (i / (float)feelers.size()) * 2.0f * M_PI;
        angleToFeeler += spinCounter;
        
        vec2 offsetPosition = vec2( sin(angleToFeeler), cos(angleToFeeler ) );
        
        feelers.at(i)->global = global + offsetPosition * 8.0f;
        
        feelers.at(i)->update();
        
        feelers.at(i)->addForce( offsetPosition * (20.5f + sin(spinCounter + (angleToFeeler)) * 9.0f) );

        
        
        //random force added, which stops things being static - looks like the flow of underwater currents
        feelers.at(i)->addForce( vrand(20.0f) );
    }
    
    Swimmer::update();
}

void Starfish :: draw(){
    
    if(!onScreen()) return;
    entityDrawCount++;

    for(int i = 0; i < feelers.size(); i++){
        Shape2d mShape;
        mShape.moveTo( feelers.at(i)->drawPositions.at(0) );
        for(int n = 0; n < feelers.at(i)->drawPositions.size(); n++)
            mShape.lineTo( feelers.at(i)->drawPositions.at(n) );
        mShape.close();
        gl::color(ColorA8u(220,120 - contacts * 0.08f,6 + contacts * 0.05f,70));
        gl::drawSolid( mShape );
        gl::color(ColorA8u(220,120 - contacts * 0.08f,6 + contacts * 0.05f,140));
        gl::draw( mShape );
    }

    

}
