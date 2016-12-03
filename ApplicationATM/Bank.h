#pragma once
#include "Common.h"
#include <map> 
#include <unordered_map>

class Bank
{
public:
	Bank();
	~Bank();

	static Bank& Instance()
	{
		static Bank b;
		return b;
	}

	uint32 GetBalance(uint32 id) const;
	uint32 WithdrawMoney(uint32 id, uint32 money);

	Bank(Bank const&) = delete;
	Bank& operator= (Bank const&) = delete;

private:
	std::unordered_map<uint32, uint32> _cards;
};

#define sBankMgr Bank::Instance()