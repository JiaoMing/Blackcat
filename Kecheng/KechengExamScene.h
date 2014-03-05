//
//  ExamScene.h
//  Blackcat
//
//  Created by haojunhua on 13-10-8.
//
//

#ifndef Blackcat_ExamScene_h
#define Blackcat_ExamScene_h

#include "KechengBaseLayer.h"
#include "Hanzi.h"
using namespace std;
#include "GuideDialogLayer.h"


typedef enum{
    kAnswerResultWrong=0,
    kAnswerResultRight
}AnswerResult;


class KechengExamSceneDelegate;

class KechengExamScene : public KechengBaseLayer,public GuideDialogDelegate{
    
public:
    static CCScene* scene(KechengExamSceneDelegate* kechengExamSceneDelegate,vector<Hanzi*>* hanziVector);
public:
    KechengExamScene(KechengExamSceneDelegate* kechengExamSceneDelegate,vector<Hanzi*>* hanziVector);
    ~KechengExamScene();
    virtual bool init();
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
private:
    void outOfOrder();//打乱顺序
    void yanshuOut(float delay);//鼹鼠下层动画（首先会禁止其他动画，用于结束和进入下一题时过度）
    void exam();//执行考试逻辑和动画
    void reExam();//执行考试前消失动作并重新测试
    void startAndCountdown();//开始测试并启动倒计时
    void readHanzi(float t);//读汉字
    void timeUp();//倒计时结束处理
    void menuCallback(CCObject* object);
    void answerResult(AnswerResult ar);
    void examEnd();//测试结束处理
    void reward(float t);//执行奖励
    virtual void showDialog();
    /**
     * 对话框回调
     */
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;}
    virtual void dialogCallBack(GuideDialogCMD cmd);
private:
    int m_kcid;//课程ID
    vector<Hanzi*>* m_hanziVector;//课程汉字
    int m_examCount;//答题数量
    int m_rightCount;//答对次数
    KechengExamSceneDelegate* m_kechengExamSceneDelegate;
    Hanzi* m_hanzi;//选中汉字
    bool m_isClickable;//汉字是否可点击，用于禁止重复点击
    int m_daojishi;//倒计时计数
    GuideDialogLayer* m_gudieDialogLayer;//对话框
    bool m_isPlayDing;//是否播放ding
};

class KechengExamSceneDelegate{
public:
    virtual void examAllRightCallback()=0;
};

#endif
