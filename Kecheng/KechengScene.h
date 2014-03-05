//
//  YiKeScene.h
//  Blackcat
//
//  Created by haojunhua on 13-10-7.
//
//

#ifndef Blackcat_YiKeScene_h
#define Blackcat_YiKeScene_h

#include "Hanzi.h"
#include "KechengBaseLayer.h"
#include "KechengExamScene.h"
using namespace std;

class KechengSceneDelegate;

class KechengScene : public KechengBaseLayer,public KechengExamSceneDelegate{
public:
    static CCScene* scene(int kcid,KechengSceneDelegate* kechengSceneDelegate);
public:
    KechengScene(int kcid,KechengSceneDelegate* kechengSceneDelegate);
    ~KechengScene();
    virtual bool init();
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual void examAllRightCallback();
    
    virtual void runRewardAnimate();
    virtual void runRewardAnimateCallBack(CCObject* obj);
private:
    void menuCallback(CCObject* object);
public:
    int m_kcid;//课程ID
    vector<Hanzi*>* m_hanziVector;
    KechengSceneDelegate* m_kechengSceneDelegate;
    vector<int> m_hids;
};


class KechengSceneDelegate{
public:
    virtual void examAllRightCallback(int kcid)=0;
};


#endif
