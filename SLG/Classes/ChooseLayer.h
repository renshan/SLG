//
//  ChooseLayer.h
//  SLG
//
//  Created by renshan on 14-10-8.
//
//

#ifndef __SLG__ChooseLayer__
#define __SLG__ChooseLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

typedef enum
{
    NOTHING = 0,
    WHEAT,
    CORN ,
    CARROT
} CropsType;

class SeedChooseLayer: public Layer
{
public:
    virtual bool init() override;
    bool onTouchBegan(Touch *touch, Event *event);
    CREATE_FUNC(SeedChooseLayer);
    CC_SYNTHESIZE(CropsType, currType, CurrType);
    
private:
    Sprite* wheat;
    Sprite* corn;
    Sprite* carrot;
};

class RemoveLayer: public Layer
{
public:
    virtual bool init() override;
    bool onTouchBegan(Touch *touch, Event *event);
    CREATE_FUNC(RemoveLayer);
    CC_SYNTHESIZE(bool, remove, Remove);
    
private:
    Sprite* fork;
};

class HarvestLayer: public Layer
{
public:
    virtual bool init() override;
    bool onTouchBegan(Touch *touch, Event *event);
    CREATE_FUNC(HarvestLayer);
    CC_SYNTHESIZE(bool, harvest, Harvest);
    
private:
    Sprite* harvestSprite;
};

#endif /* defined(__SLG__ChooseLayer__) */
