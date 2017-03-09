#include "Urchin.hpp"

Urchin :: Urchin(vec2 loc) : Swimmer(loc, true, randFloat(10,30), 0.002f ){
    

    int feelerLength = mRadius * randFloat(0.2,0.5);
    float tentacleStartWidth = 4.0f;
    float tentacleEndWidth = 0.2f;
    
    for(int i = 0; i < randInt(15,40); i++){
        feelers.push_back(new Feeler(loc, 1, feelerLength, tentacleStartWidth, tentacleEndWidth));
    }

}


void Urchin :: collide(const vec2 & loc, float colliderSize ){
    //count number of feelers that are making contact
    //contactCount = 0;
    //only bother checking if hero is near enough
    if( dist(loc,mPosition) < 200){
        for(int i = 0; i < feelers.size(); i++){
            feelers.at(i)->collide( loc, colliderSize );
            if(feelers.at(i)->inContactWithCollider()){
                contactCount++;
            }
        }
    }
    
    //if( contactCount > 0 ) cout << contactCount << "\n";
}


void Urchin :: updateFeelers(){
    
    for(int i = 0; i < feelers.size(); i++){
        //distribute feelers equally around the Urchin
        float pos = 2*M_PI * i / feelers.size();
        feelers.at(i)->setHomePosition( mPosition + vec2(sin(pos) * mRadius, cos(pos) * mRadius ) );
        feelers.at(i)->update();
        feelers.at(i)->addForce( (feelers.at(i)->getPosition() - mPosition) * 1.0f );
        //random force added to simulate underwater currents
        feelers.at(i)->addForce( vrand(30.0f));

    }
    
}


void Urchin :: update(){
    
    if( randFloat() < 0.001f )
    {
        setDestination( mPosition + vrand(400.0f) );
    }
    
    //collide( global, radius );
    
    updateFeelers();

    
    contactCount--;
    if(contactCount <= 0){      contactCount = 0;   }
    if(contactCount >= 10){     contactCount = 10; }
    
    Swimmer::update();
}


void Urchin :: draw( CellRenderer & renderer ){

    vector< vector< vec2 > > positions;
    
    for(int i = 0; i < feelers.size(); i++){
        vector<vec2> pos;
        for( int n = 0; n < feelers[i]->springs.size(); n++)
            pos.push_back( feelers[i]->springs[n]->getPosition() );
        positions.push_back( pos );
    }
    
    renderer.drawUrchin(mPosition, mRadius, positions);
}
