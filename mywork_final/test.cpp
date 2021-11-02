#include <iostream>
#include <unistd.h>
#include "CLThread.h"
#include "CLExecutiveFunction.h" //随机表格生成头文件
#include "Form.h"   //存储表实现
using namespace std;

int main()
{
    Form *form = Form::GetInstance();
    //form->GenerateIndexFile(0);
    //form->GenerateForm();
    form->printForm();
    //生成随机表格
    // CLExecutiveFunctionProvider *generateform = new CLGenerateForm();
    // CLExecutive *pThread = new CLThread(generateform);

    //添加功能
    CLExecutiveFunctionProvider *addrow = new CLRowAdd();
    CLExecutive *pThread1 = new CLThread(addrow);

    //搜索功能
    int col;
    int64_t leftv, rightv;
    cout << "请输入你要查询的属性列，以及区间，格式为：col left right"<<endl;
    cin >> col >> leftv >> rightv;
    CLExecutiveFunctionProvider *searchrow = new CLRowSearch(leftv, rightv, col);//输入left， right, col
    CLExecutive *pThread2 = new CLThread(searchrow);

    pThread1->Run();
    pThread2->Run();

    pThread1->WaitForDeath();
    pThread2->WaitForDeath();

    delete pThread1;
    delete pThread2;
    delete searchrow;
    delete addrow;
    // delete generateform;

    return 0;
}
