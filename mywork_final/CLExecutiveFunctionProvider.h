#ifndef CLEXECUTIVEFUNCTIONPROVIDER_H
#define CLEXECUTIVEFUNCTIONPROVIDER_H

#include "CLStatus.h"
#include "CLMutex.h"

/*
CLExecutiveFunctionProvider���װ��ִ��������ҵ����
*/
class CLExecutiveFunctionProvider
{
public:
	CLExecutiveFunctionProvider();
	virtual ~CLExecutiveFunctionProvider();

public:
	virtual CLStatus RunExecutiveFunction() = 0;

private:
	CLExecutiveFunctionProvider(const CLExecutiveFunctionProvider&);
	CLExecutiveFunctionProvider& operator=(const CLExecutiveFunctionProvider&);
public:
	static CLMutex *mutex_operate; //用于表格操作时的互斥量
};

#endif
