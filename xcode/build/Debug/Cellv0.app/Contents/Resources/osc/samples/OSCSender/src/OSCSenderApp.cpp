
#include "OscSenderApp.h"

void OSCSenderApp::prepareSettings( Settings *settings ){
    settings->setWindowSize( 800, 600 );
   // settings->setFrameRate( 60.0f );
}

void OSCSenderApp::setup(){
    
   // gl::enableDepthRead();
   // gl::enableDepthWrite();
    
    menunodes.push_back( new MenuNode(Vec2f(50, 50), 15, "sine"));
    menunodes.push_back( new MenuNode(Vec2f(50, 100), 15, "pad"));
    menunodes.push_back( new MenuNode(Vec2f(50, 150), 15, "delay"));
    menunodes.push_back( new MenuNode(Vec2f(50, 200), 15, "filter"));
    
    gl::enableAdditiveBlending();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    linking = false;
    
    host = "localhost";
    port = 57121;
    sender.setup(host, port);
    listener.setup(12000);
    
    osc::Message message;
    message.setAddress("/start");
    message.setRemoteEndpoint(host, port);
    sender.sendMessage(message);
    
    //hideCursor();
    
    addNode(Vec2f(200,200), "sine");
    
    
    for(int i = 0; i < 25; i++){
        
    }

    
};

void OSCSenderApp::mouseDown( MouseEvent event ){
    
    if( event.isRight() || event.isAltDown() ){
        
        //addNode(event.getPos(), "sine");
        osc::Message message;
        message.setAddress("/start");
        message.setRemoteEndpoint(host, port);
        sender.sendMessage(message);
        
    } else if( event.isShiftDown() == true){
        for(int i = 0; i < nodes.size(); i++){
            nodes.at(i)->resize(event.getPos());
            if(nodes.at(i)->state == "resizing"){
                osc::Message message;
                message.setAddress("/updateNodeSize");
                message.addIntArg(i);
                message.addFloatArg((nodes.at(i)->radius));
                message.setRemoteEndpoint(host, port);
                sender.sendMessage(message);
            }
        }
    } else if(event.isControlDown() == true){
        for(int i = 0; i < nodes.size(); i++){
            if(linking == false){
                Vec2f dir = event.getPos() - nodes.at(i)->loc;
                if(dir.length() < nodes.at(i)->radius){
                    linking = true;
                    //cannot use " links.push_back( Link( nodes.at(i), i) ) " as constructor ??
                    links.push_back( Link() );
                    links.back().node1 = nodes.at(i);
                    links.back().index1 = i;
                }
            }
        }
    } else {
        for(int i = 0; i < nodes.size(); i++){
            nodes.at(i)->click(event.getPos());
        }
        for(int i = 0; i < menunodes.size(); i++){
            menunodes.at(i)->click(event.getPos());
        }
    }
}

void OSCSenderApp::mouseUp( MouseEvent event ){
    
    // bool linkSuccesful = false;
    
    for(int i = 0; i < nodes.size(); i++){
        
        nodes.at(i)->unclick();
        
        if(linking == true && event.isControlDown() == true){
            Vec2f dir = event.getPos() - nodes.at(i)->loc;
            if(dir.length() < nodes.at(i)->radius){
                cout << "adding new link" << "\n";
                links.back().node2 = nodes.at(i);
                if(links.back().node1 == links.back().node2){
                    
                } else {
                    if(links.back().node1->type == "sine"){
                        
                        if(links.back().node2->type == "delay" || links.back().node2->type == "filter"){
                        
                                links.back().index2 = i;
                            links.back().state = "complete";
                            linking = false;
                            cout << "completing" << "\n";
                    
                            osc::Message message;
                            message.setAddress("/addLink");
                            message.addIntArg(links.back().index1);
                            message.addIntArg(links.back().index2);
                            message.setRemoteEndpoint(host, port);
                            sender.sendMessage(message);
                        }
                    }

                    
                }
            }
        }
    }
     
    for(int i = 0; i < menunodes.size(); i++){
        
        if(menunodes.at(i)->inCircle() == true){
            addNode(event.getPos(), menunodes.at(i)->type);
        }
        menunodes.at(i)->unclick();
    }
    
    if(linking == true){
        cout << "failed link" << "\n";
        linking = false;
        links.pop_back();
    }
}

