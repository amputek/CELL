#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "EntityManager.h"
#include "Images.h"

using namespace ci::app;
using namespace std;

class Cellv0App : public AppBasic {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
    
	void update();
	void draw();

    bool paused = false;
    bool gameStart = false;
    bool quitScreen = false;
    bool yesSelected = false;
    bool noSelected = false;
    int gameFrames = 0;
    int quitOpacity = 0;
    
    EntityManager* entityManager;
    OSCManager* oscManager;
    Images* image; 
};


void Cellv0App::prepareSettings( Settings *settings ){
    settings->setWindowSize( 400, 300 );
    settings->setFrameRate(30.0f);
    settings->setTitle( "cell v0.1" );
}


void Cellv0App::setup(){
    image = new Images();
    entityManager = new EntityManager( image );    
    gl::enableAdditiveBlending( );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void Cellv0App::mouseDown( MouseEvent event ){
    
    if(gameStart == false){
        gameStart = true;
        hideCursor();
    }
    
    if(yesSelected == true){
        entityManager->quit();
        quit();
    }
    if(noSelected == true){
        quitScreen = false;
        paused = false;
        hideCursor();
        quitOpacity = 0;
    }
    
    cout << "fps: " << getAverageFps() << "\n";
}

void Cellv0App::keyDown( KeyEvent event ){
    if(event.getChar() == 'p'){
        if(paused == true){
            hideCursor();
            paused = false;
        } else {
            showCursor();
            paused = true;
        }
    }
    
    if(event.getChar() == 's'){
        entityManager->create("star");
    }
    
    if(event.getCode() == 27 || event.getChar() == 'q'){
        //entityManager->quit();
        //quit();
        if(quitScreen == false){
            paused = true;
            quitScreen = true;
            showCursor();
        } else {
            quitOpacity = 0;
            paused = false;
            quitScreen = false;
            hideCursor();
        }
    }
}

void Cellv0App::update(){
    if(gameStart == true && paused == false){
        entityManager->updateHero( getMousePos() );
        entityManager->update();
    }
    
    if(quitScreen == true && quitOpacity < 250){
        quitOpacity += 5;
    }
    
    if( (getMousePos() - Vec2f(getWindowWidth() * 0.3, getWindowHeight() * 0.65)).length() < 50){
        noSelected = true;
    } else {
        noSelected = false;
    }
    
    if( (getMousePos() - Vec2f(getWindowWidth() * 0.7, getWindowHeight() * 0.65)).length() < 50){
        yesSelected = true;
    } else {
        yesSelected = false;
    }
}

void Cellv0App::draw(){
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    if(gameStart == true){
        gameFrames++;
        entityManager->drawEntities();

        if(paused == true){
            gl::color( ColorA8u( 255,255,255,50) );
            gl::draw( *image->title(), Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
        }
    } else {
        gl::clear( Color(0,0,0) );
    }
    
    //draw splash screen ("CELL")
    if(gameStart == false || gameFrames < 125){
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        float g = sin( getElapsedFrames()*0.1 )*30;
        
        gl::color( ColorA8u( 220+g, 220-g, 255, 255 - (gameFrames*2) ) );
        gl::draw( *image->title(), Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
    }
    
    if(quitScreen == true){
      //  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        gl::enableAlphaBlending();
        gl::color( ColorA8u( 225, 255, 255, quitOpacity ) );
        gl::draw( *image->menu(),  Rectf(0,0,getWindowWidth(),getWindowHeight() ) );

        if(yesSelected == true){
            gl::draw( *image->menuy(), Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
        }
        if(noSelected == true){
            gl::draw( *image->menun(), Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
        }
    }
    
    gl::color( ColorA8u(255,255,255,200) );
    gl::drawSolidCircle( getMousePos(), 2 );

}



CINDER_APP_BASIC( Cellv0App, RendererGl )
