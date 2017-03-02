#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Globals.hpp"
#include "Singletons/Images.hpp"
#include "Singletons/EntityManager.hpp"
#include "GameObjects/GameObject.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class CellApp : public App {
  public:
	void setup() override;
    void cleanup() override;
    void keyDown( KeyEvent event ) override;
	void update() override;
	void draw() override;
    vec2 mousePosition();
    
    void drawSplashScreens();
    void drawMenu();
    void drawCursor();
    
    int gameFrames = 0;

    float lastStepTime;
    
    EntityManager* entityManager;
    Images* image;
    
    
    bool inFullScreen = true;
    bool gamePaused = false;
    bool gameStarted = true;

    bool runAudio = false;
    
    
    float splashOpacity = 0.0f;
    
};

void CellApp::setup(){
    
    image = new Images();
    entityManager = new EntityManager( image );
    
    //Open Cell-Audio.app
    if( runAudio )
    {
        DataSourceRef rf = loadResource( "Cell-Audio.app");
        auto p = rf->getFilePath();
        string s = "open -g '" + p.string() + "'";
        system( s.c_str() );
    }
    
    
    gl::enableAdditiveBlending( );
    
}

void CellApp::cleanup()
{
    CI_LOG_I( "Cleaning up application." );
    entityManager->quit();
}


void CellApp::keyDown( KeyEvent event ){
    if( event.getChar() == 'f' )
    {
        inFullScreen = !inFullScreen;
        setFullScreen( inFullScreen );
        if( !inFullScreen ) setWindowSize(800, 600);
    }
    
    if( event.getChar() == KeyEvent::KEY_ESCAPE )
    {
        if(!gameStarted) return;
        gamePaused = !gamePaused;
    }
}

vec2 CellApp::mousePosition()
{
    if( inFullScreen )
    {
        return getMousePos();
    }
    else
    {
        return getMousePos() - getWindowPos();
    }
}

void CellApp::update(){


    float currentTime = app::getElapsedSeconds();
    deltaTime = currentTime - lastStepTime;
    if( deltaTime > 0.1f ) deltaTime = 0.1f;
    lastStepTime = currentTime;
    
    
    if(gamePaused) return;
    if(!gameStarted) return;
    
    entityManager->updateHero( mousePosition(), getElapsedFrames() > 400 );
    entityManager->update(  );

}

void CellApp::draw(){


    entityDrawCount = 0;

    gl::ScopedBlendAdditive additive;
    
    if(gameStarted){
        gameFrames++;
        entityManager->drawEntities();
    } else {
         gl::clear( Color(0,0,0) );
    }

    drawSplashScreens();
    
    
    entityManager->environment->drawMask();
    
    

    
    drawCursor();
    
    drawMenu();

//    gl::drawString( "Framerate: " + to_string( roundf(getAverageFps()) ) + " Delta: " + to_string(deltaTime), vec2( 10.0f, 10.0f ) );
//    gl::drawString( "Draw Count: " + to_string( entityDrawCount ), vec2( 10.0f, 30.0f ) );
//

}



void CellApp::drawMenu()
{
    if( !gamePaused ) return;
    gl::ScopedBlendAlpha alpha;
    vec2 menuPos = vec2(40.0f,40.0f);
    gl::color( ColorA(0.0,0.0,0.0,0.2) );
    gl::drawSolidRect( Rectf( menuPos.x, menuPos.y, menuPos.x + 160.0f, menuPos.y + 145 ) );
    gl::color(1.0,1.0,1.0);
    gl::drawString("Decrease Volume: -"  , menuPos + vec2(20.0f,20.0f) );
    gl::drawString("Increase Volume: +"  , menuPos + vec2(20.0f,40.0f) );
    gl::drawString("Pause: ESC"          , menuPos + vec2(20.0f,60.0f) );
    gl::drawString("Quit: cmd-Q"         , menuPos + vec2(20.0f,80.0f) );
    gl::drawString("(C) Rob Dawson, 2017", menuPos + vec2(20.0f,120.0f) );
}

void CellApp::drawSplashScreens()
{
    if( gameFrames > 800) return;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    
    if(gameFrames < 400)
    {
        if(splashOpacity < 1.0f ) splashOpacity += deltaTime * 0.3f;
        offset = vec2(-400,-2000);
    }
    else
    {
        splashOpacity -= deltaTime * 0.3f;
    }
    
    if( splashOpacity >= 0.0 )
    {
        double op = splashOpacity;
        if( op > 1.0f ) op  = 1.0f;
        gl::color( ColorA8u(op * 130,op * 228,op * 247,op * 255.0f) );
        
        Rectf rect = Rectf(-300,-300,300,300);
        vec2 pos = vec2(-400,-2000);
        
        
        gl::pushModelView();
        gl::translate( globals::localise( pos, 0.55f ) );
        gl::draw( image->title1,rect);
        gl::popModelView();
        gl::pushModelView();
        gl::translate( globals::localise( pos, 0.6f ) );
        gl::draw( image->title2,rect);
        gl::popModelView();
        gl::pushModelView();
        gl::translate( globals::localise( pos, 0.57f ) );
        gl::draw( image->title3, rect);
        gl::popModelView();
        gl::pushModelView();
        gl::translate( globals::localise( pos, 0.525f ) );
        gl::draw( image->title4, rect);;
        gl::popModelView();
    }
}

void CellApp::drawCursor(){


    vec2 mousePos = mousePosition();

    //This is dumb-- but only way to get around Cinder cursor bug
    showCursor();
    hideCursor();

    //draw cursor
    gl::ScopedBlendAlpha alpha;
    gl::color(1.0,1.0,1.0);
    gl::draw( image->cursorImg, Rectf( mousePos.x - 2, mousePos.y - 2, mousePos.x + 2, mousePos.y + 2) );
}


CINDER_APP( CellApp, RendererGl( RendererGl::Options().msaa( 8 ) ), [&]( App::Settings *settings ) {
    settings->setFullScreen();
    //settings->setWindowSize(800, 600);
    settings->setFrameRate(60.0f);
    settings->setHighDensityDisplayEnabled();
    settings->setTitle( "CELL v1.5" );
    settings->setResizable(false);
    settings->setAlwaysOnTop(true);
})
