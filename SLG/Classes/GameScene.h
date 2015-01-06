#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "ChooseLayer.h"
#include "Timing.h"
USING_NS_CC;
using namespace ui;

typedef enum
{
    GROUD = 1,
    GROUD_CROP = 2,
    CROP_HARVEST,
    OTHER,
} TileType;

class GameScene : public cocos2d::Layer
{
public:
    GameScene();
    ~GameScene();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    virtual bool init();
    void updatePress(float t);
    virtual void update(float dt);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuShopCallback(cocos2d::Ref* pSender, Widget::TouchEventType type);
    void SpriteCallback(cocos2d::Ref* pSender, Widget::TouchEventType type);
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event);
    void initUI();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    
private:
    
    cocos2d::Layer* mapLayer;
    Layer* shopLayer;
    cocos2d::Menu* shopMenu;
    cocos2d::Sprite* shopMenubg;
    bool comeOut;
    Layout* playerLayout;
    
    Layout* panel_shop;
    Button* shop_btn;
    Sprite* buyTarget;
    bool createTarget;
    
    Sprite* bgSprite;
    Vec2 bgOrigin;
    
    
    double deltax;    //目标x轴的改变值
    double deltay;    //目标y轴的改变值
    
    TMXTiledMap* map;
    bool canBliud;
    Vec2 currPos;
    Vec2 perPos;
    bool longPress;
    Vec2 touchObjectPos;
    TileType tileType;
    Vector<TimingLayer *> timingVector;
    
    Vec2 convertTotileCoord(Vec2 position);
    Vec2 convertToScreenCoord(Vec2 position);

    void createTimingLayer(CropsType type);
    void moveCheck(Vec2 position, int tag);
    void updateRipeCrop();
    void checkTileType();
    
    void checkAndCreate(Vec2 pos);
};

#endif // __GAME_SCENE_H__
