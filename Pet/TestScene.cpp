//
//  TestScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-2.
//
//

#include "TestScene.h"
#include "ResManager.h"

TestScene::TestScene()
{
    ResManager::sharedResManager()->addSceneRes("test","waijing");
}

TestScene::~TestScene()
{
    ResManager::sharedResManager()->removeSceneRes("test");
}

CCScene* TestScene::scene()
{
    CCScene *scene = CCScene::create();
    
    TestScene *layer = TestScene::create();
    
    scene->addChild(layer);
    
    return scene;
}


bool TestScene::init(){
    if(! CCLayer::init())
    {
        return false;
    }
    
    m_layer1=CCLayerColor::create(ccc4(0,174,204, 255));
    CCSprite* yun1=CCSprite::createWithSpriteFrameName("yun1.png");
    yun1->setPosition(ccp(890, 768-235));
    CCSprite* yun2=CCSprite::createWithSpriteFrameName("yun2.png");
    yun2->setPosition(ccp(1140, 768-130));
    CCSprite* yun3=CCSprite::createWithSpriteFrameName("yun3.png");
    yun3->setPosition(ccp(1500, 768-215));
    CCSprite* shan=CCSprite::createWithSpriteFrameName("shan.png");
    shan->setPosition(ccp(1520, 768-590));
    m_layer1->addChild(yun1);
    m_layer1->addChild(yun2);
    m_layer1->addChild(yun3);
    m_layer1->addChild(shan);
    m_layer1->setPosition(CCPointZero);
    this->addChild(m_layer1);
    
    m_layer2=CCLayer::create();
    CCSprite* cao3=CCSprite::createWithSpriteFrameName("cao3.png");
    cao3->setPosition(ccp(572, 768-616));
    m_layer2->addChild(cao3);
    CCSprite* shu=CCSprite::createWithSpriteFrameName("shu.png");
    shu->setPosition(ccp(335,768-386));
    m_layer2->addChild(shu);
    CCSprite* cao1=CCSprite::createWithSpriteFrameName("cao1.png");
    cao1->setPosition(ccp(291, 768-590));
    m_layer2->addChild(cao1);
    CCSprite* cao2=CCSprite::createWithSpriteFrameName("cao2.png");
    cao2->setPosition(ccp(1025, 768-573));
    m_layer2->addChild(cao2);
    m_layer2->setPosition(CCPointZero);
    this->addChild(m_layer2);
    
    m_layer3=CCLayer::create();
    CCSprite* caodi=CCSprite::createWithSpriteFrameName("caodi.png");
    caodi->setPosition(ccp(0, 0));
    caodi->setAnchorPoint(ccp(0, 0));
    m_layer3->addChild(caodi);
    m_layer3->setPosition(ccp(-200, 0));
    this->addChild(m_layer3);
    
    
    
    return true;
}



void TestScene::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-127, false);
}

void TestScene::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool TestScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
    return true;
}

void TestScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    CCPoint touchLocation = pTouch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	CCPoint lastLocation =pTouch->getPreviousLocationInView();
	lastLocation = CCDirector::sharedDirector()->convertToGL( lastLocation );
    
    float moveDistance = touchLocation.x - lastLocation.x;
	CCPoint moveto3=ccp(m_layer3->getPosition().x+moveDistance,m_layer3->getPosition().y);
	CCPoint moveto2=ccp(m_layer2->getPosition().x+moveDistance*.6,m_layer2->getPosition().y);
	CCPoint moveto1=ccp(m_layer1->getPosition().x+moveDistance*.2,m_layer1->getPosition().y);
	m_layer3->setPosition(moveto3);
	m_layer2->setPosition(moveto2);
	m_layer1->setPosition(moveto1);
    
}

void TestScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}




