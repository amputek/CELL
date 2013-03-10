//
//  OscSenderApp.h
//  OSCSender
//
//  Created by Rob on 19/10/2012.
//
//

#ifndef OSCSender_OscSenderApp_h
#define OSCSender_OscSenderApp_h

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Color.h"
#include "cinder/Utilities.h"
#include "OscSenderApp.h"

#include "Bubble.h"
#include "Splash.h"
#include "Node.h"
#include "Link.h"
#include "MenuNode.h"

#include <list>
#include <vector>
#include "cinder/Rand.h"
#include "OscSender.h"
#include "OscListener.h"


using namespace ci;
using namespace ci::app;
using namespace std;

Vec2f center = Vec2f(400, 300);
int radius = 280;

// We'll create a new Cinder Application by deriving from the BasicApp class
class OSCSenderApp : public AppBasic {
public:
	void setup();
    void prepareSettings(Settings *settings);
    
	void mouseDown( MouseEvent event );
    void mouseUp( MouseEvent event );
    void keyDown( KeyEvent event );
    //  void keyUp( KeyEvent event );
    
	void update();
	void draw();
    void drawCursor();
    
    void nodeTrigger(Node n);
    
    Vec2f collide(Node a, Node b);
    
    void addNode(Vec2f pos, string type);
    
    void recieveMessage();
    
    bool linking;
    string cursor;
    float frameCount;
    float r;
    
    float rippleLocation;
    
    list<Spark> sparks;
    list<Splash> splashes;
    vector<Node*> nodes;
    vector<MenuNode*> menunodes;
    vector<Link> links;
    
    //OSC stuff
    osc::Listener listener;
    osc::Sender sender;
    std::string host;
	int port;
    
};


#endif
