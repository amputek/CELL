#ifndef Images_hpp
#define Images_hpp

#include <stdio.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Log.h"
#include "Globals.hpp"

using namespace std;
using namespace ci::app;
using namespace ci;
using namespace gl;

class CellRenderer{
public:
    
    CellRenderer();

    int entityDrawCount = 0;

    

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
        
        gl::pushModelView();
        gl::disableAlphaBlending();
        color( ColorA(1,1,1,0.8));
        gl::ScopedModelMatrix m;
        gl::draw( miniMap->getColorTexture(), Rectf( 0,0,400,300) );
        gl::popModelView();
    }
    
    void drawPlayer( const vec2 & global, float direction, float radius, int bodyType )
    {
        
        entityDrawCount++;
        
        gl::color(Color(1,1,1));
        gl::ScopedBlendAlpha alpha;
        gl::pushModelView();
        gl::translate( toLocal(global, 1.0f) );
        gl::rotate( -direction + M_PI );
        gl::draw( playerImgs[bodyType] , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
        gl::popModelView();
        
        
        addToMiniMap( global, Color(1,1,1), radius, true);

    }
    
    
    void drawFriendly( const vec2 & global, float direction, float radius )
    {
        vec2 pos = toLocal(global,1);
        
        bool isOnScreen = onScreen( pos, radius + 10 );

        if( isOnScreen )
        {
            entityDrawCount++;
            gl::pushModelView();
            gl::translate( pos );
            gl::rotate( -direction + M_PI );
            gl::draw( friendlyImg , Rectf( -radius*2, -radius*2, radius*2, radius*2) );
            gl::popModelView();
            
        }
        
        
        addToMiniMap( global, Color(0.2,1,0.2), radius, isOnScreen);
    }
    
    void drawBubble( const vec2 & global , float radius, float depth )
    {
        entityDrawCount++;
        
        vec2 pos = toLocal(global, depth);
        
        bool isOnScreen = onScreen( pos, radius + 10 );
        
        if(isOnScreen){
            int img = 0;
            if( depth < 0.8 || depth > 1.2 ) img = 1;
            if( depth < 0.5 || depth > 1.5 ) img = 2;
            
            gl::color(ColorA( 1.0, 1.0, 1.0, depth * 0.6 ));
            gl::ScopedModelMatrix modelScope;
            gl::translate( pos );
            gl::draw( bubbleImgs[img], Rectf(-radius,-radius,radius,radius) );
        }
    }
    

    void drawSplash( const vec2 & global, float currentLife, float maxLife, float startSize )
    {
        float radius = startSize + currentLife;
        
        vec2 pos = toLocal(global,1);
        
        bool isOnScreen = onScreen( pos, radius + 10 );
        
        if(isOnScreen){
        
            entityDrawCount++;
            
        
            float opacityRadiusMod = 1.0f;
            
            opacityRadiusMod = (radius-14.0f) * 0.002f;
            if(opacityRadiusMod >= 1.0f) opacityRadiusMod = 1.0f;
            if(opacityRadiusMod <= 0.4f) opacityRadiusMod = 0.4f;
            
            float opacity = maxLife - currentLife;
            opacity *= opacityRadiusMod;
        

            gl::color(ColorA8u(255,255,255, opacity));
            gl::ScopedModelMatrix modelScope;
            gl::translate( pos );
            gl::draw( splashImg, Rectf(-radius, -radius, radius, radius) );
                
        }
    }
    
    void drawSpore( const vec2 & global, float radius, float depth, int sporeType )
    {
        entityDrawCount++;

        int blurLevel = 2;
        if( depth > 0.5) blurLevel = 1;
        if( depth > 0.8) blurLevel = 0;
        
        vec2 pos = toLocal( global, depth );
        bool isOnScreen = onScreen( pos, 30 );
        if( isOnScreen )
        {
        
            gl::pushModelView();
            gl::color( ColorA(1.0,1.0,1.0,1.0 * depth) );
            gl::ScopedModelMatrix modelScope;
            gl::translate( pos );
            gl::draw( sporeImgs[sporeType][blurLevel], Rectf( -radius, -radius, radius, radius ) );
            gl::popModelView();
            
        }
        
        addToMiniMap( global, Color(0.5,1,0.5), radius, isOnScreen);
    }
    
    void drawStarfish( const vec2 & global, array< vector<vec2>, 30 > feelers, float contacts )
    {
        
        
        vec2 pos = toLocal( global, 1 );
        bool isOnScreen = onScreen( pos, 100 );
        if( isOnScreen )
        {
            
            entityDrawCount++;
        
            for(int i = 0; i < feelers.size(); i++){
                Shape2d mShape = drawFeeler( feelers[i], 0.3f, 20.0f);
                gl::color(ColorA8u(220,120 - contacts * 0.08f,6 + contacts * 0.05f,70));
                gl::drawSolid( mShape );
                gl::color(ColorA8u(220,120 - contacts * 0.08f,6 + contacts * 0.05f,140));
                gl::draw( mShape );
            }
            
        }
        
        addToMiniMap( global, Color(1.0,0.9,0.1), 50, isOnScreen );
    
    }
    
    Shape2d drawFeeler( const vector<vec2> & positions, float tipWidth, float baseWidth )
    {
        vector<vec2> drawPositions;
        drawPositions.clear();
        for(int i = 0; i < positions.size() - 1; i++)
        {
            drawPositions.push_back( toLocal(positions[i], 1) );
            drawPositions.push_back( toLocal((positions[i] + positions[i + 1]) * 0.5f, 1) );
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
        for(int n = (int)drawPositions.size() - 1; n >= 1; n--)
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
     
        
        Shape2d mShape;
        mShape.moveTo( drawPositions.at(0) );
        for(int n = 0; n < drawPositions.size(); n++)
            mShape.lineTo( drawPositions.at(n) );
        mShape.close();
        

        return mShape;
        
    }
    
    void drawBeam( const vec2 & global, float depth, float op ){
        
        
        float x = toLocal( global, depth ).x;
        
        entityDrawCount++;
        gl::color(ColorA(1.0,1.0,1.0,op));
        gl::ScopedModelMatrix modelScope;
        gl::translate(vec2(x,0));
        

        float scale = beamImg->getHeight() / cinder::app::getWindowHeight();
        Rectf scaledRect = Rectf(0, 0, beamImg->getWidth() * scale, beamImg->getHeight() * scale );
        
        gl::draw( beamImg, scaledRect );
    }

    
    void drawJellyfish( const vec2 & global, float radius, vector<vector<vec2>> feelers, int jellyType, float counter, float baseWidth )
    {

        vec2 pos = toLocal( global, 1 );
        bool isOnScreen = onScreen( pos, 100 );
        
        if( isOnScreen )
        {
        
            entityDrawCount++;
            
            gl::ScopedBlendAdditive additive;
            
            switch( jellyType )
            {
                case 0:
                    gl::color(ColorA8u(150 + cos(counter) * 30,200 + sin(counter) * 30,200,155));
                    break;
                case 1:
                    gl::color(ColorA8u(150,200 + sin(counter) * 50,130,155));
                    break;
                case 2:
                    gl::color(ColorA8u(150,255,200,155));
                    break;
            }
            
            
            //draw the feelers
            for(int i = 0; i < feelers.size(); i++){
                Shape2d mShape = drawFeeler( feelers[i], 0.3f, baseWidth);
                gl::drawSolid( mShape );
            }
            
            //Draw texture body
            gl::ScopedBlendAlpha alpha;
            gl::color(Color(1,1,1));
            float width  = radius*2 + sin(counter)*2;
            float height = radius*2 + cos(counter)*3;
            gl::draw( jellyImgs[ jellyType ], Rectf(pos.x - width, pos.y - height, pos.x + width, pos.y + height )) ;
            
        }
        
        addToMiniMap( global, Color(0.6,0.3,1.0), radius, isOnScreen);

    }

    void drawPlankton( const vec2 & global, float depth, float radius, int planktonType, float rotation ){

        entityDrawCount++;
        
        
        vec2 pos = toLocal( global, depth );
        bool isOnScreen = onScreen( pos, 10 );
        
        if( isOnScreen )
        {
            color(1,1,1);
            gl::pushModelView();
            gl::translate( pos );
            gl::rotate( rotation );
            gl::draw( planktonImgs[planktonType], Rectf(-radius,-radius,radius,radius) );
            gl::popModelView();
        }
        
        addToMiniMap( global, Color(0.5,0.8,1), 4, isOnScreen);
    }

    
    void drawEgg( const vec2 & global, float ratio, float counter, vector<vec2> positions )
    {
        
        vec2 local = toLocal(global,1);
        
        
        bool isOnScreen = onScreen( local, ratio );
        
        if( isOnScreen )
        {
        
            entityDrawCount++;
         
            
            for( int i = 0; i < positions.size(); i++ )
            {
                positions[i] = toLocal(positions[i], 1);
            }
            
            gl::color(ColorA(1,1,1,0.5 + (sin(counter)*0.20f)) );
            gl::draw( eggImg, Rectf(local.x - ratio, local.y - ratio, local.x + ratio, local.y + ratio) );
            
            
            vector<vec2> drawPositions;
            for(int i = 0; i < positions.size(); i++)
            {
                int after = i + 1;
                if( after == (int)positions.size() ) after = 0;
                drawPositions.push_back( positions[i] );
                drawPositions.push_back( (positions[i] + positions[after]) * 0.5f);
            }
            
            //Average twice
            for(int n = 0; n < 2; n++)
            {
                for(int i = 0; i < drawPositions.size(); i++)
                {
                    int before = i - 1;
                    if( before < 0 ) before = (int)drawPositions.size()-1;
                    int after = i + 1;
                    if( after == drawPositions.size() ) after = 0;
                    vec2 p = (drawPositions.at(before) + drawPositions.at(after)) * 0.5f;
                    drawPositions.at(i) = (drawPositions.at(i) + p) * 0.5f;
                }
            }
            

            
            
            Shape2d mShape;
            mShape.moveTo( drawPositions.at(0) );
            for(int i = 1; i < drawPositions.size(); i++)
                mShape.lineTo( drawPositions.at(i) );
            mShape.close();
            
            gl::color( ColorA (0.7,0.25,0.15,0.5) );
            gl::drawSolid( mShape );
            gl::draw( mShape );
            
        }

        
        addToMiniMap( global, ColorA (0.7,0.25,0.15,0.5), ratio * 0.5f, isOnScreen);
    }
    
    void drawSpark( const vec2 & global, float radius, int type, vector<vec2> positions, vector<float> sizes )
    {
        vec2 pos = toLocal(global,1);
        
        bool isOnScreen = onScreen( pos, radius );
        
        if(isOnScreen){
        
            entityDrawCount++;
            
            if(type == 0){ gl::color(ColorA8u( 255         , 80+radius*3  , 20+radius*4,     220 ) ); }
            if(type == 1){ gl::color(ColorA8u( 108+radius*3, 5+radius*3   , 255        ,     220 ) ); }
            if(type == 2){ gl::color(ColorA8u( 75+radius*4 , 228	      , 178	       ,     220 ) ); }
            
            //draw a few circles at each Finite position
            for(int i = 0; i < positions.size(); i++){
                gl::ScopedModelMatrix modelScope;
                gl::translate( toLocal(positions[i],1) );
                float r = sizes[i];
                gl::draw( sparkImg, Rectf(-r, -r, r, r ));
            }
            
            gl::ScopedModelMatrix modelScope;
            gl::translate( pos );
            gl::draw( sparkImg, Rectf(-radius, -radius, radius, radius ));
            gl::draw( sparkImg, Rectf(-radius/2, -radius/2, radius/2, radius/2 ));
                
        }
        
        addToMiniMap( global, Color(1,1,0.2), radius, isOnScreen);
    }
    
    void drawUrchin( const vec2 & global, float radius, vector< vector<vec2> > & feelers )
    {
        
        vec2 local = toLocal(global,1);
        
        bool isOnScreen = onScreen( local, radius + 50 );
        
        if(isOnScreen){
        
            entityDrawCount++;
            
            gl::color(ColorA(1,0.5,1,0.5));
            
            //draw the feelers
            for(int i = 0; i < feelers.size(); i++){
                Shape2d mShape = drawFeeler( feelers[i], 0.3f, 4.0f );
                gl::drawSolid( mShape );
            }
            
            
            gl::color(Color(1,1,1));
            gl::draw( urchinImg, Rectf(local.x - radius*2.5, local.y - radius*2.5, local.x + radius*2.5, local.y + radius*2.5) );
                
        }
        
        addToMiniMap( global, Color(1,0.2,1.0), radius, isOnScreen);
    }
    
    void drawTail( vector<vec2> & positions, bool wideTail, bool fins, float width, float direction )
    {
      
        Path2d mPath;
        Path2d lPath;
        Path2d rPath;
        
        float rotMod = M_PI/2.5;
        
        vector<Path2d> finPaths;
        vector<Path2d> ridgePaths;
        
        for( int i = 0; i < positions.size(); i++ )
        {
            positions[i] = toLocal(positions[i], 1);
        }
        
        //initilase the path2Ds
        if(positions.size() > 0)
        {
            mPath.moveTo(positions[0]);
            if( wideTail ){
                lPath.moveTo(positions[0]);
                rPath.moveTo(positions[0]);
            }
        }
        
        
        //draw Line to each part of the tail
        int index = 0;
        
        for(int i = 1; i < positions.size(); i++ )
        {
            
            mPath.lineTo( positions[i] );
            
            if(wideTail){
                //width of the tail increases towards body
                //points on the 'wide' parts of the tail are points on the tail + a sin/cos vector
                float sizeMod = width * index / positions.size();
                lPath.lineTo(vec2(positions[i].x + sin(direction + rotMod) * sizeMod, positions[i].y + cos(direction + rotMod) * sizeMod));
                rPath.lineTo(vec2(positions[i].x + sin(direction - rotMod) * sizeMod, positions[i].y + cos(direction - rotMod) * sizeMod));
                
                
                //add ridges along the tail, keep these in a collection
                Path2d sPath;
                sPath.moveTo(lPath.getCurrentPoint());
                sPath.curveTo(mPath.getCurrentPoint(), mPath.getCurrentPoint(), rPath.getCurrentPoint());
                ridgePaths.push_back( sPath );
                
                
            }
            index++;
        }
        
        //add Fins
        if(fins){
            finPaths.clear();
            if(positions.size() > 5){
                Path2d fin1 = drawFin( positions[3],positions[2],positions[1],positions[2], width);
                gl::draw( fin1 );
            }
            if(positions.size() > 18){
                Path2d fin2 = drawFin( positions[15],positions[14],positions[12],positions[13], width);
                gl::draw( fin2 );
            }
        }
 
    
    
        gl::color(ColorA8u(255,255,255,255));
        gl::draw(mPath);
    
        //draw side paths if tail is wide
        if(wideTail == true){
            gl::draw(lPath);
            gl::draw(rPath);
            for(int i = 0; i < ridgePaths.size(); i++){
                gl::draw(ridgePaths.at(i) );
            }
        }
        

    }
    
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
        return !(local.x < -onScreenSize || local.x > getWindowWidth() + onScreenSize || local.y < -onScreenSize || local.y > getWindowHeight() + onScreenSize);
        
    }

    
    vec2 globalToMiniMap( vec2 position )
    {
//        return (((position-globals::offset) + (getWindowCenter())) * 0.2f) + vec2(400,300);
        return (position + vec2(3000,4000)) * 0.15f;
    }
    
    void addToMiniMap( const vec2 & position, const Color & color, float r, bool isGettingDrawn )
    {
        
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
    
    
};


#endif /* Images_hpp */
