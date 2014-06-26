//
//  GuideBaseLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-10.
//
//

#ifndef Blackcat_GuideBaseLayer_h
#define Blackcat_GuideBaseLayer_h

#include "GuideManager.h"
#include "GuideDialogLayer.h"
#include "Cartoon.h"

/**
 *GuideBaseLayer并非Guide中其他layer类的基类，而是Dialog或Cover宿主类(如HomeScene)的基类
 */
class GuideBaseLayer: public CCLayer,public GuideDialogDelegate{
public:
    GuideBaseLayer();
    virtual ~GuideBaseLayer();
    
    virtual bool init();
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority;};
    
public:
    void startGuide(const char *queueKey,const char* stepKey,bool isDefaultFirstStepKey=false);
    void step();
    
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
    CC_SYNTHESIZE(string, m_queueKey, QueueKey);
    CC_SYNTHESIZE(string, m_stepKey, StepKey);
    
private:
    Guide* m_guide;
    Cartoon* m_cartoon;
    int m_lastReorderTag;
    CCPoint m_cartoonPosition;
    int m_lastReorderOrder;
};


#endif
