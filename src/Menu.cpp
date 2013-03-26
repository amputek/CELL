#include "Menu.h"

Menu :: Menu(Images* imgs){
    loc = Vec2f(160,120);
    offset = Vec2f(0,0);
    images = imgs;
    menuImg = images->menuback();
    resumeButton = new Button( offset  + Vec2f(495,330), 20, images->b_resume());
    quitButton = new Button( offset  + Vec2f(495,400), 20, images->b_quit());
    resButtons.push_back( new Button( offset + Vec2f(175,300), 20, images->res_six()));
    resButtons.push_back( new Button( offset + Vec2f(175,360), 20, images->res_eight()));
    resButtons.push_back( new Button( offset + Vec2f(175,420), 20, images->res_twelve()));
    
    volume = new Slider(offset + Vec2f(430,320), 10, offset.y + 100, offset.y+340);
    mouseDown = false;
    active = false;
}


string Menu :: update( Vec2f mousePos ){
    
    string response = "";
    
    if(resumeButton->update( mousePos, mouseDown ) == true){
        response = "resume";
    }
    
    if(quitButton->update( mousePos, mouseDown ) == true){
        response = "quit";
    }
    
    volume->update( mousePos, mouseDown );
    
    bool clicked = false;
    if(resButtons.at(0)->update(mousePos, mouseDown ) == true){
        setWindowSize(600,450);
        offset = Vec2f(-100,-75);
        clicked = true;
    }
    if(resButtons.at(1)->update(mousePos, mouseDown ) == true){
        setWindowSize(800,600);
        offset = Vec2f(0,0);
        clicked = true;
    }
    if(resButtons.at(2)->update(mousePos, mouseDown ) == true){
        setWindowSize(1200,900);
        offset = Vec2f(200,150);
        clicked = true;
    }
    
    if(clicked == true){
        resButtons.at(0)->loc = offset + Vec2f(175,300);
        resButtons.at(1)->loc = offset + Vec2f(175,360);
        resButtons.at(2)->loc = offset + Vec2f(175,420);
        resumeButton->loc = offset  + Vec2f(495,330);
        quitButton->loc = offset  + Vec2f(495,400);
        volume->loc = offset + Vec2f(430,320);
    }
    return response;
}


void Menu :: draw(){
    gl::enableAlphaBlending();
    
    gl::color(Color(1,1,1));

    gl::draw( *menuImg, offset);
    
    resumeButton->draw();
    quitButton->draw();
    volume->draw();
    for(int i = 0; i < 3; i++){
        resButtons.at(i)->draw();
    }

}

void Menu :: activate(bool t){
    active = t;
    mouseDown = false;
}


GuiElement :: GuiElement(Vec2f l, float s){
    loc = l;
    size = s;
    over = false;
    hit = false;
}

bool GuiElement :: update( Vec2f mousePos, bool mouse ) {
    over = false;
    hit = false;
    if( mousePos.x > loc.x && mousePos.y > loc.y && mousePos.x < loc.x + img->getWidth() && mousePos.y < loc.y +img->getHeight()){
        over = true;
        if (mouse == true) {
            hit = true;
            if (holdable == false) {
                mouse = false;
            }
        }
    }
    return hit;
}

Button :: Button(Vec2f loc, float size, gl::Texture* tex) : GuiElement(loc, size){
    img = tex;
}


void Button :: draw(){
    
    if(over == true){
        gl::color(Color(1,1,1));
    } else {
        gl::color(Color(0.7,0.7,0.7));
    }
    gl::draw( *img, loc );
    
   }

Slider :: Slider(Vec2f loc, float size, float l, float r) : GuiElement(loc, size){
    top = l;
    bottom = r;
    holdable = true;
}

void Slider :: update(Vec2f mousePos, bool mouse){
    if(mousePos.y > top && mousePos.y < bottom && abs(mousePos.x - loc.x) < 10){
        if(mouse == true){
            loc = Vec2f(loc.x, mousePos.y);
        }
    }
}


void Slider:: draw(){
    gl::color(Color(0.5,0.5,0.5));
    gl::drawLine( Vec2f(loc.x, top), Vec2f(loc.x, bottom));
    gl::drawSolidCircle( loc, size );
    if(over == true){
        gl::color(Color(1,1,1));
        gl::drawStrokedCircle(loc, size);
    }
    if(hit == true){
        gl::color(Color(1,1,1));
        gl::drawStrokedCircle(loc,size+3);
    }
}