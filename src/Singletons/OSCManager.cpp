#include "OSCManager.hpp"
#include "EntityManager.hpp"

OSCManager :: OSCManager(){
    host = "localhost";
    port = 57120;
    sender.setup(host, port);
    listener.setup(12080);
    //sendMessage("/confirm");
}

//for pre-created messages
void OSCManager :: sendMessage(osc::Message message, string addr){
    message.setAddress(addr);
    message.setRemoteEndpoint(host, port);
    sender.sendMessage(message);
}

//for messages wtihtout parameters
void OSCManager :: sendMessage(string addr){
    osc::Message message;
    message.setAddress(addr);
    message.setRemoteEndpoint(host, port);
    sender.sendMessage(message);
}

//recieve from SuperCollider
void OSCManager :: recieveMessage(){
    //continuously checks for new messages from SuperCollider
    
    while(listener.hasWaitingMessages()){
        osc::Message message;
        
        listener.getNextMessage(&message);
        if(message.getAddress() == "/friendlyPulse"){
            entities->pulseEvents->push_back( * new PulseEvent( FRIENDLY, message.getArgAsInt32(0) ) );
        }
        
        if(message.getAddress() == "/twirlPulse"){
            entities->pulseEvents->push_back( * new PulseEvent( URCHIN, message.getArgAsInt32(0) ) );
        }
        
        if(message.getAddress() == "/sparkPulse"){
            entities->pulseEvents->push_back( * new PulseEvent( SPARK, message.getArgAsInt32(0) ) );
        }
        
        if(message.getAddress() == "/audioFinishedLoading"){
            cout << "Audio finished loading! Start Game. Telling SC to start loop" << "\n";

            if( port != message.getRemotePort() )
            {
                port = message.getRemotePort();
                cout << "Switching Port to: " << port << "\n";
                sender.setup(host, port);
                listener.setup(12080);
            }
            
            sendMessage("/startLoop");
            initialised = true;
        } 
    }
    
  
}

//MESSAGES: method names should be self-explanatory

void OSCManager :: quit(){
    cout << "Qutting OSC" << "\n";
    sendMessage("/quitSC");
}

void OSCManager :: newSpark(int type){
    osc::Message msg;
    msg.addIntArg(type);
    sendMessage(msg, "/newSpark");
}

void OSCManager :: eatPlankton(int type, float pan, float dist){
    osc::Message msg;
    msg.addIntArg(type);
    msg.addFloatArg(pan);
    msg.addFloatArg(dist);
    sendMessage(msg, "/planktonEat");
}

void OSCManager :: eighthPlankton(){
    sendMessage("/levelUp");
}

void OSCManager :: sporeBoop(int health){
    osc::Message msg;
    msg.addIntArg(health);
    sendMessage(msg, "/sporeBoop");
}

void OSCManager :: egg(int inside){
    osc::Message msg;
    msg.addIntArg(inside);
    sendMessage(msg, "/egg");
}

void OSCManager :: urchin(float distance, int contact){
    if(distance > 500) return;
        
    urchinSendCounter += deltaTime;
        
    if( urchinSendCounter < 0.16f ) return;
    urchinSendCounter = 0.0f;
        
    osc::Message msg;
    msg.addFloatArg(distance);
    msg.addIntArg(contact);
    sendMessage(msg, "/urchin");
    
}

void OSCManager :: grass(bool contact){
    if(contact == true){
        sendMessage("/grass");
    }
}

void OSCManager :: surface(int where){
    osc::Message msg;
    msg.addIntArg(where);
    sendMessage(msg, "/surface");
}

void OSCManager :: bornFriendly( int id ){
    osc::Message msg;
    msg.addIntArg(id);
    sendMessage(msg, "/bornFriendly");
}

void OSCManager :: updateFriendly(int id, float pan, float dist){
    osc::Message msg;
    msg.addIntArg(id);
    msg.addFloatArg(pan);
    msg.addFloatArg(dist);
    sendMessage(msg, "/updateFriendly");
}

void OSCManager :: changeChord(){
    sendMessage("/changeChord");
}

void OSCManager :: jelly(int feelerContactCount, float dist){
    
    if( feelerContactCount == 0) return;
    
    jellySendCounter += deltaTime;
    
    if( jellySendCounter < 0.35f ) return;
    jellySendCounter = 0.0f;
    
    
    osc::Message msg;
    msg.addFloatArg(dist);
    msg.addIntArg( feelerContactCount );
    sendMessage(msg, "/jelly");
}

void OSCManager :: setDepth(float d){
    
    depthSendCounter += deltaTime;
    if( depthSendCounter < 1.0f ) return;

    depthSendCounter = 0.0f;
    osc::Message msg;
    msg.addFloatArg(d);
    sendMessage(msg, "/setDepth");
}
