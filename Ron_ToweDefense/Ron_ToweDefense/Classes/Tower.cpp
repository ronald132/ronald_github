//
//  Tower.cpp
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Tower.h"
#include "HelloWorldScene.h"
#include "Enemy.h"

using namespace cocos2d;

Tower::Tower(){
    CCNode::CCNode();
}

Tower * Tower::node() {
    return new Tower() ;       
}

Tower * Tower::nodeWithThegame(HelloWorld* _game, CCPoint location){
    Tower *tower = new Tower();
    tower->initWithTheGame(_game, location);
    return tower;
}

void Tower::initWithTheGame(HelloWorld* _game, CCPoint location){
    theGame = _game;
    attackRange = 70;
    damage = 10;
    fireRate = 1;
    chosenEnemy = NULL;
    
    mySprite = CCSprite::create("tower.png");
    this->addChild(mySprite);
    mySprite->setPosition(location);
    
    theGame->addChild(this);
    this->scheduleUpdate();    
}


void Tower::update(float dt){
    if(chosenEnemy){
        //We make it turn to target the enemy chosen
        CCPoint normalized = ccpNormalize(ccp(chosenEnemy->mySprite->getPositionX()-mySprite->getPositionX(),chosenEnemy->mySprite->getPositionY()-mySprite->getPositionY()));
        mySprite->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x))+90);
        if(!theGame->circle(mySprite->getPosition(), attackRange, chosenEnemy->mySprite->getPosition(), 1)){
            this->lostSightOfEnemy();
        }
    }else{
        for(int i=0; i<theGame->enemies->count(); i++){
            Enemy *enemy = (Enemy*)theGame->enemies->objectAtIndex(i);
            if(theGame->circle(mySprite->getPosition(), attackRange, enemy->mySprite->getPosition(), 1)){
                this->chosenEnemyForAttack(enemy);
                break;
            }
        }
    }
}

void Tower::chosenEnemyForAttack(Enemy *enemy){
    chosenEnemy = NULL;
    chosenEnemy = enemy;
    this->attackEnemy();
    enemy->getAttacked(this);
}

void Tower::attackEnemy(){
    this->schedule(schedule_selector(Tower::shootWeapon), fireRate);
}

void Tower::shootWeapon(){
    CCSprite *bullet = CCSprite::create("bullet.png");
    theGame->addChild(bullet);
    bullet->setPosition(mySprite->getPosition());
    bullet->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.1, chosenEnemy->mySprite->getPosition()),CCCallFuncN::actionWithTarget(this, callfuncN_selector(Tower::damageEnemy)), CCCallFuncN::actionWithTarget(this, callfuncN_selector(Tower::removeBullet)), NULL) ); 
}

void Tower::removeBullet(CCSprite *bullet){
    bullet->getParent()->removeChild(bullet, true);
}

void Tower::damageEnemy(){
    chosenEnemy->getDamaged(damage);
}


void Tower::targetKilled(){
    if(chosenEnemy)
        chosenEnemy = NULL;
    
    this->unschedule(schedule_selector(Tower::shootWeapon));
}

void Tower::lostSightOfEnemy(){
    chosenEnemy->gotLostSight(this);
    if(chosenEnemy)
        chosenEnemy = NULL;
    
    this->unschedule(schedule_selector(Tower::shootWeapon));
}



void Tower::draw(){
    ccDrawColor4F(255, 255, 255, 255);
    ccDrawCircle(mySprite->getPosition(), attackRange, 360, 30, false);
    CCNode::draw();
}



