
//some of the code in this class is based on algorithms found on the Processing website, from an example sketch by Casey Reas and Ben Fry
//Sketch URL: http://processing.org/learning/topics/chain.html
// For the most part, this involves using damping, mass, and stiffness values to calculate acceleration and velocity.
// My extensions of this sketch are: collisions and adding further exeternal forces.

#ifndef Spring_hpp
#define Spring_hpp

#include "PhysicsObject.hpp"

using namespace std;

class SpringyObject : public PhysicsObject{
    
public:
    SpringyObject(vec2 startLocation, float depth, float stiffness, float mass, float damping )
        : PhysicsObject(startLocation, 1, 1, damping), mStiffness(stiffness), mMass(mass){ }

    
    void moveTowards(const vec2 & targetToMoveTowards)
    {
        vec2 force = (targetToMoveTowards - mPosition) * mStiffness;
        addForce( force );
    }
    
    void setPosition(vec2 target)
    {
        mPosition = target;
    }
    
    bool collide(const vec2 & colliderPosition, float collideDist)
    {
        if(dist(colliderPosition, mPosition) > collideDist) return false;
        

        vec2 d = colliderPosition - mPosition;
        float angle = atan2(d.y, d.x);
        vec2 target = vec2(colliderPosition.x - cos(angle) * collideDist, colliderPosition.y - sin(angle) * collideDist);
        vec2 force = (target - mPosition) * mStiffness * 3.0f;
        addForce( force );
        
        mPosition = target;
        
        return true;
    }
    
    void addForce(vec2 force)
    {
        vec2 accel = force / mMass;
        addVelocity( accel * deltaTime * 60.0f );
    }
    

    
private:
    float mMass;
    float mStiffness;
};

#endif
