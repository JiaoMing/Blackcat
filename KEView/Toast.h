//
//  Toast.h
//  Blackcat
//
//  Created by haojunhua on 14-4-13.
//
//

#ifndef __Blackcat__Toast__
#define __Blackcat__Toast__

#include "resource.h"

/**
 *信息显示面板，仿Anroid Toast
 */
class Toast: public CCLayer{
public:
    static Toast* sharedToast();
    virtual bool init();
    CREATE_FUNC(Toast);
    
    void makeText(const char* text);
private:
    CCLabelTTF* m_label;
    CCLayerColor* m_bg;
};

#endif /* defined(__Blackcat__Toast__) */
