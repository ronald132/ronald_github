//
//  Tower.h
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Ron_ToweDefense_Tower_h
#define Ron_ToweDefense_Tower_h

#define kTOWER_COST 300

#include "cocos2d.h"


USING_NS_CC;

class HelloWorld;
class Enemy;
class Tower : public CCNode{
private:

    int attackRange;
    int damage;
    float fireRate;
    HelloWorld *theGame;
    
    
    //attack of the towers
    bool attacking;
    Enemy *chosenEnemy;

    void update(float dt);
    void draw();
    
    void attackEnemy();
    void chosenEnemyForAttack(Enemy *enemy);
    void shootWeapon();
    void removeBullet(CCSprite *bullet);
    void damageEnemy();
    void lostSightOfEnemy();
public:
    CCSprite *mySprite; 
    void targetKilled();


    Tower();
    static Tower * node();
    
    static Tower * nodeWithThegame(HelloWorld* _game, CCPoint location);
    void initWithTheGame(HelloWorld* _game, CCPoint location); 
};

#endif
