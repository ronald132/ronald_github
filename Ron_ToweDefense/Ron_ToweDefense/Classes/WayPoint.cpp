//
//  WayPoint.cpp
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "WayPoint.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

WayPoint::WayPoint(){
    CCNode::CCNode();
}

WayPoint * WayPoint::node() {
    return new WayPoint() ;       
}

WayPoint * WayPoint::nodeWithThegame(HelloWorld* _game, CCPoint location){
    WayPoint *wayPoint = new WayPoint();
    wayPoint->initWithTheGame(_game, location);
    return wayPoint;
}

void WayPoint::initWithTheGame(HelloWorld* _game, CCPoint location){
    nextWaypoint = NULL;
    theGame = _game;
    this->setPosition(CCPointZero);
    myPosition = location;
    theGame->addChild(this);
}

void WayPoint::draw(){
    ccDrawColor4F(0, 255, 0, 255);
    ccDrawCircle(myPosition, 6, 360, 30, false);
    ccDrawCircle(myPosition, 2, 360, 30, false);
    
    if(nextWaypoint)
        ccDrawLine(myPosition, nextWaypoint->myPosition);
    CCNode::draw();
}
