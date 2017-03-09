#include "Starfish.hpp"

Starfish :: Starfish(vec2 loc) : Swimmer(loc, randFloat(20,30), 0.5f, true ){
    
    float tentacleStartWidth = 20.0f;
    float tentacleEndWidth = 0.3f;
    int feelerCount = 30;
    
    for(int i = 0; i < feelerCount; i++){
        float angleToFeeler = (i / (float)feelerCount) * 4.0f * M_PI;
        vec2 pos = loc + vec2( sin(angleToFeeler), cos(angleToFeeler ) ) * 2.0f;
        int feelerLength = randInt(4,8);
        mFeelers.push_back(new Feeler(pos, 1, feelerLength, tentacleStartWidth, tentacleEndWidth ));
    }
    
    mSpinCounter = 0;
    
    mContacts = 0;

}

void Starfish:: collide(const vec2 & loc){
    
    if( dist(loc,mPosition) > 200) return;

    vec2 d = loc - mPosition;
    float angleToHero = atan2(d.y, d.x);

    for(int i = 0; i < mFeelers.size(); i++){
        
     //   float angleToFeeler = (i / mFeelers.size()) * 4.0f * M_PI;

        
        vec2 d2 = mFeelers.at(i)->getPosition() - mPosition;

        
        float angleToFeeler = atan2(d2.y, d2.x);

        
        
        float angleDifference = atan2(sin(angleToFeeler-angleToHero), cos(angleToFeeler-angleToHero));
        
        if( abs(angleDifference) < M_PI * 0.2f )
        {
            if( mFeelers.at(i)->feelTowards( loc, 50.0f, 2.0f ) )
            {
                if(!mFleeing) mContacts += deltaTime * 65.0f;
            }
        }
    }

}

// true if enough contact has happened
bool Starfish :: reachedContactThreshold(){
    if(mFleeing == false && mContacts >= 1000){
        vec2 n = vrand(300.0f, 400.0f);
        setDestination( mPosition + n );
        mFleeing = true;
        return true;
    }
    return false;
}

void Starfish :: update(){
    
    //spin amount
    
    
    mSpinCounter += 0.2f * deltaTime;

    if( mContacts > 750 )
    {
        mSpinCounter += (mContacts-750) * deltaTime * 0.005f;
    }
    
    //reduce amount of contact
    if(mContacts > 0){
        mContacts -= deltaTime * 120.0f;
    }
    
    if( mFleeing == true )
    {
        if( glm::distance( targetDestination, mPosition ) < 20.0f )
        {
            mFleeing = false;
        }
    }
    else
    {
        vec2 n = vrand(15.0f);
        setDestination( mPosition + n );
    }
    
        

    //update feelers
    for(int i = 0; i < mFeelers.size(); i++){
        
        
        float angleToFeeler = (i / (float)mFeelers.size()) * 2.0f * M_PI;
        angleToFeeler += mSpinCounter;
        
        vec2 offsetPosition = vec2( sin(angleToFeeler), cos(angleToFeeler ) );
        
        mFeelers.at(i)->setHomePosition( mPosition + offsetPosition * 8.0f );
        mFeelers.at(i)->update();
        mFeelers.at(i)->addForce( offsetPosition * (20.5f + sin(mSpinCounter + (angleToFeeler)) * 9.0f) );

        //random force added, which stops things being static - looks like the flow of underwater currents
        mFeelers.at(i)->addForce( vrand(20.0f) );
    }
    
    Swimmer::update();
}

void Starfish :: draw( CellRenderer * renderer ){
    renderer->drawStarfish( mPosition, mFeelers, mContacts);
}
