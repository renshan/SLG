#include "GameScene.h"
#include "ChooseLayer.h"

#include "cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
#define SHOP_ITEM_LAYOUT_TAG    100
#define SEEDPANEL_TAG           888
#define HARVESTPANEL_TAG        889
#define REMOVEPANEL_TAG         890

const char* shop_textures[8] =
{
    "shopItem/Item1.png",
    "shopItem/Item2.png",
    "shopItem/Item3.png",
    "shopItem/Item4.png",
    "shopItem/Item5.png",
    "shopItem/Item6.png",
    "shopItem/Item1.png",
    "shopItem/Item2.png"
};

const char* move_textures[8] =
{
    "shopItem/moveItem1.png",
    "shopItem/moveItem2.png",
    "shopItem/moveItem3.png",
    "shopItem/moveItem4.png",
    "shopItem/moveItem5.png",
    "shopItem/moveItem6.png",
    "shopItem/moveItem1.png",
    "shopItem/moveItem2.png"
};

const char* move_textures_en[8] =
{
    "shopItem_en/Item1.png",
    "shopItem_en/Item2.png",
    "shopItem_en/Item3.png",
    "shopItem_en/Item4.png",
    "shopItem_en/Item5.png",
    "shopItem_en/Item6.png",
    "shopItem_en/Item1.png",
    "shopItem_en/Item2.png"
};

const char* shop_info[8] =
{
    "土地",
    "破树",
    "烂树",
    "烂草",
    "破屋",
    "破地",
    "婆婆",
    "破",
};

const int shop_money[8] =
{
    20,
    40,
    60,
    100,
    120,
    99,
    50,
    200,
};

GameScene::GameScene()
:buyTarget(NULL)
,comeOut(false)
,createTarget(false)
,deltax(0)
,deltay(0)
,longPress(false)
{
}

GameScene::~GameScene()
{
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(winSize.width - closeItem->getContentSize().width/2 ,
                                closeItem->getContentSize().height*12));
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    mapLayer = Layer::create();
    this->addChild(mapLayer,-1);
    
    bgSprite = Sprite::create("2.jpg");
    bgSprite->setAnchorPoint(Vec2::ZERO);
    bgSprite->setPosition(Vec2::ZERO);
    bgOrigin = Vec2(Vec2::ZERO);
    mapLayer->addChild(bgSprite);
    
    auto treeSprite = Sprite::create("1.png");
    treeSprite->setAnchorPoint(Vec2::ZERO);
    treeSprite->setPosition(Vec2::ZERO);
    treeSprite->setScale(2);
    bgSprite->addChild(treeSprite, 1);
    
    map = TMXTiledMap::create("mymap8.tmx");
    map->setAnchorPoint(Vec2::ZERO);
    map->setPosition(Vec2::ZERO);
    bgSprite->addChild(map);
    
    currPos = Vec2(Vec2::ZERO);
    perPos = currPos;

    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, bgSprite);
    
    scheduleUpdate();
    initUI();
    
    return true;
}

void GameScene::initUI()
{
    playerLayout = static_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Ui_3/Ui_1.json"));
    addChild(playerLayout, 10);
    
    panel_shop = dynamic_cast<Layout*>(playerLayout->getChildByName("panel_shop"));
    auto shop_scrollView = dynamic_cast<ScrollView*>(panel_shop->getChildByName("scrollview_shop"));
    
    for (int i = 0; i < shop_scrollView->getChildren().size(); ++i)
    {
        Layout* shop_layout = static_cast<Layout*>(shop_scrollView->getChildren().at(i));
        shop_layout->setTag(SHOP_ITEM_LAYOUT_TAG + i);
        
        ImageView* buy_Sprite = static_cast<ImageView*>(shop_layout->getChildByName("shopitem"));
        buy_Sprite->loadTexture(shop_textures[i]);
        buy_Sprite->addTouchEventListener(CC_CALLBACK_2(GameScene::SpriteCallback, this));
        
        TextField* info = static_cast<TextField*>(shop_layout->getChildByName("info"));
        info->setText(shop_info[i]);
        
        TextField* money = static_cast<TextField*>(shop_layout->getChildByName("money_image")->getChildByName("money"));
        money->setText(std::to_string(shop_money[i]));
    }
    shop_btn = dynamic_cast<Button*>(playerLayout->getChildByName("button_shop"));
    shop_btn->addTouchEventListener(CC_CALLBACK_2(GameScene::menuShopCallback, this));
    
    comeOut = false;
}

