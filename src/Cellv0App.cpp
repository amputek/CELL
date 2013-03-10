#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Mask.h"
#include "OSCManager.h"
#include "EntityManager.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"


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
    void debugInfo();
    
    EntityManager* entityManager;
    
    bool paused = false;
    bool gameStart = false;
    int gameFrames = 0;
    
    Mask* mask;
    OSCManager* oscManager;
    float r,g,b;
    
    gl::Texture* splashscreen;
    
};


void Cellv0App::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 600 );
    settings->setFrameRate(30.0f);
    settings->setTitle( "cell v0.1" );
}


void Cellv0App::setup(){
    oscManager = new OSCManager();
    entityManager = new EntityManager( oscManager );
  //  oscManager->entities = entityManager;
  //  oscManager->startLoop();
    
    splashscreen = new gl::Texture(loadImage(loadResource("cell-splashscreen.png") ) );
    
    mask = new Mask( gl::Texture( loadImage( loadResource( "mask-1.png" ) ) ) );
    
    gl::enableAdditiveBlending( );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void Cellv0App::mouseDown( MouseEvent event ){
    
    if(gameStart == false){
        gameStart = true;
        hideCursor();
    }
    
    cout << "fps: " << getAverageFps() << "\n";
    
}

void Cellv0App::keyDown( KeyEvent event ){
	if( event.getChar() == 'p' ){
        if(paused == true){
            hideCursor();
            paused = false;
        } else {
            showCursor();
            paused = true;
        }
        // paused = !paused;
    }
    
    if( event.getChar() == 'z' ){
 //       oscManager->duck();
    }
    
    if( event.getChar() == 'f' ){
        entityManager->createSpore();
    }
    
    if( event.getChar() == 'u' ){
        entityManager->createStarfish();
    }
}

void Cellv0App::update(){
    if(gameStart == true){
        if(paused == false){
            
            
            entityManager->updateHero( getMousePos() );
            entityManager->update();
            
            float depth = entityManager->getDepth();
            float widthMod = sin(entityManager->getX()*0.0001)*0.1;
            
            r = depth*0.06 -widthMod; //0.04
            g = depth*0.3 + widthMod;
            b = depth*0.8;  //0.4
            
         //   oscManager->setDepth( depth );
            
            mask->update(offset.x - entityManager->getX(),
                         offset.y - entityManager->getY(),
                         (entityManager->getDepth()+0.2) * 3.0);
            
        }
    }
}

void Cellv0App::draw(){
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    
    if(gameStart == true){
        gameFrames++;
        gl::clear( Color( r,g,b ) );
        entityManager->drawEntities();
        glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
        
        mask->draw();
        
        if(gameFrames < 125){
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            float g = sin( getElapsedFrames()*0.1 )*30;
            gl::color( ColorA8u( 220+g, 220-g, 255, (255.0-gameFrames*2) ) );
            gl::draw( *splashscreen, Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
           // gl::color(ColorA8u(0,0,0,(255.0-gameFrames)));
          //  gl::drawSolidRect( Rectf( 0,0,800,600 ) );
        }
        
        
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        
        gl::color( ColorA8u(255,255,255,200) );
        gl::drawSolidCircle( getMousePos(), 2 );
        
    } else {
        float g = sin( getElapsedFrames()*0.1 )*30;
        gl::clear( Color(0,0,0) );
        gl::color( ColorA8u( 220+g, 220-g, 255, 255 ) );
        gl::draw( *splashscreen, Rectf(0,0,getWindowWidth(),getWindowHeight() ) );
      // gl::drawStringCentered("CELL", (cinder::app::getWindowSize() / 2) );
        
    }
    
}


void Cellv0App::debugInfo(){
    gl::color(Color(1,1,1));
    //    gl::drawString("elapsed frames: " + toString(getElapsedFrames()), Vec2f( 10,20 ));
    gl::drawString("fps     : " + toString(getAverageFps()), Vec2f( 10,35 ));
    //    gl::drawString("global.y: " + toString(entityManager->getY()), Vec2f( 10,50 ));
    //    gl::drawString("depth   : " + toString(entityManager->getDepth()), Vec2f( 10,65 ));
    //    gl::drawString("global.x: " + toString(entityManager->getX()), Vec2f( 10,80 ));
    //    gl::drawString("environment: " + toString(entityManager->getEntities()), Vec2f( 10,95 ));
    //    gl::drawString("twirl counter: " + toString(entityManager->getTwirlCounter()), Vec2f( 10,110 ));
    //    gl::drawString("field counter: " + toString(entityManager->getFieldCounter()), Vec2f( 10,125 ));
    //    gl::drawString("colliders: " + toString(entityManager->getColliders()), Vec2f( 10,140 ));
}

CINDER_APP_BASIC( Cellv0App, RendererGl )
