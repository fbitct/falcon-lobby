#include "stdafx.h"
#include "Mutex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Mutex::Mutex(const char * name)
: m_hMutex(0), m_name(name)
{
	m_hMutex = CreateMutex(0,TRUE,m_name.c_str());

	return;
}

Mutex::~Mutex()
{
	if (0 != m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		m_hMutex = 0;
	}

	return;
}
