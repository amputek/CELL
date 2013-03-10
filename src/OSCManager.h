//
//  OSCManager.h
//  Cellv0
//
//  Created by Rob on 08/03/2013.
//
//

#ifndef __Cellv0__OSCManager__
#define __Cellv0__OSCManager__

#include <iostream>
#include "OscSender.h"
#include "OscListener.h"

using namespace ci;
using namespace std;

//forward decleration
class EntityManager;

class OSCManager{
public:
    OSCManager();
    
    void eatPlankton(float depth, int type, float radius);
    void eighthPlankton();
    
    void startLoop();
    void setDepth(float d);
    
    void urchin(int density, int size, int length);
    void puff(float dist);
    void changeChord();
    void jelly(vector<bool> feelers);
    void feeler(bool contact, int legnth);
    void sporeBoop(int health);
    void egg(int inside);
    void chimes();
    void surface(int where);
    void newSpark(int type);
    void duck();
    void siren(float d);
    void startManta();
    
    EntityManager* entities;
    void recieveMessage();
    
private:
    void sendMessage(osc::Message message, string addr);
    void sendMessage(string addr);
    
    osc::Listener listener;
    osc::Sender sender;
    string host;
	int port;
    
};


#endif /* defined(__Cellv0__OSCManager__) */
