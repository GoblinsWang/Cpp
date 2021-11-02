#ifndef RECORD_H
#define RECORD_H

#define RECORD_LENGTH 100 //每条记录属性个数
#define RECODE_SIZE (sizeof(int64_t)*(RECORD_LENGTH+1)) //每条记录的字节数

class Record{
public:
    //默认构造函数
    Record(){

    }
    //析构函数
    ~Record(){

    }
    //设置主键
    void setKey(int64_t key){
        this->key = key;
    } 
    //设置列值
    void setColValue(int col, int64_t colvalue){
        this->value_array[col] =colvalue;
    }    
    //获取主键
    int64_t getkey(){
        return key;
    }
    //获取某列的值
    int64_t getColValue(int col){
        return value_array[col];
    }


private://将成员变量设置为私有，防止外界直接修改，提高安全性
    int64_t key; //作为记录主键，可以充当序号
    int64_t value_array[RECORD_LENGTH];//属性值数组, 按要求规定为占8个字节

};

#endif