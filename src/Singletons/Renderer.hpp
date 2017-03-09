#ifndef Images_hpp
#define Images_hpp

#include <stdio.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Log.h"
#include "Globals.hpp"
#include "Feeler.hpp"

using namespace std;
using namespace ci::app;
using namespace ci;
using namespace gl;

class CellRenderer{
public:
    
    CellRenderer();

    int entityDrawCount = 0;
    bool miniMapActive = true;
    

    void drawCursor( const vec2 & mousePos )
    {
        gl::ScopedBlendAlpha alpha;
        gl::color(1.0,1.0,1.0);
        gl::draw( cursorImg, Rectf( mousePos.x - 2, mousePos.y - 2, mousePos.x + 2, mousePos.y + 2) );
    }
    
    void drawTitle( float splashOpacity )
    {
        double op = splashOpacity;
        if( op > 1.0f ) op  = 1.0f;
        gl::color( ColorA8u(op * 130,op * 228,op * 247,op * 255.0f) );
        
        Rectf rect = Rectf(-300,-300,300,300);
        vec2 pos = vec2(-400,-2000);
        
        gl::pushModelView();
        gl::translate( toLocal( pos, 0.55f ) );
        gl::draw( title1,rect);
        gl::popModelView();
        gl::pushModelView();
        gl::translate( toLocal( pos, 0.6f ) );
        gl::draw( title2,rect);
        gl::popModelView();
        gl::pushModelView();
        gl::translate( toLocal( pos, 0.57f ) );
        gl::draw( title3, rect);
        gl::popModelView();
        gl::pushModelView();
        gl::translate( toLocal( pos, 0.525f ) );
        gl::draw( title4, rect);;
        gl::popModelView();
    }
    
    
    void setupMask( FboRef & mFbo )
    {
        //set up FBO with mask texture
        gl::Fbo::Format format;
        mFbo = gl::Fbo::create( 1024, 1024 , format );
        
        mFbo->bindFramebuffer();
        
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatricesWindow( mFbo->getSize(), false );
        gl::enableAlphaBlending();  //no additive blending when drawing to Fbo
        
        gl::clear(ColorA(0,0,0,0.5));
        gl::color( ColorA(1.0f, 1.0f, 1.0f, 0.65f) );
        
        gl::draw( maskImg, vec2(384,384) );
        
        mFbo->unbindFramebuffer();
        
    }
    
    void drawMask( const vec2 & position, FboRef & mFbo, float depthMod )
    {
        //center of mask will sit above the player's location (which is not neccesarily the center of the screen)
        //mask's size will increase in shallower waters
        
        Rectf drawRect = Rectf(position.x - (cinder::app::getWindowWidth()*depthMod),
                               position.y - (cinder::app::getWindowWidth()*depthMod),
                               position.x + (cinder::app::getWindowWidth()*depthMod),
                               position.y + (cinder::app::getWindowWidth()*depthMod));

        gl::color(1,1,1);
        gl::draw( mFbo->getColorTexture(), drawRect );
        
        if( !miniMapActive ) return;
        
        gl::pushModelView();
        gl::disableAlphaBlending();
        color( ColorA(1,1,1,0.8));
        gl::ScopedModelMatrix m;
        gl::draw( miniMap->getColorTexture(), Rectf( 0,0,400,300) );
        gl::popModelView();
    }
    
    void drawPlayer( const vec2 & global, float direction, float radius, int bodyType );
    
    void drawFriendly( const vec2 & global, float direction, float radius );
    
    void drawBubble( const vec2 & global , float radius, float depth );

    void drawSplash( const vec2 & global, float currentLife, float maxLife, float startSize );
    
    void drawSpore( const vec2 & global, float radius, float depth, int sporeType );
    
    void drawStarfish( const vec2 & global, const vector<Feeler*> & feelers, float contacts );
    
    void drawBeam( const vec2 & global, float depth, float op );

    void drawJellyfish( const vec2 & global, float radius, const vector<Feeler*> & feelers, int jellyType, float counter, float baseWidth );
    
    void drawPlankton( const vec2 & global, float depth, float radius, int planktonType, float rotation );
    
    void drawEgg( const vec2 & global, float ratio, float counter, const vector<vec2> & positions );
    
    void drawSpark( const vec2 & global, float radius, int type, const vector<vec2> & positions, const vector<float> & sizes );
    
    void drawUrchin( const vec2 & global, float radius, const vector< Feeler * > & feelers );
    
    void drawTail( vector<vec2> positions, bool wideTail, bool fins, float width, float direction );
    
