//
//  InstallScene.h
//  Blackcat
//
//  Created by haojunhua on 14-2-27.
//
//

#ifndef Blackcat_InstallScene_h
#define Blackcat_InstallScene_h
#include "cocos2d.h"
USING_NS_CC;

class InstallScene : public CCLayerColor
{
public:
    static CCScene* scene();
public:
    InstallScene();
    ~InstallScene();
    virtual bool init();
    virtual void onEnter();
    CREATE_FUNC(InstallScene);
    
    void gotoHomeScene();
    void installCallback();
private:
    CCLabelTTF* m_labelProcess;
    int m_installCount;
};

#endif
