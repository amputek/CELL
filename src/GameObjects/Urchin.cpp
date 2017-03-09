#include "Urchin.hpp"

Urchin :: Urchin(vec2 loc) : Swimmer(loc, true, randFloat(10,30), 0.002f ){
    

    int feelerLength = mRadius * randFloat(0.2,0.5);
    float tentacleStartWidth = 4.0f;
    float tentacleEndWidth = 0.2f;
    
    for(int i = 0; i < randInt(15,40); i++){
        mFeelers.push_back(new Feeler(loc, 1, feelerLength, tentacleStartWidth, tentacleEndWidth));
    }

}


void Urchin :: collide(const vec2 & loc, float colliderSize ){
    //count number of feelers that are making contact
    //only bother checking if hero is near enough
    
    if( dist(loc,mPosition) < 200){
        for(int i = 0; i < mFeelers.size(); i++){
            mFeelers.at(i)->collide( loc, colliderSize );
            if(mFeelers.at(i)->inContactWithCollider()){
                mContactCount++;
            }
        }
    }

}


void Urchin :: updateFeelers(){
    
    for(int i = 0; i < mFeelers.size(); i++){
        //distribute feelers equally around the Urchin
        float pos = 2*M_PI * i / mFeelers.size();
        mFeelers.at(i)->setHomePosition( mPosition + vec2(sin(pos) * mRadius, cos(pos) * mRadius ) );
        mFeelers.at(i)->update();
        mFeelers.at(i)->addForce( (mFeelers.at(i)->getPosition() - mPosition) * 1.0f );
        //random force added to simulate underwater currents
        mFeelers.at(i)->addForce( vrand(30.0f));

    }
    
}


void Urchin :: update(){
    
    if( randFloat() < 0.001f )
    {
        setDestination( mPosition + vrand(400.0f) );
    }
    
    //collide( global, radius );
    
    updateFeelers();

    
    mContactCount--;
    if(mContactCount <= 0){      mContactCount = 0;   }
    if(mContactCount >= 10){     mContactCount = 10; }
    
    Swimmer::update();
}


void Urchin :: draw( CellRenderer & renderer ){
    renderer.drawUrchin(mPosition, mRadius, mFeelers);
}
