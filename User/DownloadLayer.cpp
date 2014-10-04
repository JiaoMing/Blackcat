//
//  DownloadLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-24.
//
//

#include "DownloadLayer.h"
#include "DownloadManager.h"


DownloadLayer::~DownloadLayer(){
    CCLog("~DownloadLayer");
}

bool DownloadLayer::init(){
    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 0))) {
        return false;
    }
    
    DownloadObject* dlObj=DownloadObject::create();
    dlObj->setUrl("http://www.kidsedu.com/blackcat/backup.zip");
    dlObj->setExpectFilename("backup.zip");
    dlObj->setFileTotalSize(60000000);
    dlObj->setTarget(this);
    dlObj->setSelector(callfuncO_selector(DownloadLayer::download));
    DownloadManager::sharedDownloadManager()->download(dlObj);
    CCLabelTTF* ttf1=CCLabelTTF::create("backup.zip进度:0%", "KaiTi.ttf", 40);
    ttf1->setColor(ccBLACK);
    ttf1->setPosition(ccp(300, 600));
    ttf1->setTag(100);
    this->addChild(ttf1,1000);
    
    DownloadObject* dlObj2=DownloadObject::create();
    dlObj2->setUrl("http://www.kidsedu.com/blackcat/blackcatDB0828.sql");
    dlObj2->setExpectFilename("blackcatDB0828.sql");
    dlObj2->setFileTotalSize(60000000);
    dlObj2->setTarget(this);
    dlObj2->setSelector(callfuncO_selector(DownloadLayer::download));
    DownloadManager::sharedDownloadManager()->download(dlObj2);
    CCLabelTTF* ttf2=CCLabelTTF::create("blackcatDB0828.zip进度:0%", "KaiTi.ttf", 40);
    ttf2->setPosition(ccp(300, 400));
    ttf2->setColor(ccBLACK);
    ttf2->setTag(101);
    this->addChild(ttf2,1000);
    
    return true;
}

void DownloadLayer::onExit(){
    CCLayerColor::onExit();
    CCLog("DownloadLayer::onExit");
}

void DownloadLayer::download(cocos2d::CCObject *obj){
    DownloadObject* dlObj=(DownloadObject*)obj;
    if (dlObj->getRealFilename()=="backup.zip") {
        CCLabelTTF* label1=(CCLabelTTF*)this->getChildByTag(100);
        CCString* str=CCString::createWithFormat("backup.zip进度:%d%%",dlObj->getPercent());
        label1->setString(str->getCString());
    }else{
        CCLabelTTF* label2=(CCLabelTTF*)this->getChildByTag(101);
        CCString* str=CCString::createWithFormat("blackcatDB0828.zip进度:%d%%",dlObj->getPercent());
        label2->setString(str->getCString());
    }
}