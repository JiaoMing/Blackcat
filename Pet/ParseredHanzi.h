//
//  ParseredHanzi.h
//  Blackcat
//
//  Created by haojunhua on 14-4-1.
//
//

#ifndef __Blackcat__ParseredHanzi__
#define __Blackcat__ParseredHanzi__

#include "Hanzi.h"
#include "cocos2d.h"
USING_NS_CC;

typedef enum{
    kHanziStepError,
    kHanziStepMiddlePoint,
    kHanziStepHanziStart,
    kHanziStepStrokeStart,
    kHanziStepStrokeEnd,
    kHanziStepHanziEnd
}HanziStepStatus;

class ParseredHanzi{
public:
    ParseredHanzi(Hanzi* hanzi);
    HanziStepStatus step();
    void reset();
    CCPoint getTempPoint(){return m_tempPoint;}
    int getLocusIndex(){return m_locusIndex;};
    int getLocusTempIndex(){return m_locusTempIndex;};
    int getStrokeIndex(){return m_strokeIndex;};
    
    void setHeartBeatFactor(float factor);
private:
    Hanzi* m_hanzi;
    CCArray* m_pLocusPath;//路径数据
    CCPoint m_tempPoint;
    int m_strokeIndex;
    int m_locusIndex;
    int m_locusTempIndex;
    
    float m_heartBeatDistance;
};

#endif /* defined(__Blackcat__ParseredHanzi__) */
