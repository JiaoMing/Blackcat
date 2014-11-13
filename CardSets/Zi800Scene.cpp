//
//  Zi800Scene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-9-17.
//
//

#include "Zi800Scene.h"
#include "resource.h"
#include "CardManager.h"
//#include "Zuciju.h"
#include "Zczj.h"
#include "XieziCoverLayer.h"

using namespace cocostudio::timeline;

Zi800Scene::Zi800Scene(){
    m_hanzis=new vector<Hanzi*>();
    m_contentMap=new map<string, Content*>();
    m_isBgMusicStart=false;
    
    CCString* audioPath=CCString::createWithFormat("800zi/%d.mp3",CURRENT_ECARDPAGE->getid()-21);
    S_AE->preloadBackgroundMusic(audioPath->getCString());
}

Zi800Scene::~Zi800Scene(){
    DELETE_MODEL_VECTOR(Hanzi, m_hanzis);
    
    for (map<string,Content*>::iterator it = m_contentMap->begin(); it != m_contentMap->end(); it ++){
        if (NULL != it->second)
        {
            delete it->second;
            it->second = NULL;
        }
    }
    m_contentMap->clear();
    delete m_contentMap;
    m_contentMap=NULL;
    
}

CCScene* Zi800Scene::scene()
{
    CCScene *scene = CCScene::create();
    Zi800Scene *layer = Zi800Scene::create();
    scene->addChild(layer);
    return scene;
}

