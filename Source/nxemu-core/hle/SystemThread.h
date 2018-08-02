#pragma once
#include <map>

class CHleKernel;

class CSystemThread
{
public:
    CSystemThread(CHleKernel * m_Kernel);
    ~CSystemThread();

private:
    CSystemThread();                               // Disable default constructor
    CSystemThread(const CSystemThread&);            // Disable copy constructor
    CSystemThread& operator=(const CSystemThread&); // Disable assignment
};

typedef std::map<uint32_t, CSystemThread*> SystemThreadList;