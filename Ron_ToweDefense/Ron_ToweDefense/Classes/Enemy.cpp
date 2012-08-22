//
//  Enemy.cpp
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Enemy.h"
#include "HelloWorldScene.h"
#include "WayPoint.h"
#include "Tower.h"
#import "SimpleAudioEngine.h"

#define HEALTH_BAR_WIDTH 20
#define HEALTH_BAR_ORIGIN -10

using namespace cocos2d;
using namespace CocosDenshion;


Enemy::Enemy(){
    CCNode::CCNode();
}

Enemy * Enemy::node() {
    return new Enemy() ;       
}

Enemy * Enemy::nodeWithThegame(HelloWorld* _game){
    Enemy *enemy = new Enemy();
    enemy->initWithTheGame(_game);
    return enemy;
}

void Enemy::initWithTheGame(HelloWorld* _game){
    theGame = _game;

    attackedBy = new CCArray(5);
    maxHp = 40;
    currentHp = maxHp;
    
    active = false;
    walkingSpeed = 0.5;
    
    mySprite = CCSprite::create("enemy.png");
    this->addChild(mySprite);
    
    WayPoint *waypoint = (WayPoint*) theGame->waypoints->objectAtIndex(theGame->waypoints->count()-1);
    destinationWaypoint = waypoint->nextWaypoint;
    
    CCPoint pos = waypoint->myPosition;
    myPosition = pos;
    
    mySprite->setPosition(pos);
    
    theGame->addChild(this);
    
    this->scheduleUpdate();
}

void Enemy::doActivate()
{
    active = true;
}

void Enemy::getRemoved()
{
    for(int i=0; i<attackedBy->count(); i++){
        Tower *attacker = (Tower*)attackedBy->objectAtIndex(i);
        attacker->targetKilled();
    }
    
    this->getParent()->removeChild(this, true);
    theGame->enemies->removeObject(this);
    
    //Notify the game that we killed an enemy so we can check if we can send another wave
    theGame->enemyGotKilled();
}


void Enemy::getAttacked(Tower *attacker){
    attackedBy->addObject(attacker);
}

void Enemy::gotLostSight(Tower *attacker){
    attackedBy->removeObject(attacker);
}

void Enemy::getDamaged(int damage){
    SimpleAudioEngine::sharedEngine()->playEffect("laser_shoot.wav");

    currentHp -=damage;
    if(currentHp<=0){
        theGame->awardGold(200);
        this->getRemoved();
    }
}

void Enemy::update(float dt){
    if(!active)return;
    if(theGame->circle(myPosition, 1, destinationWaypoint->myPosition, 1)){
        if(destinationWaypoint->nextWaypoint){
            destinationWaypoint = destinationWaypoint->nextWaypoint;
        }else{
            theGame->getHpDamage();
            this->getRemoved();
        }
    }
    
    CCPoint targetPoint = destinationWaypoint->myPosition;
    
    
    float movementSpeed = walkingSpeed;
    CCPoint normalized = ccpNormalize(ccp(targetPoint.x-myPosition.x, targetPoint.y-myPosition.y));
    
    mySprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x)));
    
    myPosition = ccp(myPosition.x+normalized.x * movementSpeed, myPosition.y+normalized.y * movementSpeed);
    
    mySprite->setPosition(myPosition);
}

void Enemy::draw(){
    ccDrawColor4F(255, 0, 0, 255);
    //    CGPoint healthBarBack[] = {ccp(mySprite.position.x -10,mySprite.position.y+16),ccp(mySprite.position.x+10,mySprite.position.y+16),ccp(mySprite.position.x+10,mySprite.position.y+14),ccp(mySprite.position.x-10,mySprite.position.y+14)};
    
    CCPoint healthBarBack[] = {ccp(mySprite->getPositionX() -10,mySprite->getPositionY()+16),ccp(mySprite->getPositionX()+10,mySprite->getPositionY()+16),ccp(mySprite->getPositionX()+10,mySprite->getPositionY()+14),ccp(mySprite->getPositionX()-10,mySprite->getPositionY()+14)};
    ccDrawPoly(healthBarBack, 4, true);
    
    ccDrawColor4F(0, 255, 0, 255);
    CCPoint healthBar[] = {ccp(mySprite->getPositionX() + HEALTH_BAR_ORIGIN,mySprite->getPositionY()+16),ccp(mySprite->getPositionX()+HEALTH_BAR_ORIGIN+(float)(currentHp * HEALTH_BAR_WIDTH) / maxHp,mySprite->getPositionY()+16),ccp(mySprite->getPositionX()+HEALTH_BAR_ORIGIN+(float)(currentHp * HEALTH_BAR_WIDTH) / maxHp,mySprite->getPositionY()+14),ccp(mySprite->getPositionX()+HEALTH_BAR_ORIGIN,mySprite->getPositionY()+14)};
    ccDrawPoly(healthBar, 4, true);
    
    CCNode::draw();
}






