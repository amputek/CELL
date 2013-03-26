#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "EntityManager.h"
#include "Images.h"
#include "Menu.h"

using namespace ci::app;
using namespace std;

class Cellv0App : public AppBasic {
public:
    void prepareSettings(Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );
    void mouseUp( MouseEvent event );
    void keyDown( KeyEvent event );
    
	void update();
	void draw();

    bool gameStart = false;

    int gameFrames = 0;
    int quitOpacity = 0;
    
    EntityManager* entityManager;
    OSCManager* oscManager;
    Images* image;
    
    Menu* menu;
};


void Cellv0App::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 600 );
    settings->setFrameRate(30.0f);
    settings->setTitle( "cell v0.1" );
}


void Cellv0App::setup(){
    image = new Images();
    entityManager = new EntityManager( image );
    menu = new Menu(image);
    gl::enableAdditiveBlending( );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void Cellv0App::mouseDown( MouseEvent event ){
    
    if(gameStart == false){
        gameStart = true;
        hideCursor();
    }

    if(menu->active == true){
        menu->mouseDown = true;
    }
    
    cout << "fps: " << getAverageFps() << "\n";
}

void Cellv0App::mouseUp( MouseEvent event ){

    
    if(menu->active == true){
        menu->mouseDown = false;
    }
}

void Cellv0App::keyDown( KeyEvent event ){
    if(event.getChar() == 'p'){
        if(menu->active == true){
            hideCursor();
            menu->activate(false);
        } else {
            showCursor();
            menu->activate(true);
        }
    }
    
    if(event.getChar() == 's'){
        entityManager->create("star");
    }
    
    if(event.getChar() == 'e'){
        entityManager->create("egg");
    }
    
    if(event.getChar() == 'f'){
        entityManager->create("spores");
    }
    
    if(event.getChar() == 'j'){
        entityManager->create("jelly");
    }
    
    if(event.getChar() == 'u'){
        entityManager->create("urchin");
    }




}

void Cellv0App::update(){
    if(gameStart == true && menu->active == false){
        entityManager->updateHero( getMousePos() );
        entityManager->update();
    }

    if(menu->active == true){
        string response = menu->update( getMousePos() );
        if(response == "quit"){
            quit();
        }
        if(response == "resume"){
            hideCursor();
            menu->activate(false);
        }
    }
}

void Cellv0App::draw(){
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    if(gameStart == true){
        gameFrames++;
        entityManager->drawEntities();
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
    
    
    if(menu->active == true){
        menu->draw();
    }
    
    gl::color( ColorA8u(255,255,255,200) );
    gl::drawSolidCircle( getMousePos(), 2 );

}



CINDER_APP_BASIC( Cellv0App, RendererGl )
