#ifndef CLEXECUTIVE_H
#define CLEXECUTIVE_H

#include "CLStatus.h"

class CLExecutiveFunctionProvider;

/* 
CLExecutive类用于创建一个执行体
*/
class CLExecutive
{
public:
	explicit CLExecutive(CLExecutiveFunctionProvider *pExecutiveFunctionProvider);
	virtual ~CLExecutive();

	virtual CLStatus Run() = 0;

	virtual CLStatus WaitForDeath() = 0;
	
protected:
	CLExecutiveFunctionProvider *m_pExecutiveFunctionProvider;

private:
	CLExecutive(const CLExecutive&);
	CLExecutive& operator=(const CLExecutive&);
};

#endif
