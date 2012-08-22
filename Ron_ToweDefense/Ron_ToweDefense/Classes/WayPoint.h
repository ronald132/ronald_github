//
//  WayPoint.h
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Ron_ToweDefense_WayPoint_h
#define Ron_ToweDefense_WayPoint_h
#include "cocos2d.h"

USING_NS_CC;
class HelloWorld;
class WayPoint : public CCNode{
private:
    HelloWorld *theGame;
    
    void draw();
public:
    WayPoint *nextWaypoint;
    CCPoint myPosition;

    WayPoint();
    static WayPoint * node();
    
    static WayPoint * nodeWithThegame(HelloWorld* _game, CCPoint location);
    void initWithTheGame(HelloWorld* _game, CCPoint location); 
};



#endif