void GameScene::SpriteCallback(cocos2d::Ref* pSender, Widget::TouchEventType type)
{
    Size winSize = Director::getInstance()->getWinSize();
    Widget* widget = static_cast<Widget*>(pSender);
    Widget* parent = static_cast<Widget*>(widget->getParent());
    
    int tag = parent->getTag();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            widget->runAction( EaseElasticInOut::create( ScaleTo::create(0.1f, 1.5), 0.2f));
            break;
            
        case Widget::TouchEventType::MOVED:
            if(comeOut == true)
            {
                panel_shop->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                shop_btn->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(- panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                comeOut = false;
            }
            if( buyTarget== NULL ){
                
                buyTarget = Sprite::create(move_textures[tag - SHOP_ITEM_LAYOUT_TAG]);
                buyTarget->setAnchorPoint(Vec2(0.5f, 0.27f));
                bgSprite->addChild(buyTarget, 10);
            }
            else{
                Vec2 pos;
                pos.x = (widget->getTouchMovePos().x - bgOrigin.x)/bgSprite->getScale();
                pos.y = (widget->getTouchMovePos().y - bgOrigin.y)/bgSprite->getScale();
                
                this->moveCheck(pos, tag - SHOP_ITEM_LAYOUT_TAG);
            }
            break;
            
        case Widget::TouchEventType::ENDED:
            widget->runAction( EaseElasticInOut::create(ScaleTo::create(0.1f, 1), 0.2f));
            if(buyTarget != NULL)
            {
                buyTarget->removeFromParent();
                buyTarget= NULL;
            }
            
            canBliud = false;
            break;
            
        case Widget::TouchEventType::CANCELED:
            widget->runAction( EaseElasticInOut::create(ScaleTo::create(0.1f, 1), 0.2f));
            
            if( canBliud == true )
            {
                auto endPos =Vec2((widget->getTouchEndPos().x - bgOrigin.x)/bgSprite->getScale(), (widget->getTouchEndPos().y - bgOrigin.y)/bgSprite->getScale());
                map->getLayer("goodsLayer")->setTileGID(9 + tag - SHOP_ITEM_LAYOUT_TAG, convertTotileCoord( endPos));
                canBliud = false;
            }
            else{
                auto endPos =Vec2((widget->getTouchEndPos().x - bgOrigin.x)/bgSprite->getScale(), (widget->getTouchEndPos().y - bgOrigin.y)/bgSprite->getScale());
                auto coord = convertTotileCoord( endPos);
                auto screenPos = this->convertToScreenCoord(coord);
                auto tips = Sprite::create("tip.png");
                tips->setPosition(screenPos);
                bgSprite->addChild(tips);
                tips->runAction(Sequence::create(DelayTime::create(0.1f),
                                                 CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, tips)),
                                                 NULL));
            }
            if(buyTarget != NULL)
            {
                buyTarget->removeFromParent();
                buyTarget= NULL;
            }
            
            if(comeOut == false)
            {
                panel_shop->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                shop_btn->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                comeOut = true;
            }
            map->getLayer("tipsLayer")->removeTileAt(perPos);
            map->getLayer("tipsLayer")->removeTileAt(currPos);
            break;
        default:
            break;

    }
}

Vec2 GameScene::convertTotileCoord(Vec2 position)
{
    auto mapSize = map->getMapSize();

    auto tileWidth = map->getBoundingBox().size.width / map->getMapSize().width;
	auto tileHeight = map->getBoundingBox().size.height / map->getMapSize().height;
    
    int posx = mapSize.height - position.y / tileHeight + position.x / tileWidth - mapSize.width / 2;
    int posy = mapSize.height - position.y / tileHeight - position.x / tileWidth + mapSize.width / 2;
    
    return Point(posx, posy);
}

Vec2 GameScene::convertToScreenCoord(Vec2 position)
{
    auto mapSize = map->getMapSize();
    auto tileSize = map->getTileSize();
    auto tileWidth = map->getBoundingBox().size.width / map->getMapSize().width;
	auto tileHeight = map->getBoundingBox().size.height / map->getMapSize().height;
    
    auto variable1 = (position.x + mapSize.width / 2 - mapSize.height) * tileWidth * tileHeight ;
    auto variable2 = (-position.y + mapSize.width / 2 + mapSize.height) * tileWidth * tileHeight ;
    
    int posx = (variable1 + variable2) / 2 / tileHeight;
    int posy = (variable2 - variable1) / 2 / tileWidth;
    
    return Point(posx, posy);
}

