//
//  GiftAndFood.h
//  PointPen
//
//  Created by wanghongjun on 13-10-17.
//
//

#ifndef PointPen_GiftAndFood_h
#define PointPen_GiftAndFood_h

#include "BaseModel.h"

class GiftAndFood: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(GiftAndFood);
    
    SYNTHESIZE_INT(id);
    SYNTHESIZE_STRING(name);
    SYNTHESIZE_INT(nums);
    SYNTHESIZE_INT(gift_value);
    SYNTHESIZE_INT(food_value);
    SYNTHESIZE_INT(Lv_value);
    SYNTHESIZE_INT(degree);
    SYNTHESIZE_INT(type);
    SYNTHESIZE_STRING(actions);
    SYNTHESIZE_STRING(say);
    SYNTHESIZE_INT(foodtype);
    
    
    virtual void registTablenameAndProperty(){
        tablename="gift_food";
        INSERT_PROPERTYMAP(GiftAndFood,id);
        INSERT_PROPERTYMAP(GiftAndFood,name);
        INSERT_PROPERTYMAP(GiftAndFood,nums);
        INSERT_PROPERTYMAP(GiftAndFood,gift_value);
        INSERT_PROPERTYMAP(GiftAndFood,food_value);
        INSERT_PROPERTYMAP(GiftAndFood,Lv_value);
        INSERT_PROPERTYMAP(GiftAndFood,degree);
        INSERT_PROPERTYMAP(GiftAndFood,type);
        INSERT_PROPERTYMAP(GiftAndFood,actions);
        INSERT_PROPERTYMAP(GiftAndFood,say);
        INSERT_PROPERTYMAP(GiftAndFood,foodtype);
    }
    
public:
   
};


#endif