bool Zi800Scene::init(){
    if (!PageViewScene::init()) {
        return false;
    }
    
    CCSprite* spriteBg=CCSprite::createWithSpriteFrameName("tuce_bg.png");
    spriteBg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(spriteBg);
    
    m_rootNode=NodeReader::getInstance()->createNode("800zi/800zi.ExportJson");
    m_rootNode->setScale(W_SIZE.height/768);
    m_rootNode->setPositionX(W_SIZE.width/2-512);
    this->addChild(m_rootNode);
    
    TouchGroup* root=(TouchGroup*)m_rootNode->getChildren()->objectAtIndex(0);
    Widget* rootWidget=root->getWidgetByName("root");
    
    CCString* path=CCString::createWithFormat("%secards/%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),CURRENT_ECARDPAGE->getimg().c_str());
    UIImageView* tu=(UIImageView*)UIHelper::seekWidgetByName(rootWidget, "tu");
    tu->loadTexture(path->getCString());
    tu->setTouchEnabled(true);
    tu->addTouchEventListener(this,toucheventselector(Zi800Scene::touchEvent));
    
    Widget* item=(Widget*)UIHelper::seekWidgetByName(rootWidget, "Panel_Item");
    UIListView* listView=(UIListView*)UIHelper::seekWidgetByName(rootWidget, "ListView");
    
    string zis="";
    for (int i=0; i<S_CM->getECards()->size(); i++) {
        if (i>0) {
            zis.append(",");
        }
        ECard* ecard = (*S_CM->getECards())[i];
        zis.append("'").append(ecard->getzi()).append("'");
    }
    CLAUSE_INIT;
    CCString* where=CCString::createWithFormat("zi in (%s)",zis.c_str());
    whereClause.push_back(where->getCString());
    S_DM->findScrollData(m_hanzis,"id,zi,pinyinyindiao,bushou,bihuashu",whereClause, orderbyClause, groupByClause);
    
    for (int i=0; i<S_CM->getECards()->size(); i++) {
        ECard* ecard = (*S_CM->getECards())[i];
        Hanzi* hanzi=NULL;
        for (vector<Hanzi*>::iterator it = m_hanzis->begin(); it != m_hanzis->end(); it ++){
            hanzi=*it;
            if (ecard->gethid()==hanzi->getid()) {
                break;
            }
        }
        
        //        vector<Zuciju*>* zucijus=new vector<Zuciju*>();
        //        CLAUSE_INIT;
        //        CCString* where=CCString::createWithFormat("zid=%d and pinyinyindiao='%s'",hanzi->getid(),ecard->getpyyd().c_str());
        //        whereClause.push_back(where->getCString());
        //        orderbyClause.push_back("ci1 desc");
        //        S_DM->findScrollData(zucijus,"*",whereClause, orderbyClause, groupByClause);
        
        vector<Zczj*>* zczjs=new vector<Zczj*>();
        CLAUSE_INIT;
        CCString* where=CCString::createWithFormat("hid=%d and pyyd='%s'",hanzi->getid(),ecard->getpyyd().c_str());
        whereClause.push_back(where->getCString());
        S_DM->findScrollData(zczjs,"*",whereClause, orderbyClause, groupByClause);
        
        Widget* widget=item->clone();
        Widget* hanziKuang=UIHelper::seekWidgetByName(widget, "ImageView_zi");
        UILabel* hanziLabel=(UILabel*)UIHelper::seekWidgetByName(hanziKuang, "hanzi");
        hanziLabel->setFontName("KaiTi.ttf");
        hanziLabel->setText(hanzi->getzi().c_str());
        hanziLabel->setTag(i);
        hanziLabel->addTouchEventListener(this,toucheventselector(Zi800Scene::touchEvent));
        
        Widget* huise=UIHelper::seekWidgetByName(widget, "ImageView_huise");
        UILabel* pinyin=(UILabel*)UIHelper::seekWidgetByName(huise, "pinyin");
        pinyin->setText(ecard->getpyyd().c_str());
        UILabel* bushou=(UILabel*)UIHelper::seekWidgetByName(huise, "bushou");
        CCString* bushouStr=CCString::createWithFormat("%s部",hanzi->getbushou().c_str());
        bushou->setText(bushouStr->getCString());
        UILabel* bihuashu=(UILabel*)UIHelper::seekWidgetByName(huise, "bihuashu");
        CCString* bihuashuStr=CCString::createWithFormat("%d画",hanzi->getbihuashu());
        bihuashu->setText(bihuashuStr->getCString());
        //        if (zucijus->size()>0) {
        //            Zuciju* zuciju=(*zucijus)[0];
        //            UILabel* ci1=(UILabel*)UIHelper::seekWidgetByName(huise, "ci1");
        //            ci1->setText(zuciju->getci1().c_str());
        //            ci1->setTag(zuciju->getid());
        //            ci1->setFontName("KaiTi.ttf");
        //            ci1->addTouchEventListener(this, toucheventselector(Zi800Scene::touchEvent));
        //            UILabel* ci2=(UILabel*)UIHelper::seekWidgetByName(huise, "ci2");
        //            ci2->setText(zuciju->getci2().c_str());
        //            ci2->setTag(zuciju->getid());
        //            ci2->setFontName("KaiTi.ttf");
        //            ci2->addTouchEventListener(this, toucheventselector(Zi800Scene::touchEvent));
        //            Widget* tiao=UIHelper::seekWidgetByName(widget, "ImageView_tiao");
        //            UILabel* ju=(UILabel*)UIHelper::seekWidgetByName(tiao, "ju");
        //            ju->setTag(zuciju->getid());
        //            ju->setText(zuciju->getju().c_str());
        //            ju->setFontName("KaiTi.ttf");
        //            ju->addTouchEventListener(this, toucheventselector(Zi800Scene::touchEvent));
        //        }
        //        listView->pushBackCustomItem(widget);
        //        DELETE_MODEL_VECTOR(Zuciju, zucijus);
        
        
        if (zczjs->size()>0) {
            Zczj* zczj=(*zczjs)[0];
            
            if(zczj->getc1Cid()>0){
                Content* content=new Content();
                S_DM->getByKey(content, zczj->getc1Cid());
                if(content!=NULL){
                    UILabel* ci1=(UILabel*)UIHelper::seekWidgetByName(huise, "ci1");
                    ci1->setText(content->getMD());
                    ci1->setFontName("KaiTi.ttf");
                    ci1->addTouchEventListener(this, toucheventselector(Zi800Scene::touchEvent));
                    ci1->setTag(content->getid());
                    CCString* key=CCString::createWithFormat("ci1_%d",content->getid());
                    m_contentMap->insert(std::pair<std::string, Content*>(key->getCString(), content));
                }
            }
            
            if(zczj->getc2Cid()>0){
                Content* content=new Content();
                S_DM->getByKey(content, zczj->getc2Cid());
                if(content!=NULL){
                    UILabel* ci2=(UILabel*)UIHelper::seekWidgetByName(huise, "ci2");
                    ci2->setText(content->getMD());
                    ci2->setFontName("KaiTi.ttf");
                    ci2->addTouchEventListener(this, toucheventselector(Zi800Scene::touchEvent));
                    ci2->setTag(content->getid());
                    CCString* key=CCString::createWithFormat("ci2_%d",content->getid());
                    m_contentMap->insert(std::pair<std::string, Content*>(key->getCString(), content));
                }
            }
            
            if(zczj->getjuCid()>0){
                Content* content=new Content();
                S_DM->getByKey(content, zczj->getjuCid());
                if(content!=NULL){
                    Widget* tiao=UIHelper::seekWidgetByName(widget, "ImageView_tiao");
                    UILabel* ju=(UILabel*)UIHelper::seekWidgetByName(tiao, "ju");
                    ju->setText(content->getMD());
                    ju->setFontName("KaiTi.ttf");
                    ju->addTouchEventListener(this, toucheventselector(Zi800Scene::touchEvent));
                    ju->setTag(content->getid());
                    CCString* key=CCString::createWithFormat("ju_%d",content->getid());
                    m_contentMap->insert(std::pair<std::string, Content*>(key->getCString(), content));
                }
            }
        }
        listView->pushBackCustomItem(widget);
    }
    
    if (CURRENT_ECARDPAGENUMBER>=5) {
        CCLayer* cover=CCLayerColor::create(ccc4(0, 0, 0, 150));
        this->addChild(cover);
        CCPoint middle=S_RM->getJpgBgPosition();
        CCLabelTTF* label=CCLabelTTF::create("此内容暂未上线，请等待升级版本。", "KaiTi.ttf", 40);
        label->setPosition(ccp(middle.x,middle.y+20));
        cover->addChild(label);
//        CCSprite* suo=CCSprite::create("suo.png");
//        suo->setPosition(ccp(middle.x,middle.y-20));
//        this->addChild(suo);
    }
    
    return true;
}

void Zi800Scene::onEnter(){
    PageViewScene::onEnter();
}

void Zi800Scene::onExit(){
    PageViewScene::onExit();
}

void Zi800Scene::beforeReplace(){
    S_AE->stopBackgroundMusic();
}

void Zi800Scene::touchEvent(cocos2d::CCObject *sender, TouchEventType type){
    if (CURRENT_ECARDPAGENUMBER>=5)return;
    if (type ==TOUCH_EVENT_ENDED)
	{
        UILabel* label=(UILabel*)sender;
        int tag=label->getTag();
        if (strcmp(label->getName(), "tu")==0) {
            S_ALP->stop();
            
            if (m_isBgMusicStart) {
                if (S_AE->isBackgroundMusicPlaying()) {
                    S_AE->pauseBackgroundMusic();
                }else{
                    S_AE->resumeBackgroundMusic();
                }
            }else{
                CCString* audioPath=CCString::createWithFormat("800zi/%d.mp3",CURRENT_ECARDPAGE->getid()-21);
                S_AE->playBackgroundMusic(audioPath->getCString(),true);
                m_isBgMusicStart=true;
            }
            
        }else{
            //暂停朗读
            if (S_AE->isBackgroundMusicPlaying()) {
                S_AE->pauseBackgroundMusic();
            }
            
            if (strcmp(label->getName(), "hanzi")==0) {
                ECard* ecard = (*S_CM->getECards())[tag];
                Hanzi* hanzi=NULL;
                for (vector<Hanzi*>::iterator it = m_hanzis->begin(); it != m_hanzis->end(); it ++){
                    hanzi=*it;
                    if (ecard->gethid()==hanzi->getid()) {
                        break;
                    }
                }
                XieziCoverLayer* xieziCoverLayer=XieziCoverLayer::create(hanzi->getid(),NULL,"",ecard->getpyyd());
                xieziCoverLayer->setDelegate(this);
                this->addChild(xieziCoverLayer,INT_MAX);
            }else{
                CCString* key=CCString::createWithFormat("%s_%d",label->getName(),label->getTag());
                Content* content=m_contentMap->find(key->getCString())->second;
                string url=FileUtils::getContentFilePath(content->getfileUrl());
                S_ALP->play(url.c_str(),NULL);
            }
        }
    }
}
