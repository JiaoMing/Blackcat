//
//  SummaryScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-4.
//
//

#include "SummaryScene.h"
#include "resource.h"


typedef enum{
    kOrientationLeft=0,
    kOrientationRight,
    kOrientationUp,
    kOrientationDown
}Orientation;

typedef struct _SumStruct
{
    string posName;
    string bgName;
    CCRect rect;
    Orientation orientation;
} SumStruct;

// 在这里配置每个场景要加载的资源
static SumStruct sum[] = {
	{
		"",
		"yindao_jieshao_1",
        CCRectZero,
        kOrientationLeft
	},
	{
		"yindao_jianjie_shujia",
		"yindao_jieshao_2",
        CCRectZero,
        kOrientationLeft
	},{
		"yindao_jianjie_xiaoxiongmao",
		"yindao_jieshao_3",
        CCRectZero,
        kOrientationDown
    },{
		"yindao_jianjie_heiban",
		"yindao_jieshao_5",
        CCRectZero,
        kOrientationUp
    },{
		"yindao_jianjie_heimao",
		"yindao_jieshao_6",
        CCRectZero,
        kOrientationRight
    },{
		"yindao_jianjie_chouti",
		"yindao_jieshao_7",
        CCRectZero,
        kOrientationRight
    },{
		"yindao_jianjie_bisai",
		"yindao_jieshao_4",
        CCRectZero,
        kOrientationLeft
    }
};

enum{
    kTagBack=0,
    kTagBackCover
};

CCScene* SummaryScene::scene(int key)
{
    CCScene* scene = CCScene::create();
    SummaryScene *layer = new SummaryScene();
    layer->m_key=key;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    scene->addChild(layer);
    return scene;
}


bool SummaryScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    string bgstr=sum[m_key].bgName;
    CCSprite* bg=CCSprite::create(bgstr.append(".png").c_str());
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    CCMenu* menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    CCSprite* back= CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItem* backItem=CCMenuItemSprite::create(back, back, this, menu_selector(SummaryScene::menuCallback));
    backItem->setPosition(S_RM->getPositionWithName("global_back"));
    menu->addChild(backItem);
    if (m_key==kSumHome) {
        for (int i=kSumShujia; i<kSumChouti+1; i++) {
            CCPoint point=S_RM->getPositionWithName(sum[i].posName.c_str());
            CCSize size=S_RM->getSizeWithName(sum[i].posName.c_str());
            sum[i].rect=CCRectMake(point.x, point.y-size.height, size.width, size.height);
        }
        
        backItem->setTag(kTagBack);
    }else{
        if(m_key==kSumXiaoxiongmao){
            CCPoint point=S_RM->getPositionWithName(sum[kSumBisai].posName.c_str());
            CCSize size=S_RM->getSizeWithName(sum[kSumBisai].posName.c_str());
            sum[kSumBisai].rect=CCRectMake(point.x, point.y-size.height, size.width, size.height);
        }
        backItem->setTag(kTagBackCover);
    }
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void SummaryScene::onEnter()
{
    CCLayer::onEnter();
    if(m_key==kSumHome||m_key==kSumXiaoxiongmao)S_DR->getTouchDispatcher()->addTargetedDelegate(this,0, true);
}

void SummaryScene::onExit()
{
    CCLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
    
    
    string bgstr=sum[m_key].bgName;
    CCTextureCache::sharedTextureCache()->removeTextureForKey(bgstr.append(".png").c_str());
}

bool SummaryScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    for (int i=kSumShujia; i<kSumBisai+1; i++) {
        if (sum[i].rect.containsPoint(pTouch->getLocation())) {
            m_touchedKey=i;
            return true;
        }
    }
    return false;
}

void SummaryScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void SummaryScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    for (int i=kSumShujia; i<kSumBisai+1; i++) {
        if (!sum[i].rect.equals(CCRectZero)&&sum[i].rect.containsPoint(pTouch->getLocation())) {
            if (m_touchedKey==i) {
                CCScene* scene;
                switch (sum[i].orientation) {
                    case kOrientationLeft:
                        scene=CCTransitionSlideInL::create(0.5, SummaryScene::scene(i));
                        break;
                    case kOrientationRight:
                        scene=CCTransitionSlideInR::create(0.5, SummaryScene::scene(i));
                        break;
                    case kOrientationUp:
                        scene=CCTransitionSlideInT::create(0.5, SummaryScene::scene(i));
                        break;
                    case kOrientationDown:
                        scene=CCTransitionSlideInB::create(0.5, SummaryScene::scene(i));
                        break;
                }
                S_DR->replaceScene(scene);
                break;
            }
        }
    }
}

void SummaryScene::menuCallback(cocos2d::CCObject *object){
    CCNode* node=(CCNode*)object;
    switch (node->getTag()) {
        case kTagBack:
        case kTagBackCover:
            this->keyBackClicked();
            break;
            
        default:
            break;
    }
    
}

void SummaryScene::keyBackClicked(){
    if (m_key==kSumHome) {
        S_DR->popScene();
    }else if(m_key==kSumBisai){
        S_DR->replaceScene(CCTransitionSlideInR::create(0.5, SummaryScene::scene(kSumXiaoxiongmao)));
    }else{
        CCScene* scene;
        switch (sum[m_key].orientation) {
            case kOrientationLeft:
                scene=CCTransitionSlideInR::create(0.5, SummaryScene::scene(kSumHome));
                break;
            case kOrientationRight:
                scene=CCTransitionSlideInL::create(0.5, SummaryScene::scene(kSumHome));
                break;
            case kOrientationUp:
                scene=CCTransitionSlideInB::create(0.5, SummaryScene::scene(kSumHome));
                break;
            case kOrientationDown:
                scene=CCTransitionSlideInT::create(0.5, SummaryScene::scene(kSumHome));
                break;
        }
        S_DR->replaceScene(scene);
    }
}

void SummaryScene::keyMenuClicked(){
    
}