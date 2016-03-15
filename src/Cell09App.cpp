#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Singletons/Images.hpp"
#include "Singletons/EntityManager.hpp"
#include "GameObjects/GameObject.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class Cell09App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    void drawSplashScreens();
    void drawCursor();
    
    int gameFrames = 0;
    bool gameStart = false;
    
    EntityManager* entityManager;
    Images* image;
    
};

void Cell09App::setup(){
    image = new Images();
    entityManager = new EntityManager( image );
    gl::enableAdditiveBlending( );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void Cell09App::mouseDown( MouseEvent event ){
    if(gameStart == false){
        gameStart = true;
        hideCursor();
    }
}

void Cell09App::update(){
    if(gameStart == true){
        entityManager->updateHero( getMousePos() - getWindowPos() );
        entityManager->update();
    }
}

void Cell09App::draw(){
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    if(gameStart == true){
        gameFrames++;
        entityManager->drawEntities();
    } else {
        gl::clear( Color(0,0,0) );
    }
    
    //draw the "CELL" splash screen and the instruction/hint
    if(gameFrames < 255){
        drawSplashScreens();
    }
    drawCursor();
}

void Cell09App::drawSplashScreens(){
    
    //draw splash screen ("CELL")

    if(gameStart == false || gameFrames < 25.5){
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        float g = sin( getElapsedFrames()*0.1 )*30;
        gl::color( ColorA8u( 220+g, 220-g, 255, 255 - (gameFrames*10) ) );
        gl::draw( *image->title(), Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
    }
    
    //draw the instructional splash screen "CELL will follow your cursor. Explore"
    if( gameStart == true && gameFrames < 127 ){
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        gl::enableAlphaBlending();
        gl::color( ColorA8u( 255,255,255, gameFrames*2 ) );
        gl::draw( *image->instructions(), vec2(100,0) );
    }
    
    //fade out instructional screen
    if(gameStart == true && gameFrames >= 127 && gameFrames <= 255 ){
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);        
        gl::enableAlphaBlending();
        gl::color( ColorA8u( 255,255,255, 255 - ((gameFrames-127) * 2) ) );
        gl::draw( *image->instructions(), vec2(100,0) );
    }
    
}

void Cell09App::drawCursor(){
    
    vec2 mousePos = getMousePos() - getWindowPos();
    
    showCursor();
    
    //hide the cursor if it's inside the windows boundaries
    if(mousePos.x > 0 && mousePos.x < getWindowWidth() && mousePos.y > 0 && mousePos.y < getWindowHeight() ){
        hideCursor();
    }
    
    //keeps the cursor inside the window boundaries - avoids confusion of losing a hidden cursor
    if(mousePos.x < 0){                 mousePos.x = 5;                     }
    if(mousePos.y < 0){                 mousePos.y = 5;                     }
    if(mousePos.x > getWindowWidth()){  mousePos.x = getWindowWidth() - 5;  }
    if(mousePos.y > getWindowHeight()){ mousePos.y = getWindowHeight() - 5; }
    
    //draw cursor
    gl::enableAlphaBlending();
    gl::color( ColorA8u(255,255,255,255) );
    gl::drawSolidCircle( mousePos, 2 );
    glLineWidth(1);
    gl::drawStrokedCircle( mousePos, 4 );
}


CINDER_APP( Cell09App, RendererGl, [&]( App::Settings *settings ) {
    settings->setWindowSize( 800, 600 );
    settings->setFrameRate(30.0f);
    settings->setTitle( ":::CELL:::" );
})
