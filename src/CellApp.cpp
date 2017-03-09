#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


#include "Globals.hpp"
#include "Singletons/Renderer.hpp"
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
    CellRenderer* image;
    
    
    bool inFullScreen = true;
    bool gamePaused = false;
    bool gameStarted = true;

    bool runAudio = true;
    
    
    float splashOpacity = 0.0f;
    
};

void CellApp::setup(){
    
    image = new CellRenderer();
    entityManager = new EntityManager( image );
    
    //Open Cell-Audio.app
    if( runAudio )
    {
        DataSourceRef rf = loadResource( "Cell-Audio.app");
        auto p = rf->getFilePath();
        string s = "open -g '" + p.string() + "'";
        system( s.c_str() );
    }
    
    inFullScreen = isFullScreen();
    

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


    image->refresh();


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

    gl::drawString( "Framerate: " + to_string( roundf(getAverageFps()) ) + " Delta: " + to_string(deltaTime), vec2( 10.0f, 10.0f ) );
//    gl::drawString( "Draw Count: " + to_string( entityDrawCount ), vec2( 10.0f, 30.0f ) );
//

}



void CellApp::drawMenu()
{
    if( !gamePaused ) return;
    gl::ScopedBlendAlpha alpha;
    vec2 menuPos = vec2(40.0f,40.0f);
    gl::color( ColorA(0.0,0.0,0.0,0.2) );
    gl::drawSolidRect( Rectf( menuPos.x, menuPos.y, menuPos.x + 160.0f, menuPos.y + 105 ) );
    gl::color(1.0,1.0,1.0);
    //gl::drawString("Decrease Volume: -"  , menuPos + vec2(20.0f,20.0f) );
    //gl::drawString("Increase Volume: +"  , menuPos + vec2(20.0f,40.0f) );
    gl::drawString("Toggle Full Screen: F", menuPos + vec2(20.0f,20.0f) );
    gl::drawString("Pause: ESC"           , menuPos + vec2(20.0f,40.0f) );
    gl::drawString("Quit: cmd-Q"          , menuPos + vec2(20.0f,60.0f) );
    gl::drawString("(C) Rob Dawson, 2017" , menuPos + vec2(20.0f,100.0f) );
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
        image->drawTitle( splashOpacity );
    }
}

void CellApp::drawCursor(){


    vec2 mousePos = mousePosition();

    //This is dumb-- but only way to get around Cinder cursor bug
    showCursor();
    hideCursor();

    image->drawCursor( mousePos );
}


CINDER_APP( CellApp, RendererGl( RendererGl::Options().msaa( 4 ) ), [&]( App::Settings *settings ) {
    settings->setFullScreen();
    settings->setWindowSize(800, 600);
    settings->setFrameRate(60.0f);
    settings->setHighDensityDisplayEnabled();
    settings->setTitle( "CELL v1.5" );
   // settings->setResizable(false);
    //settings->setBorderless();
})
