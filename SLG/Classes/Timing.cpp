//
//  Timing.cpp
//  SLG
//
//  Created by renshan on 14/12/31.
//
//

#include "Timing.h"

bool TimingLayer::init()
{
    if (!Layer::init())
	{
		return false;
	}
    counter= 1;
    percent = 100;
	auto progressBarBg = Sprite::create("progressBg.png");
    progressBarBg->setAnchorPoint(Vec2(0, 0));
	progressBarBg->setPosition(Vec2(0,  0 ));
	addChild(progressBarBg);
    
    progressBar = ProgressTimer::create(Sprite::create("progressBar.png"));
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Point(0, 0.5f));
	progressBar->setBarChangeRate(Point(1, 0));
	progressBar->setPercentage(percent);
	progressBar->setAnchorPoint(Point(0.5f, 0.5f));
    progressBar->setPosition(Point(progressBarBg->getContentSize().width / 2,  progressBarBg->getContentSize().height /2 ));
	progressBarBg->addChild(progressBar);
    
    nameLabel = Label::createWithBMFont("fonts/Font30.fnt"," ");
    nameLabel->setPosition(Vec2(progressBarBg->getContentSize().width / 2,  progressBarBg->getContentSize().height ));
    nameLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
    progressBarBg->addChild(nameLabel);
    
    timeLabel = Label::createWithBMFont("fonts/Font30.fnt","");
    timeLabel->setPosition(Vec2(progressBarBg->getContentSize().width / 2,  - progressBarBg->getContentSize().height ));
    timeLabel->setAnchorPoint(Vec2(0.5f, 0.0f));
    progressBarBg->addChild(timeLabel);
    this->schedule(schedule_selector(TimingLayer::updateProgressBar), 1.0f);
    return true;
}

void TimingLayer::setParam(CropsType type)
{
    switch (type) {
        case WHEAT:
            nameLabel->setString("小麦");
            growUpTime = 60;
            break;
        case CORN:
            nameLabel->setString("玉米");
            growUpTime = 120;
            break;
        case CARROT:
            nameLabel->setString("胡萝卜");
            growUpTime = 180;
            break;
            
        default:
            break;
    }
}
TimingLayer* TimingLayer::create(Vec2 pos, CropsType type)
{
    TimingLayer *pRet = new TimingLayer();
    if (pRet && pRet->init())
    {
        pRet->setTimingLayerPos(pos);
        pRet->setParam(type);
        
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }

}

void TimingLayer::updateProgressBar(float dt)
{
    counter++;
    percent = 100 - float(counter) / float(growUpTime ) * 100;
    if(percent <= 100 && percent > 0 )
    {
        progressBar->setPercentage(percent);
        timeLabel ->setString(std::to_string(growUpTime - counter) + " 秒");
    }
    else
    {
        this->setTimeOut( true);
        this->unschedule(schedule_selector(TimingLayer::updateProgressBar));
        this->removeFromParent();
    }
}
