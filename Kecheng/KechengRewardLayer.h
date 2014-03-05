//
//  KechengRewardLayer.h
//  Blackcat
//
//  Created by yangfei on 13-11-22.
//
//

#ifndef __Blackcat__KechengRewardLayer__
#define __Blackcat__KechengRewardLayer__

#include "Hanzi.h"
#include <vector>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;

class KechengRewardLayer : public CCLayer
{
public:
    static KechengRewardLayer* create(vector<Hanzi*>* hanziVector);
    virtual bool init();
    virtual void onEnter();
    
    void runRewardAnimate();
    void runRewardAnimateCallBack(CCObject* obj);
private:
    vector<Hanzi*>* m_hanziVector;
};

#endif /* defined(__Blackcat__KechengRewardLayer__) */