void OSCSenderApp::keyDown( KeyEvent event ){
    if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
        osc::Message message;
        message.setAddress("/end");
        message.setRemoteEndpoint(host, port);
        sender.sendMessage(message);
    }
    
    
    if(event.getChar() == 'c'){
        hideCursor();
    }
    if(event.getChar() == 'v'){
        showCursor();
    }
}

void OSCSenderApp::update(){
    recieveMessage();
    
    cursor = "inactive";
    
    for(int i = 0; i < menunodes.size(); i++){
        menunodes.at(i)->update(getMousePos());
    }
    
    for(int i = 0; i < nodes.size(); i++){
        Node n = *nodes.at(i);
        
        nodes.at(i)->update(getMousePos());
        if(n.state != "inactive"){
            cursor = "hover";
        }
        
        //if the node is in anyway moving....
        if(nodes.at(i)->state == "dragging" || nodes.at(i)->velocity.length() > 0.05){
            splashes.push_back( Splash(nodes.at(i)->loc, nodes.at(i)->radius+3));
            
            for(int j = 0; j < nodes.size(); j++){
                if(i != j){
                    Vec2f accel = collide(*nodes.at(j), *nodes.at(i));
                    nodes.at(i)->addVelocity(accel);
                    nodes.at(j)->addVelocity(-accel);
                }
            }
            
            osc::Message message;
            message.setAddress("/updateNodePosition");
            message.addIntArg(i);
            message.addFloatArg(n.getDistance());
            message.addFloatArg((n.loc.x - center.x) / radius);
            message.addFloatArg((n.loc.y - center.y) / radius);
            message.addFloatArg((n.radius));
            message.setRemoteEndpoint(host, port);
            sender.sendMessage(message);
        }
    }
    
    for(int i = 0; i < links.size(); i++){
        
        if(links.at(i).state == "incomplete"){
            //links.at(i).mouseLoc = getMousePos();
            links.at(i).update(getMousePos());
        } else {
            links.at(i).update(getMousePos());
        }
    }
    
    r = sin( getElapsedSeconds() * 0.5 ) * 0.15f + 0.35f;
    
    
}

void OSCSenderApp::nodeTrigger(Node n){
    osc::Message message;
    message.setAddress("/bang");
    message.addFloatArg((n.loc.x - center.x) / radius);
    message.addFloatArg(n.radius);
    int a = (((n.loc.y - center.y) / radius) + 1.0) * 7;
    message.addIntArg(14-a);
    message.addStringArg(n.type);
    message.setRemoteEndpoint(host, port);
    sender.sendMessage(message);
    splashes.push_back( Splash(n.loc, n.radius+6));
    splashes.push_back( Splash(n.loc, n.radius+3));
    splashes.push_back( Splash(n.loc, n.radius));
}

void OSCSenderApp::addNode(Vec2f pos, string type){

    nodes.push_back( new Node(pos, type) );
    for(int i = 10; i < 30; i+=5){
        splashes.push_back( Splash(pos, i ));
    }
    
    osc::Message message;
    message.setAddress("/addNode");
    
    Vec2f dir = center - pos;
    
    
    message.addStringArg((nodes.back()->type));
    message.addFloatArg(dir.length());
    message.addFloatArg((pos.x - center.x) / radius);
    message.addFloatArg((pos.y - center.y) / radius);
    message.addFloatArg((nodes.back()->radius/10.0));
    message.addStringArg("/Users/Rob/Desktop/CMHits/amhit.wav");
    message.setRemoteEndpoint(host, port);
    sender.sendMessage(message);
    
}