    void refresh()
    {
        entityDrawCount = 0;
        
        miniMap->bindFramebuffer();
        gl::ScopedViewport scpVp( ivec2( 0 ), miniMap->getSize() );
        gl::clear( Color(0.1,0.1,0.1) );

        gl::disableAlphaBlending();
        gl::color(1,1,1);
        
        vec2 tl = globalToMiniMap(globals::globalise( vec2(0,0), 1 ));
        vec2 br = globalToMiniMap(globals::globalise( getWindowSize(), 1 ));
                                  
        gl::drawStrokedRect( Rectf( tl.x, tl.y, br.x, br.y ) );
        

        miniMap->unbindFramebuffer();
    }
    
    
    vec2 toLocal( const vec2 & global, float depth )
    {
        //relationship between local (drawn) co-ordinates and global co-ordinates
        vec2 local = global - globals::offset;
        local *= depth;
        
        // makes the point of parralax at the center of the screen
        local += (cinder::app::getWindowSize() / 2);
        
        return local;
    }
    
    //overload with no depth (1)
    vec2 toLocal( const vec2 & global )
    {
        return toLocal( global, 1.0f );
    }
    
    bool onScreen( const vec2 & global, float depth, float onScreenSize )
    {
        return onScreen( toLocal(global,depth), onScreenSize );
    }

private:
    
    cinder::gl::TextureRef loadPngImage( string src ){
        string fullsrc = "images/" + src + ".png";
        try {
            return gl::Texture::create( loadImage( loadResource( fullsrc ) ) );
        } catch( std::exception &exc ) {
            CI_LOG_EXCEPTION( "failed to load image.", exc );
            return nullptr;
        }
    }

    gl::TextureRef sporeImgs[3][3];
    gl::TextureRef planktonImgs[5];
    gl::TextureRef cursorImg;
    gl::TextureRef bubbleImgs[3];
    gl::TextureRef beamImg;
    gl::TextureRef jellyImgs[3];
    gl::TextureRef friendlyImg;
    gl::TextureRef urchinImg;
    gl::TextureRef eggImg;
    gl::TextureRef sparkImg;
    gl::TextureRef playerImgs[3];
    gl::TextureRef title1;
    gl::TextureRef title2;
    gl::TextureRef title3;
    gl::TextureRef title4;
    gl::TextureRef maskImg;
    gl::TextureRef splashImg;
    
    
    gl::FboRef miniMap;
    
    bool onScreen( const vec2 & local, float onScreenSize ){
        if (local.x < -onScreenSize ) return false;
        if (local.x > getWindowWidth() + onScreenSize ) return false;
        if (local.y < -onScreenSize ) return false;
        if (local.y > getWindowHeight() + onScreenSize) return false;
        return true;
    }

    
    vec2 globalToMiniMap( vec2 position )
    {
//        return (((position-globals::offset) + (getWindowCenter())) * 0.2f) + vec2(400,300);
        return (position + vec2(3000,4000)) * 0.15f;
    }
    
    void addToMiniMap( const vec2 & position, const Color & color, float r, bool isGettingDrawn )
    {
        
        if( !miniMapActive ) return;
        
        miniMap->bindFramebuffer();
        gl::ScopedBlendAlpha alpha;
        gl::ScopedViewport scpVp( ivec2( 0 ), vec2(800,600) );
        gl::color( color );
        vec2 p = globalToMiniMap( position );

        float radius = r * 0.15f;
        if ( radius < 3 ) radius = 3;
        
        if( isGettingDrawn )
            gl::drawSolidCircle( p, radius );
        else
            gl::drawStrokedCircle( p, radius );
        miniMap->unbindFramebuffer();
    }
    
    
    Shape2d drawFeeler( Feeler * feeler );

    
    Path2d drawFin( const vec2 & top, const vec2 & mid, const vec2 & point, const vec2 & nextPoint, float width )
    {
        float finDirection = atan2( nextPoint.y - point.y, nextPoint.x - point.x );
        float finSize = width*3;
        
        vec2 finLeft = vec2(point.x + cos(finDirection + M_PI*0.5) * finSize, point.y + sin(finDirection + M_PI*0.5) * finSize);
        vec2 finRight = vec2(point.x + cos(finDirection - M_PI*0.5) * finSize, point.y + sin(finDirection - M_PI*0.5) * finSize);
        
        Path2d fin;
        fin.moveTo(finLeft);
        fin.curveTo(top, top, finRight);
        fin.curveTo(mid, mid, finLeft);
        
        return fin;
    }
    

    
    
};


#endif /* Images_hpp */
