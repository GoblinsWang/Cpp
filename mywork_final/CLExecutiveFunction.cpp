#include "CLExecutiveFunction.h"

//-------------CLGenerateForm类成员函数实现---------
CLGenerateForm::CLGenerateForm()
{
    cout << "创建CLGenerateForm对象成功！"<<endl;
}
CLGenerateForm::~CLGenerateForm(){

}

CLStatus CLGenerateForm::RunExecutiveFunction()
{
    Form *pForm = Form::GetInstance();
    if(pForm == 0)
		return CLStatus(-1, 0);
    mutex_operate->Lock();
    cout << "Generate Random Records to Form!" << endl;
    CLStatus s = pForm->GenerateForm();
    mutex_operate->Unlock();
    if(s.IsSuccess()){
        cout << "Generate Task is Finished" << endl;
		return CLStatus(0, 0);
    }
	else
		return CLStatus(-1, 0);
}

//-------------CLRowAdd类成员函数实现---------
CLRowAdd::CLRowAdd()
{
    cout << "创建CLRowAdd对象成功！"<<endl;
}
CLRowAdd::~CLRowAdd(){

}
CLStatus CLRowAdd::RunExecutiveFunction()
{   
    Form *pForm = Form::GetInstance();
    if(pForm == 0)
		return CLStatus(-1, 0);
    mutex_operate->Lock();
    cout << "Add Random Record to Form!" << endl;
    Record record;
    record = pForm->GenerateRecord();
    CLStatus s = pForm->AddRow(record);
    mutex_operate->Unlock();
    if(s.IsSuccess()){
        cout << "Add Row Task is Finished" << endl;
		return CLStatus(0, 0);
    }
	else
		return CLStatus(-1, 0);
}

//-------------CLRowSearch类成员函数实现---------

CLRowSearch::CLRowSearch(int left, int right, int col)
{
    cout << "创建CLRowSearch对象成功！"<<endl;
    m_col = col;
    m_left = left;
    m_right = right;
}

CLRowSearch::~CLRowSearch(){

}

CLStatus CLRowSearch::RunExecutiveFunction()
{
    Form *pForm = Form::GetInstance();
    if(pForm == 0)
		return CLStatus(-1, 0);
    mutex_operate->Lock();
    cout << "Search Some Records From Form!" << endl;
    vector<int64_t> row_nums;
    row_nums = pForm->SearchRow(m_left, m_right, m_col);
    mutex_operate->Unlock();
    cout << "Add Row Task is Finished" << endl;
    if (row_nums.empty())
        cout << "未搜索到符合条件的记录！" <<endl;
    else{
        cout << "符合条件的记录序号为：";
        for (vector<int64_t>::iterator it = row_nums.begin();it != row_nums.end(); it++)
            cout << *it << " ";
        cout << endl;
        cout << "其中前10条记录如下：" <<endl;
        for (int i=0; i<row_nums.size() && i<10; i++)
            pForm->printRecord(pForm->records[row_nums[i]]);
    }
    return CLStatus(0,0);

}