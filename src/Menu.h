//
//  Menu.h
//  Cellv0
//
//  Created by Rob on 13/03/2013.
//
//

#ifndef __Cellv0__Menu__
#define __Cellv0__Menu__

#include <iostream>
#include "cinder/app/AppBasic.h"
#include "Globals.h"
#include "Images.h"

using namespace ci;
using namespace globals;

class GuiElement{
public:
    GuiElement(Vec2f loc, float size);
    bool update( Vec2f mousePos, bool mouse );
    void draw();
    
    Vec2f loc;
    bool over;
    bool hit;
    float size;
    bool holdable;
    gl::Texture* img;
};

class Button : public GuiElement{
public:
    Button(Vec2f loc, float size, gl::Texture* tex);
    void draw();
private:
    
};

class Slider : public GuiElement{
public:
    Slider(Vec2f loc, float size, float left, float right);
    void draw();
    void update(Vec2f mousePos, bool mouse);
private:
    float top;
    float bottom;
};

class Menu{
public:
    Menu(Images* img);
    string update( Vec2f mousePos );
    void draw();
    bool active;
    void activate(bool t);
    bool mouseDown;
    
    //getters
    bool resumed(){ return resuming; };
private:
    bool resuming = false;
    Vec2f offset = Vec2f(0,0);
    Images* images;
    gl::Texture* menuImg;
    Vec2f loc;
    Button* resumeButton;
    Button* quitButton;
    Slider* volume;
    std::vector<Button*> resButtons;

};

#endif /* defined(__Cellv0__Menu__) */
