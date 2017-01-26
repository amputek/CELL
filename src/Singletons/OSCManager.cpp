#include "OSCManager.hpp"
#include "EntityManager.hpp"

OSCManager :: OSCManager(){
    host = "localhost";
    port = 57120;
    sender.setup(host, port);
    listener.setup(12080);
    sendMessage("/confirm");
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
            entities->pulse("friendly", message.getArgAsInt32(0));
        }
        
        if(message.getAddress() == "/twirlPulse"){
            entities->pulse("urchin", 0);
        }
        
        if(message.getAddress() == "/sparkPulse"){
            entities->pulse("spark", message.getArgAsInt32(0));
        }
        
        if(message.getAddress() == "/confirmReply"){
            sendMessage("/startLoop");
            initialised = true;
        }
        
    }
    
    //Checks multiple ports for a responce from SuperCollider, which tends to initialise itself on either 57120 or 57121
    //Waits for a response from 57120 for two ticks. If no response, try 57121
    if(initialised == false){
        waitCount++;
        if(waitCount == 2){
            port = 57121;
            sender.setup(host, port);
            listener.setup(12080);
            sendMessage("/confirm");
        }
    }
}

//MESSAGES: method names should be self-explanatory


void OSCManager :: startLoop(){
    cout << "starting SC loop" << "\n";
    sendMessage("/startLoop");
}

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
    msg.addIntArg(4 - health);
    sendMessage(msg, "/sporeBoop");
}

void OSCManager :: egg(int inside){
    osc::Message msg;
    msg.addIntArg(inside);
    sendMessage(msg, "/egg");
}

void OSCManager :: urchin(float distance, int contact){
    if(distance < 600){
        osc::Message msg;
        msg.addFloatArg(distance);
        msg.addIntArg(contact);
        sendMessage(msg, "/urchin");
    }
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

void OSCManager :: newFriendly(){
    sendMessage("/newFriendly");
}

void OSCManager :: bornFriendly( int index ){
    osc::Message msg;
    msg.addIntArg(index);
    sendMessage(msg, "/bornFriendly");
}

void OSCManager :: updateFriendly(int index, float pan, float dist){
    osc::Message msg;
    msg.addIntArg(index);
    msg.addFloatArg(pan);
    msg.addFloatArg(dist);
    sendMessage(msg, "/updateFriendly");
}

void OSCManager :: changeChord(){
    sendMessage("/changeChord");
}

void OSCManager :: jelly(vector<bool> feelers, float dist){
    bool send = false;
    osc::Message msg;
    msg.addFloatArg(dist);
    for(int i = 0; i < feelers.size(); i++){
        if( feelers.at(i) == true ){
            send = true;
        }
        msg.addIntArg(feelers.at(i));
    }
    if(send == true){
        sendMessage(msg, "/jelly");
    }
}

void OSCManager :: setDepth(float d){
    osc::Message msg;
    msg.addFloatArg(d);
    sendMessage(msg, "/setDepth");
}
