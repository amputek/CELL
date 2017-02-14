#ifndef OSCManager_hpp
#define OSCManager_hpp

#include <iostream>
#include "OscSender.h"
#include "OscListener.h"

using namespace ci;
using namespace std;

//forward decleration of Entity Manager - stops Entity/OSC referencing each other
class EntityManager;

class OSCManager{
public:
    OSCManager();
    
    EntityManager* entities;
    void recieveMessage();
    
    //messages - self exlpanatory names
    void startLoop();
    void quit();
    void eatPlankton(int type, float pan, float dist);
    void eighthPlankton();
    void setDepth(float d);
    void urchin(float distance, int contact);
    void changeChord();
    void jelly(vector<bool> * feelers, float dist);
    void grass(bool contact);
    void sporeBoop(int health);
    void egg(int inside);
    void surface(int where);
    void floor();
    void newSpark(int type);
    void newFriendly();
    void bornFriendly( int index );
    void updateFriendly(int index, float pan, float dist);
    
private:
    void sendMessage(osc::Message message, string addr);
    void sendMessage(string addr);
    
    osc::Listener listener;
    osc::Sender sender;
    string host;
	int port;
    
    int waitCount = 0;
    bool initialised = false;
    
};


#endif
