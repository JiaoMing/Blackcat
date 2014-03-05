//
//  LoadingScene.h
//  Blackcat
//
//  Created by haojunhua on 13-10-21.
//
//

#ifndef __PointPen__LoadingScene__
#define __PointPen__LoadingScene__

#include "cocos2d.h"

USING_NS_CC;

typedef enum{
    kLoadingRolePanda=0,
    kLoadingRoleBlackcat
}LoadingRole;

class LoadingScene : public CCLayer
{
public:
    static CCScene* scene(const char* sceneName,bool isReleaseCurrentScene=false,LoadingRole role=kLoadingRoleBlackcat);
    void loadFinishedCallback(CCObject *pSender);
public:
    LoadingScene(const char* sceneName,bool isReleaseCurrentScene=false,LoadingRole loadingRole=kLoadingRoleBlackcat);
    ~LoadingScene();
    virtual bool init();
    void repalceScene();
    void processReplaceScene();
    virtual void onEnter();
private:
    const char* m_sceneName;//需要转向的目标scene
    bool m_isReleaseCurrentScene;
    LoadingRole m_loadingRole;
    int m_resTotal;//需要加载的资源总数
    int m_resCurrent;//当前已加载的资源数量
};

#endif /* defined(__Blackcat__LoadingScene__) */
