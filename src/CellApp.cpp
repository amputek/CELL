#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Globals.hpp"
#include "Singletons/Renderer.hpp"
#include "Singletons/EntityManager.hpp"


using namespace ci;
using namespace ci::app;
using namespace std;

class CellApp : public App {
public:

    CellApp(){ }
	void setup() override;
    void cleanup() override;
    void keyDown( KeyEvent event ) override;
    void mouseMove( MouseEvent event ) override {
        hideCursor();
    }
	void update() override;
	void draw() override;
    vec2 mousePosition();
    
    void drawSplashScreens();
    void drawMenu();
    void drawCursor();
    
    int gameFrames = 0;

    float lastStepTime;
    
    EntityManager entityManager;
    CellRenderer image;
    
    
    
    bool gamePaused = false;
    bool gameStarted = true;

    
    bool IN_FULL_SCREEN = false;
    const bool RUN_AUDIO = true;
    
    
    float splashOpacity = 0.0f;
    
    
};



void CellApp::setup(){
    
    image = * new CellRenderer();
    entityManager = * new EntityManager( &image );

    //Open Cell-Audio.app
    if( RUN_AUDIO )
    {
        DataSourceRef rf = loadResource( "Cell-Audio.app");
        auto p = rf->getFilePath();
        string s = "open -g '" + p.string() + "'";
        system( s.c_str() );
    }
    
    IN_FULL_SCREEN = isFullScreen();
    
    gl::enableAdditiveBlending( );
    
    auto a = vec3(0,1,0);
    auto b = vec3(1,1,0);
    cout << dot(a,b) << endl;
    
}

void CellApp::cleanup()
{
    CI_LOG_I( "Cleaning up application." );
    entityManager.quit();
}


void CellApp::keyDown( KeyEvent event ){
 
    if( event.getChar() == 'f' )
    {
        IN_FULL_SCREEN = !IN_FULL_SCREEN;
        setFullScreen( IN_FULL_SCREEN );
        if( !IN_FULL_SCREEN ) setWindowSize(800, 600);
    }
    
    if( event.getChar() == KeyEvent::KEY_ESCAPE )
    {
        if(!gameStarted) return;
        gamePaused = !gamePaused;
    }

    if( event.getChar() == 'm')
    {
        DEBUG_MODE = !DEBUG_MODE;
        image.miniMapActive = DEBUG_MODE;
    }
}


void CellApp::update(){

    //Recalculate delta time
    float currentTime = app::getElapsedSeconds();
    deltaTime = currentTime - lastStepTime;
    if( deltaTime < 0.005f ) deltaTime = 0.005f;
    if( deltaTime > 0.1f ) deltaTime = 0.1f;
    lastStepTime = currentTime;
    
    if(gamePaused) return;
    if(!gameStarted) return;
    
    //update entity manager
    entityManager.updateHero( mousePosition(), getElapsedFrames() > 400 );
    entityManager.update(  );

}

void CellApp::draw(){


    image.refresh();


    gl::ScopedBlendAdditive additive;
    
    if(gameStarted){
        entityManager.drawEntities();
    } else {
         gl::clear( Color(0,0,0) );
    }

    drawSplashScreens();
    
    if( !DEBUG_MODE ) entityManager.environment.drawMask();
    
    gl::ScopedBlendAlpha alpha;
    
    drawCursor();

    drawMenu();

    if( DEBUG_MODE )
    {
        entityManager.printEntityStats();
        gl::drawString( "Framerate: " + to_string( roundf(getAverageFps()) ) + " Delta: " + to_string(deltaTime), vec2( 10.0f, 140.0f ) );
        gl::drawString( "Draw Count: " + to_string( image.entityDrawCount ), vec2( 10.0f, 160.0f ) );
    }

}



void CellApp::drawMenu()
{
    if( !gamePaused ) return;
    gl::ScopedBlendAlpha alpha;
    vec2 menuPos = vec2(40.0f,40.0f);
    gl::color( ColorA(0.0,0.0,0.0,0.2) );
    gl::drawSolidRect( Rectf( menuPos.x, menuPos.y, menuPos.x + 160.0f, menuPos.y + 105 ) );
    gl::color(1.0,1.0,1.0);
    gl::drawString("Toggle Full Screen: F", menuPos + vec2(20.0f,20.0f) );
    gl::drawString("Pause: ESC"           , menuPos + vec2(20.0f,40.0f) );
    gl::drawString("Quit: cmd-Q"          , menuPos + vec2(20.0f,60.0f) );
    gl::drawString("(C) Rob Dawson, 2017" , menuPos + vec2(20.0f,100.0f) );
    
    
}

void CellApp::drawSplashScreens()
{

    if( getElapsedFrames() > 800) return;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    
    if( getElapsedFrames() < 400 )
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
        image.drawTitle( splashOpacity );
    }
}

void CellApp::drawCursor()
{
    image.drawCursor( mousePosition() );
}

inline vec2 CellApp::mousePosition()
{
    return IN_FULL_SCREEN ? getMousePos() : getMousePos() - getWindowPos();
}


CINDER_APP( CellApp, RendererGl( RendererGl::Options().msaa( 4 ) ), [&]( App::Settings *settings )
{
    settings->setFullScreen();
    settings->setFrameRate(60.0f);
    settings->setHighDensityDisplayEnabled();
    settings->setTitle( "CELL v1.1" );
})
