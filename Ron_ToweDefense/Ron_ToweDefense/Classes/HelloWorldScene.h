#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
    
private:
    CCArray *towerBases;
    CCArray *towers;
    int wave;
    int playerHp;
    int playerGold;
    
    bool gameEnded;
    CCLabelBMFont *ui_wave_lbl;
    CCLabelBMFont *ui_hp_lbl;
    CCLabelBMFont *ui_gold_lbl;
    
    void loadTowerPositions();
    void addWaypoints();
    void ccFillPoly(CCPoint *poli, int points, bool closePolygon);
    void doGameOver();
    bool loadWave();
    bool canBuyTower();
public:
    CCArray *waypoints;
    CCArray *enemies;

    bool circle(CCPoint circlePoint, float radius, CCPoint circlePointTwo, float radiusTwo);
    void enemyGotKilled();
    void getHpDamage();
    void awardGold(int gold);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_CREATE_FUNC(HelloWorld);
    
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

};

#endif // __HELLOWORLD_SCENE_H__
