//
//  Hanzi.h
//  blackcat
//
//  Created by haojunhua on 13-9-2.
//
//

#ifndef blackcat_Hanzi_h
#define blackcat_Hanzi_h
#include "Kapian.h"
#include "Utils.h"
#include <vector>
#include <string>
using namespace std;

#define HANZI_WRITE_THRESHOLD 3

class Hanzi: public Kapian{
    EMPTY_CONSTRUCT_DESTRUCT(Hanzi);
    
    SYNTHESIZE_STRING(zi);
    SYNTHESIZE_STRING(pinyin);
    SYNTHESIZE_STRING(pinyinyindiao);
    SYNTHESIZE_STRING(ZPYYD);
    SYNTHESIZE_INT(bihuashu);
    SYNTHESIZE_STRING(bushou);
    SYNTHESIZE_STRING(contour);
    SYNTHESIZE_STRING(locus);
    SYNTHESIZE_STRING(cnAudioPath);
    SYNTHESIZE_INT(writeCount);
    SYNTHESIZE_INT(kcid);
    SYNTHESIZE_INT(lastAnswer);
    SYNTHESIZE_INT(isReward);
    SYNTHESIZE_INT(sort);
    
public:
    /**
     *  获取主拼音音调
     *
     *  @return 主拼音音调
     */
    string getZpyyd(){
        if (ZPYYD.length()>0){
            return ZPYYD;
        }
        if (pinyinyindiao.find(",")>0) {
            vector<string> dest;
            StringUtils::split(pinyinyindiao, ",", dest);
            return dest[0];
        }else{
            return pinyinyindiao;
        }
    }
    
    virtual void registTablenameAndProperty(){
        tablename="hanzi";
        INSERT_PROPERTYMAP(Hanzi,id);
        INSERT_PROPERTYMAP(Hanzi,zi);
        INSERT_PROPERTYMAP(Hanzi,pinyin);
        INSERT_PROPERTYMAP(Hanzi,pinyinyindiao);
        INSERT_PROPERTYMAP(Hanzi,ZPYYD);
        INSERT_PROPERTYMAP(Hanzi,bihuashu);
        INSERT_PROPERTYMAP(Hanzi,bushou);
        INSERT_PROPERTYMAP(Hanzi,contour);
        INSERT_PROPERTYMAP(Hanzi,locus);
        INSERT_PROPERTYMAP(Hanzi,cnAudioPath);
        INSERT_PROPERTYMAP(Hanzi,writeCount);
        INSERT_PROPERTYMAP(Hanzi,kcid);
        INSERT_PROPERTYMAP(Hanzi,lastAnswer);
        INSERT_PROPERTYMAP(Hanzi,isReward);
        INSERT_PROPERTYMAP(Hanzi,isCollected);
        INSERT_PROPERTYMAP(Hanzi,sort);
    }
    
public:
    float getProgress() const{return (float)writeCount/HANZI_WRITE_THRESHOLD;}
    void setProgress(float progress){writeCount=(int)(progress*HANZI_WRITE_THRESHOLD);}
    string getText() const{return zi;}
    string getPath() const{return zi;}
};

#endif
