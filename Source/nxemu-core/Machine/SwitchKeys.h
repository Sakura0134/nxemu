#pragma once
#include <Common\stdtypes.h>
#include <Common\IniFileClass.h>
#include <vector>
#include <map>

class CSwitchKeys
{
public:
	CSwitchKeys();
	~CSwitchKeys();

private:
	CSwitchKeys(const CSwitchKeys&);				// Disable copy constructor
	CSwitchKeys& operator=(const CSwitchKeys&);		// Disable assignment
};