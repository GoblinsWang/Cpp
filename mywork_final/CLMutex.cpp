#include "CLMutex.h"
#include<iostream>
using namespace std;

CLMutex::CLMutex()
{
	int  r = pthread_mutex_init(&m_Mutex, 0);
	if(r != 0)
	{
		throw "In CLMutex::CLMutex(), pthread_mutex_init error";
	}
}

CLMutex::~CLMutex()
{
	int r = pthread_mutex_destroy(&m_Mutex);
	if(r != 0)
	{
		cout << "In CLMutex::~CLMutex(), pthread_mutex_destroy error";
	}
}

CLStatus CLMutex::Lock()
{
	int r = pthread_mutex_lock(&m_Mutex);
	if(r != 0)
		return CLStatus(-1, 0);
	else
		return CLStatus(0, 0);
}

CLStatus CLMutex::Unlock()
{
	int r = pthread_mutex_unlock(&m_Mutex);
	if(r != 0)
		return CLStatus(-1, 0);
	else
		return CLStatus(0, 0);
}