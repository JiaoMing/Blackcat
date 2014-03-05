//
//  CardManager.h
//  ebooks
//
//  Created by 杨飞 on 13-10-9.
//
//

#ifndef __ebooks__CardManager__
#define __ebooks__CardManager__

#include "BaseModel.h"
#include "ECardSet.h"
#include "ECardPage.h"
#include "ECard.h"

#include "cocos2d.h"
using namespace cocos2d;

class CardManager : public CCObject
{
public:
    static CardManager *sharedCardManager();
    vector<ECardSet*>* getECardSets();
    vector<ECardPage*>* getECardPages();
    vector<ECard*>* getECards();
    void setCurrentECardSet(int index);
    void setECardPages(int sid);
    void setCurrentECardPage(int pageNumber);
    void setECards(int pid);
public:
    ECardSet* getCurrentECardSet(){return m_ecardset;}
    ECardPage* getCurrentECardPage(){return m_ecardpage;}
    int getCurrentECardPageNumber(){return m_pageNumer;}
    int getCurrentSetPageCount(){return m_ecardpages->size();}
public:
    CardManager();
    ~CardManager();
private:
    ECardSet* m_ecardset;
    ECardPage* m_ecardpage;
    vector<ECardSet*>* m_ecardsets;
    vector<ECardPage*>* m_ecardpages;
    vector<ECard*>* m_ecards;
    int m_pageNumer;
    bool m_isInit;
};

#define S_CM CardManager::sharedCardManager()

#define CURRENT_ECARDSET S_CM->getCurrentECardSet()
#define CURRENT_ECARDPAGE S_CM->getCurrentECardPage()
#define CURRENT_ECARDPAGENUMBER S_CM->getCurrentECardPageNumber()
#define CURRENT_PAGECOUNT S_CM->getCurrentSetPageCount()

#endif /* defined(__ebooks__CardManager__) */