void GameScene::moveCheck(Vec2 position, int tag)
{
    auto mapSize = map->getMapSize();
    auto tilePos = this->convertTotileCoord(position);
    
    canBliud = false;
    perPos = currPos;
    if( tilePos.x >= 0 && tilePos.x <= mapSize.width - 1 && tilePos.y >= 0 && tilePos.y <= mapSize.height - 1)
    {
        currPos = tilePos;
        int gid = map->getLayer("goodsLayer")->getTileGIDAt(tilePos);
        if (gid == 0)
        {
            buyTarget->setTexture(move_textures[tag]);
            canBliud = true;
        }
        else{
            buyTarget->setTexture(move_textures_en[tag]);
            canBliud = false;
        }
        
        auto screenPos = this->convertToScreenCoord(tilePos);
        buyTarget->setPosition(screenPos);
        
        if(perPos != currPos)
        {
            map->getLayer("tipsLayer")->removeTileAt(perPos);
            map->getLayer("tipsLayer")->setTileGID(17, currPos);
        }
    }
    else{
        buyTarget->setPosition(position);
        buyTarget->setTexture(move_textures_en[tag]);
        map->getLayer("tipsLayer")->removeTileAt(perPos);
        canBliud = false;
    }
    
}

void GameScene::checkAndCreate(Vec2 pos)
{
    map->getLayer("goodsLayer")->setTileGID(9, pos);
}

