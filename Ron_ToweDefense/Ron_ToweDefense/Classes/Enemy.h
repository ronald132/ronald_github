//
//  Enemy.h
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Ron_ToweDefense_Enemy_h
#define Ron_ToweDefense_Enemy_h

#include "cocos2d.h"


USING_NS_CC;

class WayPoint;
class Tower;
class HelloWorld;

class Enemy : public CCNode{
private:
    CCPoint myPosition;
    HelloWorld *theGame;
    WayPoint *destinationWaypoint;
    
    int maxHp;
    int currentHp;
    float walkingSpeed;
    bool active;
    
    void draw();
    void update(float dt);
    void getRemoved();
    

public:    
    CCArray *attackedBy;
    
    CCSprite *mySprite; 
    void getAttacked(Tower *attacker);
    void getDamaged(int damage);
    void gotLostSight(Tower *attacker);

    void doActivate();

    Enemy();
    static Enemy * node();
    
    static Enemy * nodeWithThegame(HelloWorld* _game);
    void initWithTheGame(HelloWorld* _game); 
};


#endif
