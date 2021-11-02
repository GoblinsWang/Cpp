#ifndef CLEXECUTIVEFUNCTION_H
#define CLEXECUTIVEFUNCTION_H

#include<iostream>
#include<string>
#include<algorithm>
#include "Record.h"
#include "IndexCreator.h"
#include "Form.h"
#include "CLStatus.h"
#include "CLExecutiveFunctionProvider.h"

using namespace std;

class CLGenerateForm:public CLExecutiveFunctionProvider
{
public:
    CLGenerateForm();

    virtual ~CLGenerateForm();

    virtual CLStatus RunExecutiveFunction();
};

class CLRowAdd:public CLExecutiveFunctionProvider
{
public:
    CLRowAdd();

    virtual ~CLRowAdd();

    virtual CLStatus RunExecutiveFunction();
};

class CLRowSearch:public CLExecutiveFunctionProvider
{
public:
    CLRowSearch(int left, int right, int col);

    virtual ~CLRowSearch();

    virtual CLStatus RunExecutiveFunction();

private:
    //查询所用的信息
    int m_left;
    int m_right;
    int m_col;
};

#endif