void OSCSenderApp::recieveMessage(){
    
    while(listener.hasWaitingMessages()){
        osc::Message message;
        listener.getNextMessage(&message);
        
        cout << "recieving message" << "\n";
        if(message.getAddress() == "/nodeHit"){
            Node n = *nodes.at(message.getArgAsInt32(0));
            splashes.push_back( Splash( Vec2f(n.loc.x, n.loc.y), n.radius+5));
        }
    }
    
}

Vec2f OSCSenderApp::collide(Node a, Node b){
    Vec2f distance = a.loc - b.loc;
    float mindist = b.radius + a.radius;
    if(distance.length() < mindist){
        // cout << "collide" << "\n";
        splashes.push_back( Splash((a.loc + b.loc) / 2, 5));
        for(int i = 0;i < 1; i++){
            float x = Rand::randFloat( -30.0f, 30.0f );
            Vec2f c = Vec2f(x, x);
            splashes.push_back( Splash(   (a.loc + b.loc + c) / 2, Rand::randInt( 2, 5 )));
        }
        // splashes.push_back( Splash((a.loc + b.loc + Rand::randFloat( 10.0f, 10.0f )) / 2, Rand::randInt( 2, 10 )));
        
        
        float angle = atan2(b.loc.x - a.loc.x, b.loc.y - a.loc.y);
        float targetX = a.loc.x + cos(angle) * mindist;
        float targetY = a.loc.y + sin(angle) * mindist;
        Vec2f accel = (Vec2f(targetX, targetY) - b.loc) * 0.02;
        return accel;
    }
    return Vec2f(0.0, 0.0);
}

void OSCSenderApp::draw(){
    
	gl::clear( Color(0,0,0));
    // gl::color(Color(255,255,255));
    gl::color( Color(0.1,0.3-r,0.3-r) );
    gl::drawSolidCircle(center, radius);
    
    
    glLineWidth(10);
    gl::color( ColorA8u(0,0,0,20));
    for(int i = 1; i < 10; i++){
        gl::drawStrokedCircle(center, (radius+5)-i);
    }

    gl::color( ColorA8u(255,255,255,10));
    
    for( list<Spark>::iterator p = sparks.begin(); p != sparks.end(); ++p ){
        p->draw();
    }
    

    for( list<Splash>::iterator p = splashes.begin(); p != splashes.end(); ++p ){
        p->draw();
        if(p->alive() == false){
            splashes.erase(p);
        }
    }
    
    for(int i = 0; i < nodes.size(); i++){
        nodes.at(i)->draw();
    }
    
    for(int i = 0; i < menunodes.size(); i++){
        menunodes.at(i)->draw();
    }
    
    for(int i = 0; i < links.size(); i++){
        
        //  cout << i;
        links.at(i).draw();
        
    }
    
    //  cout << linking << "\n";
    
    drawCursor();
    
   // string fps = toString(getAverageFps());
    
    gl::drawString(" " + toString(getAverageFps()), Vec2f( 50, 550 ));
    
    
}

void OSCSenderApp::drawCursor(){

    Vec2f mousePos = getMousePos();
    
    if(cursor == "hover"){
        gl::color(Color(0,0,0));
        glLineWidth(3);
        gl::drawLine(Vec2f(mousePos.x - 5, mousePos.y), Vec2f(mousePos.x + 5, mousePos.y));
        gl::drawLine(Vec2f(mousePos.x, mousePos.y - 5), Vec2f(mousePos.x, mousePos.y + 5));
        
        gl::color(Color(1,1,1));
        glLineWidth(1);
        gl::drawLine(Vec2f(mousePos.x - 4, mousePos.y), Vec2f(mousePos.x + 4, mousePos.y));
        gl::drawLine(Vec2f(mousePos.x, mousePos.y - 4), Vec2f(mousePos.x, mousePos.y + 4));
    } else {
        glLineWidth(1);
        gl::color(Color(1,1,1));
        gl::drawSolidCircle(mousePos, 3);
        gl::color(Color(0,0,0));
        gl::drawStrokedCircle(mousePos, 3);
        gl::drawSolidCircle(mousePos, 1);
    }
    
        
}


// This line tells Cinder to actually create the application
CINDER_APP_BASIC( OSCSenderApp, RendererGl )
