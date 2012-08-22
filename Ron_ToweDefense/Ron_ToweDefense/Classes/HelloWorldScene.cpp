#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Tower.h"
#include "WayPoint.h"
#include "Enemy.h"

#include "CCDrawingPrimitives.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::loadTowerPositions()
{
    CCArray *towerPositions = CCArray::createWithContentsOfFileThreadSafe("TowersPosition.plist");
    towerBases = new CCArray(10);
    CCObject* objPos;
    CCARRAY_FOREACH(towerPositions, objPos){
        CCDictionary *towerPos = (CCDictionary*)objPos;
        CCSprite *towerBase = CCSprite::create("open_spot.png");  
        towerBase->setPosition(ccp(((CCString*)towerPos->objectForKey("x"))->intValue(), ((CCString*)towerPos->objectForKey("y"))->intValue()));
        towerBases->addObject(towerBase);
        this->addChild(towerBase);
    }

}

bool HelloWorld::canBuyTower(){
    if (playerGold - kTOWER_COST >=0)
        return true;
    
    return false;
}


void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
    
    
    for(CCSetIterator it = touches->begin();it!=touches->end();it++){
        CCTouch *curTouch = (CCTouch*)*it;
        CCPoint location = curTouch->locationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        
        if(towerBases!=NULL){
            for(int i=0;i<towerBases->count();i++){
                CCSprite *curSprite = (CCSprite*)towerBases->objectAtIndex(i);
                if(this->canBuyTower() && CCRect::CCRectContainsPoint(curSprite->boundingBox(), location)){
                    //We will spend our gold later.
                    playerGold -= kTOWER_COST;
                    CCString *strGold = CCString::createWithFormat("GOLD: %d", playerGold);
                    ui_gold_lbl->setString(strGold->getCString());

                    SimpleAudioEngine::sharedEngine()->playEffect("tower_place.wav");
                    
                    Tower *tower = Tower::nodeWithThegame(this, curSprite->getPosition());
                    towers->addObject(tower);
                    curSprite->setUserData(tower);
                }
            }
        }

    }
}

void HelloWorld::addWaypoints()
{
    waypoints = new CCArray();
    WayPoint *wayPoint1 = WayPoint::nodeWithThegame(this, ccp(420,35));
    waypoints->addObject(wayPoint1);
    
    WayPoint *wayPoint2 = WayPoint::nodeWithThegame(this, ccp(35,35));
    waypoints->addObject(wayPoint2);
    wayPoint2->nextWaypoint = wayPoint1;
    
    WayPoint *wayPoint3 = WayPoint::nodeWithThegame(this, ccp(35,130));
    waypoints->addObject(wayPoint3);
    wayPoint3->nextWaypoint = wayPoint2;
    
    WayPoint *wayPoint4 = WayPoint::nodeWithThegame(this, ccp(445,130));
    waypoints->addObject(wayPoint4);
    wayPoint4->nextWaypoint = wayPoint3;
    
    WayPoint *wayPoint5 = WayPoint::nodeWithThegame(this, ccp(445,220));
    waypoints->addObject(wayPoint5);
    wayPoint5->nextWaypoint = wayPoint4;
    
    WayPoint *wayPoint6 = WayPoint::nodeWithThegame(this, ccp(-40,220));
    waypoints->addObject(wayPoint6);
    wayPoint6->nextWaypoint = wayPoint5;
}


bool HelloWorld::circle(CCPoint circlePoint, float radius, CCPoint circlePointTwo, float radiusTwo)
{
    float xdif = circlePoint.x - circlePointTwo.x;
    float ydif = circlePoint.y - circlePointTwo.y;
    
    float distance = sqrt(xdif*xdif+ydif*ydif);
    if(distance <= radius+radiusTwo)
        return true;

    return false;
}
                                                                                        
void HelloWorld::ccFillPoly(CCPoint *poli, int points, bool closePolygon)
{
    ccDrawPoly(poli, points, closePolygon);
}

bool HelloWorld::loadWave()
{
    CCArray *waveData = CCArray::createWithContentsOfFileThreadSafe("Waves.plist");
    if(wave >= waveData->count()){
        return false;
    }
    
    CCArray *curWaveData = (CCArray*)waveData->objectAtIndex(wave);
    for(int i=0; i<curWaveData->count();i++){
        CCDictionary *enemyData = (CCDictionary*) curWaveData->objectAtIndex(i);
        Enemy *enemy = Enemy::nodeWithThegame(this);
        enemies->addObject(enemy);        
        enemy->schedule(schedule_selector(Enemy::doActivate), ((CCString*) enemyData->objectForKey("spawnTime"))->floatValue());
    }
    
    wave++;
    CCString *strWave = CCString::createWithFormat("WAVE: %d", wave);
    ui_wave_lbl->setString(strWave->getCString());
    
    return true;
}

void HelloWorld::enemyGotKilled()
{
    if(enemies->count()<=0){
        if(!this->loadWave()){
            printf("You Win!");
            CCDirector::sharedDirector()->replaceScene(CCTransitionSplitCols::transitionWithDuration(1, HelloWorld::scene()));
        }
    }
}

void HelloWorld::getHpDamage()
{
    SimpleAudioEngine::sharedEngine()->playEffect("life_lose.wav");
    playerHp--;
    CCString *strHP = CCString::createWithFormat("HP: %d", playerHp);
    ui_hp_lbl->setString(strHP->getCString());
    if (playerHp <=0) {
        this->doGameOver();
    }
}

void HelloWorld::doGameOver()
{
    if(!gameEnded){
        gameEnded = true;
        CCDirector::sharedDirector()->replaceScene(CCTransitionRotoZoom::transitionWithDuration(1, HelloWorld::scene()));
    }
}

void HelloWorld::awardGold(int gold)
{
    playerGold += gold;
    CCString *strGold = CCString::createWithFormat("GOLD: %d", playerGold);
    ui_gold_lbl->setString(strGold->getCString());
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    //-- initialize
    this->setTouchEnabled(true);

    CCSize wins = CCDirector::sharedDirector()->getWinSize();
    towers = new CCArray();
    //-- set bakcground
    CCSprite *background = CCSprite::create("Bg.png");
    this->addChild(background, -1); 
    background->setPosition(ccp(wins.width *0.5, wins.height *0.5));
    
   
    CCString *strWave = CCString::createWithFormat("WAVE: %d", wave);
    ui_wave_lbl = CCLabelBMFont::create(strWave->getCString(), "font_red_14.fnt");
    this->addChild(ui_wave_lbl, 10);
    ui_wave_lbl->setPosition(ccp(400, wins.height-12));
    ui_wave_lbl->setAnchorPoint(ccp(0,0.5));  
    
    this->loadTowerPositions();
    this->addWaypoints();
    
    
    enemies = new CCArray();
    this->loadWave();
    
    playerHp = 5;
    CCString *strHP = CCString::createWithFormat("HP: %d", playerHp);
    ui_hp_lbl = CCLabelBMFont::create(strHP->getCString(), "font_red_14.fnt");
    this->addChild(ui_hp_lbl);
    ui_hp_lbl->setPosition(ccp(35, wins.height-12));
    
    
    playerGold = 1000;
    CCString *strGold = CCString::createWithFormat("GOLD: %d", playerGold);
    ui_gold_lbl = CCLabelBMFont::create(strGold->getCString(), "font_red_14.fnt");
    this->addChild(ui_gold_lbl, 10);
    ui_gold_lbl->setPosition(ccp(135, wins.height-12));
    ui_gold_lbl->setAnchorPoint(ccp(0, 0.5));
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("8bitDungeonLevel.mp3",true) ;

    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