void GameScene::menuShopCallback(cocos2d::Ref* pSender, Widget::TouchEventType type)
{
    Size winSize = Director::getInstance()->getWinSize();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
        {
            shop_btn->runAction( EaseElasticInOut::create(Sequence::create( ScaleBy::create(0.1f, 2),ScaleBy::create(0.2f, 0.5f), NULL), 0.5f));
            
            auto seedPanel = this->getChildByTag(SEEDPANEL_TAG);
            if(seedPanel){
                this->removeChild(seedPanel);
            }
            auto removePanel = this->getChildByTag(REMOVEPANEL_TAG);
            if(removePanel){
                this->removeChild(removePanel);
            }
            auto harvestPanel = this->getChildByTag(HARVESTPANEL_TAG);
            if(harvestPanel){
                this->removeChild(harvestPanel);
            }
            for (TimingLayer* timingLayerTemp : timingVector)
            {
                timingLayerTemp->setVisible(false);
            }
        }
            break;
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
            if(comeOut == false)
            {
                panel_shop->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                shop_btn->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                comeOut = true;
            }else if(comeOut == true)
            {
                panel_shop->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                shop_btn->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(- panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
                comeOut = false;
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void GameScene::updatePress(float t)
{
    this->unschedule(schedule_selector(GameScene::updatePress));
    if(longPress)
    {
        log("长按，可在该处扩展出其他的操作面板，如移动");
        map->getLayer("tipsLayer")->removeTileAt(touchObjectPos);
        longPress = false;
    }
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
    Size winSize = Director::getInstance()->getWinSize();
    if(comeOut == true)
    {
        panel_shop->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
        shop_btn->runAction( EaseElasticOut::create(MoveBy::create(1, Vec2(-panel_shop->getContentSize().width / 3 * 2, 0)), 0.5f));
        comeOut = false;
    }
    auto seedPanel = this->getChildByTag(SEEDPANEL_TAG);
    if(seedPanel){
        this->removeChild(seedPanel);
    }
    auto removePanel = this->getChildByTag(REMOVEPANEL_TAG);
    if(removePanel){
        this->removeChild(removePanel);
    }
    auto harvestPanel = this->getChildByTag(HARVESTPANEL_TAG);
    if(harvestPanel){
        this->removeChild(harvestPanel);
    }
    for (TimingLayer* timingLayerTemp : timingVector)
    {
        timingLayerTemp->setVisible(false);
    }
    if(touches.size() == 1)
    {
        auto touch = touches[0];
        auto screenPos = touch->getLocation();
        
        auto mapSize = map->getMapSize();
        
        Vec2 pos;
        pos.x = (screenPos.x - bgOrigin.x)/bgSprite->getScale();
        pos.y = (screenPos.y - bgOrigin.y)/bgSprite->getScale();
        auto tilePos = this->convertTotileCoord(pos);
        
        if( tilePos.x >= 0 && tilePos.x <= mapSize.width - 1 && tilePos.y >= 0 && tilePos.y <= mapSize.height - 1)
        {
            int gid = map->getLayer("goodsLayer")->getTileGIDAt(tilePos);
            if (gid != 0)
            {
                touchObjectPos = tilePos;
                map->getLayer("tipsLayer")->setTileGID(17, tilePos);
                this->schedule(schedule_selector(GameScene::updatePress), 2);
                longPress = true;
                this->checkTileType();
            }
        }
    }
}

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
    auto winSize = Director::getInstance()->getWinSize();
    if(touches.size() > 1)
    {
        auto point1 = touches[0]->getLocation();
        auto point2 = touches[1]->getLocation();
        
        auto currDistance = point1.distance(point2);
        auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
        
        auto pointVec1 = point1  - bgOrigin;
        auto pointVec2 = point2  - bgOrigin;
        
        auto relMidx = (pointVec1.x + pointVec2.x) / 2 ;
        auto relMidy = (pointVec1.y + pointVec2.y) / 2 ;
        
        auto anchorX = relMidx / bgSprite->getBoundingBox().size.width;
        auto anchorY = relMidy / bgSprite->getBoundingBox().size.height;
        
        auto absMidx = (point2.x + point1.x) / 2 ;
        auto absMidy = (point2.y + point1.y) / 2 ;
        
        if(  bgOrigin.x > 0)
        {
            absMidx -= bgOrigin.x;
            bgOrigin.x = 0;
        }
        if( bgOrigin.x < -bgSprite->getBoundingBox().size.width + winSize.width )
        {
            absMidx +=  -bgSprite->getBoundingBox().size.width + winSize.width - bgOrigin.x;
            bgOrigin.x = -bgSprite->getBoundingBox().size.width + winSize.width;
        }
        if( bgOrigin.y > 0 )
        {
            absMidy -= bgOrigin.y;
            bgOrigin.y = 0;
        }
        if( bgOrigin.y < -bgSprite->getBoundingBox().size.height + winSize.height )
        {
            absMidy +=  -bgSprite->getBoundingBox().size.height + winSize.height - bgOrigin.y;
            bgOrigin.y = -bgSprite->getBoundingBox().size.height + winSize.height;
        }
        
        bgSprite->setAnchorPoint(Vec2(anchorX, anchorY));
        bgSprite->setPosition(Vec2(absMidx, absMidy));
        
        auto scale = bgSprite->getScale() * ( currDistance / prevDistance);
        scale = MIN(4,MAX(1, scale));
        bgSprite->setScale(scale);
        
        bgOrigin = Vec2(absMidx, absMidy) - Vec2(bgSprite->getBoundingBox().size.width * anchorX, bgSprite->getBoundingBox().size.height * anchorY) ;
    }
    else if(touches.size() == 1)
    {
        auto touch = touches[0];
        auto diff = touch->getDelta();
        auto currentPos = bgSprite->getPosition();
        auto pos = currentPos + diff;
        auto bgSpriteCurrSize = bgSprite->getBoundingBox().size;
        
        pos.x = MIN(pos.x, bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
        pos.x = MAX(pos.x, -bgSpriteCurrSize.width + winSize.width + bgSpriteCurrSize.width * bgSprite->getAnchorPoint().x);
        
        pos.y = MIN(pos.y, bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
        pos.y = MAX(pos.y, -bgSpriteCurrSize.height + winSize.height + bgSpriteCurrSize.height * bgSprite->getAnchorPoint().y);
     
        bgSprite->setPosition(pos);
        Vec2 off = pos - currentPos;
        bgOrigin += off;
        
        longPress = false;
        map->getLayer("tipsLayer")->removeTileAt(touchObjectPos);
        this->unschedule(schedule_selector(GameScene::updatePress));
    }
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event  *event)
{
    this->unschedule(schedule_selector(GameScene::updatePress));
    
    Size winSize = Director::getInstance()->getWinSize();
    if(touches.size() == 1)
    {
        auto touch = touches[0];
        auto screenPos = touch->getLocation();
        if(longPress)
        {
            log("短按，创建相应的操作面板");
            if(tileType == GROUD)
            {
                auto panel = SeedChooseLayer::create();
                panel->setPosition(screenPos);
                this->addChild(panel, 10, SEEDPANEL_TAG);
            }
            else if(tileType == GROUD_CROP)
            {
                for (int i = 0; i < timingVector.size(); i++)
                {
                    auto temp = timingVector.at(i);
                    auto pos = temp->getTimingLayerPos();
                    if( pos == touchObjectPos)
                    {
                        temp->setVisible(true);
                    }
                }
            }
            else if(tileType == CROP_HARVEST)
            {
                auto panel = HarvestLayer::create();
                panel->setPosition(screenPos);
                this->addChild(panel, 10, HARVESTPANEL_TAG);
            }
            else if(tileType == OTHER)
            {
                auto panel = RemoveLayer::create();
                panel->setPosition(screenPos);
                this->addChild(panel, 10, REMOVEPANEL_TAG);
            }
            
            map->getLayer("tipsLayer")->removeTileAt(touchObjectPos);
            longPress = false;
        }
    }
}

void GameScene::update(float dt)
{
    auto seedPanel = this->getChildByTag(SEEDPANEL_TAG);
    auto removePanel = this->getChildByTag(REMOVEPANEL_TAG);
    auto harvestPanel = this->getChildByTag(HARVESTPANEL_TAG);
    this->checkTileType();
    if(NULL != seedPanel && tileType == GROUD)
    {
        auto type = ((SeedChooseLayer*)seedPanel)->getCurrType();
        log("auto type = ;%u",type);
        auto screenPos = this->convertToScreenCoord(touchObjectPos);
        switch (type)
        {
            case WHEAT:
                map->getLayer("goodsLayer")->setTileGID(18, touchObjectPos);
                this->createTimingLayer(WHEAT);
                this->removeChild(seedPanel);
                break;
            case CORN:
                map->getLayer("goodsLayer")->setTileGID(20, touchObjectPos);
                this->createTimingLayer(CORN);
                this->removeChild(seedPanel);
                break;
            case CARROT:
                map->getLayer("goodsLayer")->setTileGID(22, touchObjectPos);
                this->createTimingLayer(CARROT);
                this->removeChild(seedPanel);
                break;
            default:
                break;
        }
    }
    if(NULL != removePanel && tileType == OTHER)
    {
        if(((RemoveLayer*)removePanel)->getRemove() == true)
        {
            map->getLayer("goodsLayer")->removeTileAt(touchObjectPos);
            this->removeChild(removePanel);
        }
    }
    if(NULL != harvestPanel && tileType == CROP_HARVEST)
    {
        if(((HarvestLayer*)harvestPanel)->getHarvest() == true)
        {
            map->getLayer("goodsLayer")->setTileGID(9, touchObjectPos);
            this->removeChild(harvestPanel);
        }
    }
    this->updateRipeCrop();
}

void GameScene::createTimingLayer(CropsType type)
{
    auto screenPos = this->convertToScreenCoord(touchObjectPos);
    auto timingLayer = TimingLayer::create(touchObjectPos, type);
    timingLayer->setPosition(screenPos);
    timingLayer->setVisible(false);
    bgSprite->addChild(timingLayer, 10);

    this->timingVector.pushBack(timingLayer);
}

void GameScene::updateRipeCrop()
{
    for (int i = 0; i < timingVector.size(); i++)
    {
        auto temp = timingVector.at(i);
        if( temp->getTimeOut())
        {
            auto pos = temp->getTimingLayerPos();
            auto gid = map->getLayer("goodsLayer")->getTileGIDAt( pos);
            switch (gid)
            {
                case 18:
                {
                    map->getLayer("goodsLayer")->setTileGID(19, pos);
                    tileType = TileType::CROP_HARVEST;
                }
                    break;
                case 20:
                {
                    map->getLayer("goodsLayer")->setTileGID(21, pos);
                    tileType = TileType::CROP_HARVEST;
                }
                    break;
                case 22:
                    map->getLayer("goodsLayer")->setTileGID(23, pos);
                    tileType = TileType::CROP_HARVEST;
                    break;
                default:
                    break;
            }
            timingVector.eraseObject(temp);
        }
    }
}

void GameScene::checkTileType()
{
    auto gid = map->getLayer("goodsLayer")->getTileGIDAt(touchObjectPos);
    switch (gid)
    {
        case 9:
            tileType = TileType::GROUD;
            break;
        case 18:
        case 20:
        case 22:
            tileType = TileType::GROUD_CROP;
            break;
        case 19:
        case 21:
        case 23:
            tileType = TileType::CROP_HARVEST;
            break;
        default:
            tileType = TileType::OTHER;
            break;
    }
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
