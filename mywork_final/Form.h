#ifndef FORM_H
#define FORM_H

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include "CLStatus.h"
#include "Record.h"
#include <fcntl.h>
#include <unistd.h>
#include "CLMutex.h"
#include "IndexCreator.h"
using namespace std;

class Form{
public://公有函数
	static Form* GetInstance();//用于类外创建Form对象，且不会被多次创建
	CLStatus AddRow(const Record &Record);//添加新的一行数据
	vector<int64_t> SearchRow(int left, int right, int col);//指定col行，给定区间，搜索符合条件的记录
	CLStatus GenerateForm();//用于生成含有随机记录的存储表
	BPTNode* GenerateIndexFile(int col);//为col列生成索引文件
    CLStatus UpdateIndexFile(int col, Record Record);//每次有新记录添加时，更新索引文件
	Record GenerateRecord();//用于创建随机记录
	void printForm();//打印表格前10条记录
	static void printRecord(Record record);//打印记录
	bool Is_IndexFile_Existed(int col);//判断col列是否已有索引文件


private://私有函数
	Form();
	~Form();
	CLStatus WriteRecord(Record Record);//将Record写入存储表中

private://私有变量
	int m_Fd;//文件描述符
	int num_row;//表中已有的数据行数
	static Form *m_form;
    //static SPara *spara;
    static CLMutex *mutex_form; //用于表格创建时的互斥量
	
public:
	vector<Record> records;//vector可实现记录无上限存储，动态优化内存，不必要一开始创建100万数据内存
	IndexCreator *indexcreator;//索引对象的指针
};
#endif