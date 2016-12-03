#include "stdafx.h"
#include "Bank.h"


Bank::Bank()
{
	_cards.emplace(1, 10000);
}

Bank::~Bank()
{
}

uint32 Bank::GetBalance(uint32 id) const
{
	auto itr = _cards.find(id);
	if (itr == _cards.end())
		throw std::exception("Bank: fail card not found");

	return itr->second;
}

uint32 Bank::WithdrawMoney(uint32 id, uint32 money)
{
	auto itr = _cards.find(id);
	if (itr == _cards.end())
		throw std::exception("Bank: fail card not found");

	if (itr->second < money)
		throw std::exception("Bank: fail card balance");

	itr->second -= money;
	return itr->second;
}
