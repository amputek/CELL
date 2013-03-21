#include "Mask.h"

Mask :: Mask( gl::Texture tex){
    
    //set up FBO with mask texture
    gl::Fbo::Format format;
	mFbo = gl::Fbo( 800, 600 , format );
    
    mTexture = tex;
    
    gl::SaveFramebufferBinding bindingSaver;
  	mFbo.bindFramebuffer();
    
    gl::setViewport(mFbo.getBounds() );
    gl::setMatricesWindow( mFbo.getSize(), false );
    gl::enableAlphaBlending();  //no additive blending when drawing to Fbo
    
    gl::clear(ColorA(0,0,0,0.5));
    gl::color( ColorA(1.0f, 1.0f, 1.0f, 0.5f) );
    
    gl::draw(mTexture, Vec2f( 272, 172 ) );
    mFbo.unbindFramebuffer();
    
}

void Mask :: draw(){
    gl::draw( mFbo.getTexture(0), Rectf( x1, y1, x2, y2 ) );
}

void Mask :: update(float xo, float yo, float mod){
    //center of mask will sit above the player's location (which is not neccesarily the center of the screen)
    //mask's size will increase in shallower waters
    x1 = -(cinder::app::getWindowWidth()*mod)-xo;
    y1 = -(cinder::app::getWindowHeight()*mod)-yo;
    x2 = cinder::app::getWindowWidth() + (cinder::app::getWindowWidth()*mod)-xo;
    y2 = cinder::app::getWindowHeight()+ (cinder::app::getWindowHeight()*mod)-yo;
}
