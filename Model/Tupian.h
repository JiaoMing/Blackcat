//
//  Tupian.h
//  blackcat
//
//  Created by haojunhua on 13-9-4.
//
//

#ifndef blackcat_Tupian_h
#define blackcat_Tupian_h
#include "Kapian.h"
#include <iostream>
using namespace std;

#define TUPIAN_VIEW_THRESHOLD 10

class Tupian: public Kapian{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Tupian);
    
    SYNTHESIZE_STRING(ci);
    SYNTHESIZE_STRING(pinyin);
    SYNTHESIZE_STRING(pinyinyindiao);
    SYNTHESIZE_STRING(english);
    SYNTHESIZE_STRING(iconPath);
    SYNTHESIZE_STRING(cnAudioPath);
    SYNTHESIZE_INT(viewCount);
    
    virtual void registTablenameAndProperty(){
        tablename="ciyu";
        INSERT_PROPERTYMAP(Tupian,id);
        INSERT_PROPERTYMAP(Tupian,ci);
        INSERT_PROPERTYMAP(Tupian,pinyin);
        INSERT_PROPERTYMAP(Tupian,pinyinyindiao);
        INSERT_PROPERTYMAP(Tupian,english);
        INSERT_PROPERTYMAP(Tupian,iconPath);
        INSERT_PROPERTYMAP(Tupian,cnAudioPath);
        INSERT_PROPERTYMAP(Tupian,viewCount);
        INSERT_PROPERTYMAP(Tupian,isCollected);
    }
    
public:
    float getProgress() const{return (float)viewCount/TUPIAN_VIEW_THRESHOLD;}//viewCount改为0
    void setProgress(float progress){viewCount=(int)(progress*TUPIAN_VIEW_THRESHOLD);}
    string getText() const{return ci;}
    string getPath() const{
        string iconFilePrename=iconPath;
        iconFilePrename=iconFilePrename.substr(0,iconPath.find_last_of("."));
        return iconFilePrename+".thumb.jpg";
    }
};

#endif
