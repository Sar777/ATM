#include "ATM.h"
#include "CreditCard.h"

#include <iostream>
#include <memory>
#include "Bank.h"

#define MAX_ERROR_CARD 3

pAction Actions[MAX_ACTION] =
{
	&ATM::Balance,
	&ATM::WithdrawMoney,
	&ATM::InjectCard,
};

ATM::ATM(): _state(ATM_STATE_NONE), _pinFailCounter(0)
{ }

ATM::~ATM()
{
}

void ATM::PrintActions()
{
	for (uint8 i = 0; i < MAX_ACTION; ++i)
	{
		switch (i)
		{
			case ACTION_BALANCE:
			{
				std::cout << ACTION_BALANCE << ") Balance: " << std::endl;
				break;
			}
			case ACTION_WITHDRAW_MONEY:
			{
				std::cout << ACTION_WITHDRAW_MONEY << ") Withdraw money: " << std::endl;
				break;
			}
			case ACTION_INJECT_CARD:
			{
				std::cout << ACTION_INJECT_CARD << ") InjectCard: " << std::endl;
				break;
			}
			default:
				throw std::exception("ATM::PrintActions: unknown action");
		}
	}
}

void ATM::Initialization()
{
	_banknotes.emplace_back(5, 30);
	_banknotes.emplace_back(10, 30);
	_banknotes.emplace_back(20, 30);
	_banknotes.emplace_back(100, 2);

	std::cout << "ATM initializated" << std::endl;
}

ValidateResult ATM::ValidateCard(uint16 pin)
{
	if (!_currentCard)
		throw std::exception("ATM: Fail card validate(Not card)");

	if (!_currentCard->ValidatePin(pin))
	{
		++_pinFailCounter;
		return _pinFailCounter >= MAX_PIN_COUNTER ? ATM_CARD_VALIDATE_CARD_BLOCKED : ATM_CARD_VALIDATE_FAIL_PIN;
	}

	_currentCard->AddState(ATM_CARD_VALIDATED);
	return ATM_CARD_VALIDATE_SUCCESS;
}

void ATM::InsertCard(std::unique_ptr<CreditCard> _card)
{
	_currentCard = std::move(_card);
	_currentCard->AddState(ATM_CARD_INSERTED);

	_state = ATM_STATE_CARD_INSERTED;

	std::cout << "ATM::InsertCard: Card inserted ok" << std::endl;
	std::cout << "You card info: " << _currentCard->ToString() << std::endl;
}

bool ATM::Execute(uint8 action, uint32 data)
{
	if (action >= MAX_ACTION)
		throw std::exception("ATM: Fail execute operation. Action not found");

	return (this->*Actions[action])(data);
}

// Actions
bool ATM::Balance(uint32 data /*= 0*/)
{
	if (!_currentCard)
		throw std::exception("ATM: Not found card");

	if ((_currentCard->GetState() & ATM_CARD_VALIDATED) == 0)
		throw std::exception("ATM: Fail, card not validated");

	std::cout << "You balance: " << sBankMgr.GetBalance(_currentCard->GetId()) << std::endl;
	return true;
}

bool ATM::WithdrawMoney(uint32 money)
{
	if (!_currentCard)
		throw std::exception("ATM: Not found card");

	if ((_currentCard->GetState() & ATM_CARD_VALIDATED) == 0)
		throw std::exception("ATM: Fail, card not validated");

	auto banknotes = GetBanknoteList(money);
	if (banknotes.empty())
		throw std::exception("ATM: fail ATM empty or impossible generate banknotes...");

	if (sBankMgr.GetBalance(_currentCard->GetId()) < money)
		throw std::exception("ATM: fail bank money");

	// Снимаем со счета
	sBankMgr.WithdrawMoney(_currentCard->GetId(), money);

	// Достаем из кассет
	RemoveFromCassettes(banknotes);

	std::cout << "For " << money << std::endl;
	for (auto const& banknote : banknotes)
		std::cout << banknote << std::endl;

	std::cout << std::endl;
	return true;
}

bool ATM::InjectCard(uint32 data /*= 0*/)
{
	if (!_currentCard)
		throw std::exception("ATM: Fail card inject(Not card)");

	if ((_currentCard->GetState() & ATM_CARD_INSERTED) == 0)
		throw std::exception("ATM: Fail card not inserted");

	_state = ATM_STATE_CARD_INJECTED;

	_currentCard.reset();
	_pinFailCounter = 0;

	std::cout << "ATM::InjectCard: Card injected ok\n\n" << std::endl;
	return true;
}

std::vector<uint32> ATM::GetBanknoteList(uint32 money)
{
	const uint32 INF = 0xFFFFFFFF;
	std::vector<uint32> F;
	F.resize(money + 1);

	F[0] = 0;
	for (uint32 m = 1; m <= money; ++m)
	{ 
		F[m] = INF;
		for (std::size_t i = 0; i < _banknotes.size(); ++i)
		{
			if (m >= _banknotes[i].first && F[m - _banknotes[i].first] + 1 < F[m])
				F[m] = F[m - _banknotes[i].first] + 1;
		}
	}

	if (F[money] == INF)
		return std::vector<uint32>();

	// Копирование списка банкнот
	auto copyBankNotes = _banknotes;

	std::vector<uint32> banknotes;
	while (money > 0)
	{
		for (std::size_t i = 0; i < copyBankNotes.size(); ++i)
		{
			if (F[money - copyBankNotes[i].first] != F[money] - 1)
				continue;

			if (copyBankNotes[i].second == 0)
				return std::vector<uint32>();

			banknotes.emplace_back(copyBankNotes[i].first);
			money -= copyBankNotes[i].first;

			copyBankNotes[i].second -= 1;
			break;
		}
	}

	return banknotes;
}

void ATM::RemoveFromCassettes(std::vector<uint32> const& backnotes)
{
	for (auto& banknote : _banknotes)
	{
		if (banknote.second == 0)
			continue;

		banknote.second -= std::count(backnotes.begin(), backnotes.end(), banknote.first);
	}
}
