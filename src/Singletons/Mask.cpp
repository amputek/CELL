#include "Mask.hpp"

Mask :: Mask( gl::TextureRef tex ){
    
    //set up FBO with mask texture
    gl::Fbo::Format format;
    mFbo = gl::Fbo::create( 1024, 1024 , format );
    
    mTexture = tex;
    
  	mFbo->bindFramebuffer();
    

    gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
    gl::setMatricesWindow( mFbo->getSize(), false );
    gl::enableAlphaBlending();  //no additive blending when drawing to Fbo
    
    gl::clear(ColorA(0,0,0,0.5));
    gl::color( ColorA(1.0f, 1.0f, 1.0f, 0.65f) );
    
    gl::draw(mTexture, vec2(384,384) );
    
    mFbo->unbindFramebuffer();
    
    
}

void Mask :: draw(){
    gl::color(1,1,1);
    gl::draw( mFbo->getColorTexture(), Rectf( x1, y1, x2, y2 ) );
}

void Mask :: update(const vec2 & heroLocalPosition, float depthMod)
{
    //center of mask will sit above the player's location (which is not neccesarily the center of the screen)
    //mask's size will increase in shallower waters
    
    x1 = heroLocalPosition.x - (cinder::app::getWindowWidth()*depthMod);
    y1 = heroLocalPosition.y - (cinder::app::getWindowWidth()*depthMod);
    x2 = heroLocalPosition.x + (cinder::app::getWindowWidth()*depthMod);
    y2 = heroLocalPosition.y + (cinder::app::getWindowWidth()*depthMod);

}
