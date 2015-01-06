//
//  Timing.h
//  SLG
//
//  Created by renshan on 14/12/31.
//
//

#ifndef __SLG__Timing__
#define __SLG__Timing__

#include <iostream>

#include "cocos2d.h"
#include "ChooseLayer.h"

USING_NS_CC;

class TimingLayer: public Layer
{
public:
    virtual bool init() override;
    static TimingLayer* create(Vec2 pos, CropsType type);
    virtual void updateProgressBar(float dt);
    
    CC_SYNTHESIZE(bool, timeOut, TimeOut);
    CC_SYNTHESIZE(Vec2, timingLayerPos, TimingLayerPos);
    void setParam(CropsType type);
private:
    ProgressTimer* progressBar;
    Label* nameLabel;
    Label* timeLabel;

    int counter;
    int growUpTime;
    float percent;
};


#endif /* defined(__SLG__Timing__) */
