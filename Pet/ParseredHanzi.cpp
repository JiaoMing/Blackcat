//
//  ParseredHanzi.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-1.
//
//

#include "ParseredHanzi.h"
#include "Utils.h"

//数据库保存数据，不可更改
#define HanziDBWidth 200
#define HanziDBHeight 200

#define HanziScale 2

static CCPoint coordConvert(CCPoint point){
    return ccp((point.x-30)*HanziScale,(HanziDBHeight-point.y)*HanziScale);
}

static CCPointArray* string2Array(const string& src){
    CCPointArray* subPath;
    if (src.compare("")==0) {
        return CCPointArray::create(0);
    }
    vector<std::string> d,ss;
    StringUtils::split(src, "@", d);
    subPath=CCPointArray::create((unsigned int)d.size());
    vector<std::string>::iterator p, q;
    int i=0;
    for(p=d.begin();p!=d.end();++i,++p)
    {
        ss.clear();
        StringUtils::split(*p,",",ss);
        int j=0;
        float x,y;
        for (q=ss.begin();q!=ss.end();++q,++j){
            string zuobiao=*q;
            if(j==1)x=atof(zuobiao.c_str());
            else if(j==2) y=atof(zuobiao.c_str());
        }
        subPath->addControlPoint(coordConvert(ccp(x,y)));
    }
    return subPath;
}


ParseredHanzi::ParseredHanzi(Hanzi* hanzi){
    
    this->setHeartBeatFactor(1);
    
    m_pLocusPath=CCArray::create();
    m_pLocusPath->retain();
    
    this->m_hanzi=hanzi;
    vector<std::string> vec;
    StringUtils::split(m_hanzi->getlocus(), ";", vec);
    vector<std::string>::iterator it;
    for(it=vec.begin();it!=vec.end();++it)
    {
        CCPointArray* subPath=string2Array(*it);
        m_pLocusPath->addObject(subPath);
    }
    this->reset();
}

void ParseredHanzi::setHeartBeatFactor(float factor){
    m_heartBeatDistance=1.5*CCEGLView::sharedOpenGLView()->getScaleX()*factor;
}

void ParseredHanzi::reset(){
    m_strokeIndex=0;
    m_locusIndex=0;
    m_tempPoint=ccp(0,0);
}

HanziStepStatus ParseredHanzi::step(){
    CCPointArray* pLocusPathShowing=(CCPointArray*)(m_pLocusPath->objectAtIndex(m_strokeIndex));
    if (pLocusPathShowing->getControlPoints()->size()==0) {
        CCLog("笔画编辑出有误，没有轨迹信息");
        return kHanziStepError;
    }
    if (m_tempPoint.equals(ccp(0,0))) {
        m_tempPoint=*(CCPoint*)pLocusPathShowing->getControlPoints()->at(0);
        return kHanziStepHanziStart;
    }else{
        CCPoint endPoint=*(CCPoint*)pLocusPathShowing->getControlPoints()->at(m_locusIndex+1);
        float distance=ccpDistance(m_tempPoint, endPoint);
        if (distance<=m_heartBeatDistance) {
            m_tempPoint=endPoint;
            if(m_locusIndex<pLocusPathShowing->count()-2){
                //下一个轨迹点
                m_locusIndex++;
                m_locusTempIndex=0;
                return kHanziStepMiddlePoint;
            }else{
                if (m_strokeIndex<m_pLocusPath->count()-1) {
                    m_strokeIndex++;//下一个笔划
                    m_locusIndex=0;//从头开始
                    m_locusTempIndex=0;
                    pLocusPathShowing=(CCPointArray*)(m_pLocusPath->objectAtIndex(m_strokeIndex));
                    if(pLocusPathShowing->getControlPoints()->size()>0){
                        //新笔划从头开始写
                        m_tempPoint=*(CCPoint*)pLocusPathShowing->getControlPoints()->at(m_locusIndex);
                        return kHanziStepStrokeStart;
                    }
                    else{
                        CCLog("笔画编辑出有误，没有轨迹信息");
                        return kHanziStepError;
                    }
                }else{
                    this->reset();
                    return kHanziStepHanziEnd;
                }
            }
        }else{
            m_locusTempIndex++;
            m_tempPoint=ccpLerp(m_tempPoint,endPoint,m_heartBeatDistance/distance);
            return kHanziStepMiddlePoint;
        }
    }
}