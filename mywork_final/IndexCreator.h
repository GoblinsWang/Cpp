#ifndef INDEXCREATOR_H
#define INDEXCREATOR_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "CLStatus.h"
#include "Record.h"
using namespace std;

#define INDEX_FILE "Index_" //定义索引文件前缀
#define M 3 //设置B+树的阶数为2M
/**
 * 本索引结构通过B+树来实现
**/
//索引节点结构
typedef struct KeyNode {
    int64_t primary_key;//主键
    int64_t value;//属性值
}KeyNode;

//B+树结点类型
typedef struct BPTNode
{
    int num;//记录子节点树
    bool is_leaf;//保存节点是否为叶子节点的信息
    KeyNode datanodes[2 * M - 1];//含记录主键的节点
    struct BPTNode* childs[2 * M]; //子节点数组
    BPTNode* left;//左兄弟
    BPTNode* right;//右兄弟
}BPTNode;

class IndexCreator{
public:
    IndexCreator();
    ~IndexCreator();
    //创建一个节点对象
    BPTNode* new_BPTNode();
    //创建B+树
    BPTNode* create_BPTree();
    //为col列属性创建数据节点
    KeyNode CreateDataNode(Record record,int col);
    //插入未满的节点中
    void BPTree_Insert_NotFull(BPTNode* node, const KeyNode& DataNode);
    //节点分裂
    int BPTree_Split_Child(BPTNode* parent, int pos, BPTNode* child);
    //插入节点s
    BPTNode* BPTree_Insert(BPTNode* root, const Record& record, int col);
    //根据给定条件搜索记录
    vector<int64_t> SearchValueRange(BPTNode* root, int64_t left, int64_t right, int col); //搜索在col列属性值在范围为[left, right]的记录
    //保存索引文件
    CLStatus SaveIndexFile(BPTNode *root, int col);
    //保存B+树节点
    CLStatus WriteBPTNode(int &fd, BPTNode *node);
    //读取索引文件
    BPTNode* ReadIndexFile(int col);
    //读取B+树节点
    BPTNode* ReadBPTNode(int &fd, BPTNode* &leaf_node_pre);
    //判断col列索引文件是否已经存在 
    bool Is_IndexFile_Existed(char pathname[]);

};

#endif