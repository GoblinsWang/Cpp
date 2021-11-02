#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "Form.h"
using namespace std;

#define FORM_FILE_NAME "FORM.txt"


Form* Form::m_form = 0;
CLMutex* Form::mutex_form = new CLMutex;

Form::Form()
{
    cout << "正在初始化Form！"<<endl;
    indexcreator = new IndexCreator;
    num_row = 0;
    srand((int64_t)time(NULL));//设置随机种子，使得生成的每条随机记录不一样

	m_Fd = open(FORM_FILE_NAME, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); 

    //设置容器的预留空间，用于测试
    records.reserve(10000);
    //读取存储表
    //利用lseek获取文件大小
    if (lseek(m_Fd, 0, SEEK_END)){
        num_row = lseek(m_Fd, 0, SEEK_END) / RECODE_SIZE;
        lseek(m_Fd, 0, SEEK_SET);
        records.resize(num_row);
        read(m_Fd, &records[0], num_row * RECODE_SIZE);//将表中所有数据读入Vector容器中
    }
    else
        cout << "表格为空,请添加记录或初始化表格！" << endl;
    //printForm();
    if (records.empty()){
        cout << "error when read form!"<<endl;
    }
    cout << "当前行数为：" << records.size() << endl;
    cout << "Form初始化成功！"<<endl;
}


Form::~Form() {
    if (m_Fd) 
        close(m_Fd);
    delete mutex_form;
}

Form* Form::GetInstance()
{
    if (m_form != 0)
        return m_form;
    mutex_form->Lock();
	if(m_form == 0)
	{
		m_form = new Form;
	}
    mutex_form->Unlock();

	return m_form;
}

//添加新的一行数据
CLStatus Form::AddRow(const Record &Record)
{
	Form *form = Form::GetInstance();
	if (form == 0)
		return CLStatus(-1, 0);
    if (m_Fd == -1)
        return CLStatus(-1, 0);
	CLStatus s = WriteRecord(Record);
	if(s.IsSuccess()){
        cout << "添加记录 "<<num_row<<" 成功：" ;
        printRecord(Record);
        cout << "正在更新索引文件"<<endl ;
        for (int col = 0; col<RECORD_LENGTH; col++){
            if (Is_IndexFile_Existed(col)){
                UpdateIndexFile(col, Record);
            }
        }
        num_row++;
		return CLStatus(0, 0);
    }
	else
		return CLStatus(-1, 0);
}

//指定col行，给定区间，搜索符合条件的记录
vector<int64_t> Form::SearchRow(int left, int right, int col){
    vector<int64_t> KeyVec;//用于存储符合条件的记录key值
    BPTNode *root;//用于存储col属性的B+树索引
    //cout <<"num of root:" <<root->num<<endl;
    //首先判断是否有索引文件存在
    if (Is_IndexFile_Existed(col)){
        cout << "存在第"<< col <<"列属性的索引文件" << endl;
        //存在则读取索引文件
        root = indexcreator->ReadIndexFile(col);
        cout << "索引文件读取成功！"<<endl;
    }
    else{
        cout << "不存在第"<< col <<"列属性的索引文件，正在创建索引！" << endl;
        //为col创建索引文件
        root = GenerateIndexFile(col);
    }
    KeyVec = indexcreator->SearchValueRange(root, left, right, col);
    return KeyVec;
}

//将记录保存至文件中
CLStatus Form::WriteRecord(Record Record)
{   
    if (write(m_Fd, &Record, RECODE_SIZE) == -1)
        return CLStatus(-1,0);
	return CLStatus(0, 0);
}

//生成随机的Form
CLStatus Form::GenerateForm(){
    Record record;
    for (int i = 0; i < 100; i++) {
        record = GenerateRecord();
        CLStatus s = AddRow(record);
	    if(!s.IsSuccess())
		    return CLStatus(-1, 0);
    }
    return CLStatus(0,0);

}

//生成随机记录
Record Form::GenerateRecord() {
    Record record;
    record.setKey(num_row);
    for (int i = 0; i < RECORD_LENGTH; i++)
        record.setColValue(i, rand() % 1000); //生成随机数作为属性，范围为[0,999]
    records.push_back(record);
    return record;
}

//打印存储表
void Form::printForm(){
    cout << "存储表中前10条数据如下:" << endl;
    //打印前10条记录
    for_each(records.begin(), records.begin()+10, printRecord);

    // cout << "存储表中最后10条数据如下:" << endl;
    // //打印最后10条记录
    // for_each(records.end()-10, records.end(), printRecord);

}

//打印记录
void Form::printRecord(Record record){
	cout << "第"<< record.getkey() << "记录 ";
    for (int i = 0; i < 20; i++) //只打印记录前20个value
        cout <<record.getColValue(i)<<" ";
    cout <<endl;
}

//为col列生成索引文件
BPTNode* Form::GenerateIndexFile(int col){
    BPTNode* root = indexcreator->create_BPTree();
    //printForm();
    cout << "需要创建的索引结构包含"<<num_row<<"条记录"<<endl;
    for (int i=0;i<records.size(); i++){
        //printRecord(records[i]);
        root = indexcreator->BPTree_Insert(root, records[i], col);
    }
    cout << "第" << col << "列属性的B+树索引生成成功！" << endl;
    //cout <<"in Form::GenerateIndexFile() "<<"num in rootnode:" <<root->num<<endl;
    //将索引结构保存为文件
    CLStatus s = indexcreator->SaveIndexFile(root, col);
    if (s.IsSuccess())
        cout << "第" << col << "列属性的索引保存成功！" << endl;
    return root;

}

//为col列生成更新文件
CLStatus Form::UpdateIndexFile(int col, Record Record){
    BPTNode* root;
    root = indexcreator->ReadIndexFile(col);//先读取
    root = indexcreator->BPTree_Insert(root, Record, col);//再插入新添加的记录
    //printForm();
    CLStatus s = indexcreator->SaveIndexFile(root, col);//保存更新完的索引
    if (s.IsSuccess()){
        cout << "第" << col << "列属性的索引更新成功！" << endl;
        return CLStatus(0, 0);
    }
    else
        return CLStatus(-1, 0);
}

//判断col列是否已有索引文件
bool Form::Is_IndexFile_Existed(int col){
    char index_file_path[20];
    sprintf(index_file_path, "%s%d", INDEX_FILE, col);
    return indexcreator->Is_IndexFile_Existed(index_file_path);
}
