//
//  WelcomeScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#include "WelcomeScene.h"
#include "resource.h"
#include "LoadingScene.h"
#include "InstallScene.h"

#define TAG_TOGGLE 1
#define TAG_OK 2

WelcomeScene::WelcomeScene(){
    S_RM->addSceneRes("WelcomeScene", "welcome");
}

WelcomeScene::~WelcomeScene(){
    S_RM->removeSceneRes("WelcomeScene");
}

CCScene* WelcomeScene::scene()
{
    CCScene* scene = CCScene::create();
    WelcomeScene *layer = WelcomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool WelcomeScene::init()
{
    if(! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }
    CCSprite* bg=CCSprite::createWithSpriteFrameName("beijing.png");
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    m_tableView = CCTableView::create(this,CCSizeMake(860, 530));
    m_tableView->setDirection(kCCScrollViewDirectionVertical);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setPosition(ccp(125, 100));
    m_tableView->setClippingToBounds(true);
    this->addChild(m_tableView);
    
    CCLabelTTF* label=CCLabelTTF::create("不再显示", "KaiTi.ttf", 25);
    label->setPosition(ccp(1125, 220));
    label->setColor(ccc3(0, 0, 0));
    this->addChild(label);
    
    CCSprite* fk01 = CCSprite::createWithSpriteFrameName("fangkuang01.png");
    CCMenuItem* fk01Item1=CCMenuItemSprite::create(fk01, fk01);
    CCSprite* fk02 = CCSprite::createWithSpriteFrameName("fangkuang02.png");
    CCMenuItem* fk01Item2=CCMenuItemSprite::create(fk02, fk02);
    CCMenuItemToggle* toggle=CCMenuItemToggle::createWithTarget(this, menu_selector(WelcomeScene::menuCallBack), fk01Item1,fk01Item2,NULL);
    toggle->setPosition(ccp(1050, 230));
    toggle->setTag(TAG_TOGGLE);
    
    CCSprite* okSprite=CCSprite::createWithSpriteFrameName("OK.png");
    CCMenuItemSprite* ok=CCMenuItemSprite::create(okSprite, okSprite, this, menu_selector(WelcomeScene::menuCallBack));
    ok->setPosition(ccp(1100, 140));
    ok->setTag(TAG_OK);
    
    CCMenu* menu=CCMenu::create(toggle,ok,NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    this->schedule(schedule_selector(WelcomeScene::scrollText), 0.05f);
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    
    return true;
}

void WelcomeScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCSize WelcomeScene::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(860, 2400);
}

CCTableViewCell* WelcomeScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    
    if (true) {
        cell = new CCTableViewCell();
        cell->autorelease();
        
        const char* text="                  致 家 长 的 公 开 信\n\n亲爱的家长，您好！\n\n　　寓教于乐是很多老师及家长都推崇的教育理念，如何通过游戏软件实现这个目标，是我们开发团队一贯的追求。《易迪汉字乐园》是我们开发团队历时6个月，精心研发的一款汉字学习软件。该软件针对4-6岁儿童进行开发，通过游戏的方式，让小朋友自主探索，快速认识并且学会书写汉字。\n\n　　为了达到更好的学习效果，我们还开发了专为本软件配套的纸质练习册、《幼小衔接800字》点读书、黑猫警长点读笔，这些配套产品将在软件正式发布时上市。\n\n　　感谢您下载并安装本软件的测试版，我们将为您提供优惠购买《易迪汉字乐园》正式产品的名额，欢迎关注我们的微信公众号“家长会”，填写相关信息，反馈您的意见及需求，帮助我们团队进一步完善产品。我们将选择10名热心的家长，免费赠送全套产品。\n\n　　您的需求，就是我们团队努力的方向。如何让孩子快乐的学习、成长，则是我们共同的目标。\n\n　　让我们一起努力！\n\n\n\n　　　　　　　　　　　　　　　　　　易迪汉字乐园项目组\n\n\n\n";
        CCLabelTTF* label1=CCLabelTTF::create(text, "KaiTi.ttf", 30,CCSizeMake(860, 0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentBottom);
        label1->setAnchorPoint(ccp(0, 0));
        label1->setPosition(ccp(0, 900));
        label1->setColor(ccc3(0, 0, 0));
        cell->addChild(label1);
        
        
        text="                关 于 儿 童 识 字 教 育\n\n　　随着现代生理学和心理学的发展，重视对孩子进行早期教育的家庭越来越多了。对幼儿的早期教育应以开发婴幼儿的潜能、智力为重点，以培养幼儿自觉阅读能力为中心，以培养个性品质为目标。\n\n　　要培养孩子的阅读兴趣和能力，首先要确保孩子识字。事实上，幼儿可以认识大量汉字是有一定的生理和心理依据的——汉字不同于拼音文字，具有更强的图画性，而这正好适合幼儿擅长于形象记忆和图像记忆的认知特点。进一步从理论上解释，拼音文字是音码文字，主要是在大脑左半球上发生作用；汉字则是音码、义码、形码的结合体，在大脑两半球上同时发生作用。所以音码文字可称是“单脑文字”，而汉字是“复脑文字”。汉字的表义性、方块性，恰巧迎合了幼儿右脑半球皮层发育比左侧更快的特性。\n\n　　识字一方面可以作为早期开发幼儿潜能的重要工具，同时也逐渐成为开发其智力和非智力因素的重要手段。幼儿在学习汉字的过程中必须仔细观察字形，用心记住字音，开动脑筋理解字义，这些活动无疑会有利于幼儿注意力、观察力、记忆力、想象力和思维能力的发展。\n\n　　除了对幼儿智力发展的巨大影响，汉字学习还帮助孩子培养阅读兴趣和良好的阅读习惯。研究表明，人的知识有８０％是从阅读中获得的，知识的丰富又会反过来促使孩子更好的思考，而认识一定数量的汉字是有效阅读的基础……\n\n                               —— 外研社•幼儿学前识字";
        CCLabelTTF* label2=CCLabelTTF::create(text, "KaiTi.ttf", 30,CCSizeMake(860, 0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentBottom);
        label2->setAnchorPoint(ccp(0, 0));
        label2->setPosition(ccp(0, 0));
        label2->setColor(ccc3(0, 0, 0));
        cell->addChild(label2);
        
    }
    else
    {
        
    }
    
    
    return cell;
}

unsigned int WelcomeScene::numberOfCellsInTableView(CCTableView *table)
{
    return 1;
}


void WelcomeScene::scrollText(float t){
    CCPoint point=m_tableView->getContentOffset();
    point=ccp(point.x,point.y+2);
    if (point.y>=0) {
        m_tableView->setContentOffset(ccp(point.x, 0));
        this->unschedule(schedule_selector(WelcomeScene::scrollText));
    }else{
        m_tableView->setContentOffset(point);
    }
}


void WelcomeScene::menuCallBack(CCObject* obj){
    int tag=((CCNode*)obj)->getTag();
    switch (tag) {
        case TAG_OK:
        {
            if (S_UD->getBoolForKey(IS_INSTALLED,false)) {
                S_RM->addSceneRes("LoadingScene","loading");
                S_DR->replaceScene(LoadingScene::scene("HomeScene"));
            }else{
                S_DR->replaceScene(InstallScene::scene());
            }
        }
            break;
            
        case TAG_TOGGLE:
        {
            CCMenuItemToggle* toggle=(CCMenuItemToggle*)obj;
            if(toggle->getSelectedIndex()==1){
                S_UD->setBoolForKey(IS_SHOW_GONGKAIXIN,false);
            }else{
                S_UD->setBoolForKey(IS_SHOW_GONGKAIXIN,true);
            }
            S_UD->flush();
        }
            break;
        default:
            break;
    }
    
}


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Java_com_kidsedu_KEHelper.h"
#endif
void WelcomeScene::keyBackClicked(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	showTipDialogJNI("退出", "确定退出程序吗?");
#endif
}

void WelcomeScene::keyMenuClicked(){
    
}