//
//  ChooseLayer.cpp
//  SLG
//
//  Created by renshan on 14-10-8.
//
//

#include "ChooseLayer.h"


bool SeedChooseLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    currType = CropsType::NOTHING;
    
    auto bgSprite = Sprite::create("chooseBg.png");
    bgSprite->setAnchorPoint(Vec2(1, 0));
    this->addChild(bgSprite);
    
    wheat = Sprite::create("wheat.png");
    wheat->setAnchorPoint( Point(0, 0));
    wheat->setPosition(Point(0, 0));
    bgSprite->addChild(wheat);
     
    corn = Sprite::create("corn.png");
    corn->setPosition(Point(bgSprite->getContentSize().width/2, bgSprite->getContentSize().height/2));
    bgSprite->addChild(corn);
     
    carrot = Sprite::create("carrot.png");
    carrot->setAnchorPoint( Point(1, 1));
    carrot->setPosition(Point(bgSprite->getContentSize().width, bgSprite->getContentSize().height));
    bgSprite->addChild(carrot);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(SeedChooseLayer::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, wheat);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), corn);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), carrot);
    return true;
}


bool SeedChooseLayer::onTouchBegan(Touch *touch, Event *event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertTouchToNodeSpace(touch);
    Size size = target->getContentSize();
    Rect rect = Rect(0, 0, size.width, size.height);
    
    if (rect.containsPoint(locationInNode))
    {
        
        if (target == wheat)
        {
            currType = CropsType::WHEAT;
            target->setOpacity(180);
        }
        else if(target == corn)
        {
            currType = CropsType::CORN;
            target->setOpacity(180);
        }
        else if(target == carrot)
        {
            currType = CropsType::CARROT;
            target->setOpacity(180);
        }
        else{
            currType = CropsType::NOTHING;
        }
        return true;
    }
    return false;
}


bool RemoveLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto bgSprite = Sprite::create("chooseBg.png");
    bgSprite->setAnchorPoint(Vec2(1, 0));
    this->addChild(bgSprite);
    
    fork = Sprite::create("fork.png");
    fork->setPosition(Point(bgSprite->getContentSize().width/2, bgSprite->getContentSize().height/2));
    bgSprite->addChild(fork);
    
    remove = false;
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(RemoveLayer::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, fork);
    return true;
}


bool RemoveLayer::onTouchBegan(Touch *touch, Event *event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertTouchToNodeSpace(touch);
    Size size = target->getContentSize();
    Rect rect = Rect(0, 0, size.width, size.height);
    
    if (rect.containsPoint(locationInNode))
    {
        target->setOpacity(180);
        if (target == fork)
        {
            remove = true;
        }
        return true;
    }
    return false;
}

bool HarvestLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto bgSprite = Sprite::create("chooseBg.png");
    bgSprite->setAnchorPoint(Vec2(1, 0));
    this->addChild(bgSprite);
    
    harvestSprite = Sprite::create("hand.png");
    harvestSprite->setPosition(Point(bgSprite->getContentSize().width/2, bgSprite->getContentSize().height/2));
    bgSprite->addChild(harvestSprite);
    
    harvest = false;
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HarvestLayer::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, harvestSprite);
    return true;
}


bool HarvestLayer::onTouchBegan(Touch *touch, Event *event)
{
    auto target = static_cast<Sprite*>(event->getCurrentTarget());
    
    Point locationInNode = target->convertTouchToNodeSpace(touch);
    Size size = target->getContentSize();
    Rect rect = Rect(0, 0, size.width, size.height);
    
    if (rect.containsPoint(locationInNode))
    {
        target->setOpacity(180);
        if (target == harvestSprite)
        {
            harvest = true;
        }
        return true;
    }
    return false;
}
