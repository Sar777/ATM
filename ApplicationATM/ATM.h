#pragma once
#include "Common.h"
#include "CreditCard.h"
#include <memory>
#include <vector>

#define MAX_PIN_COUNTER 3

enum Actions : uint8
{
	ACTION_BALANCE			= 0,
	ACTION_WITHDRAW_MONEY   = 1,
	ACTION_EJECT_CARD		= 2,
	MAX_ACTION
};

enum State : uint8
{
	ATM_STATE_NONE					= 0,
	ATM_STATE_CARD_INSERTED			= 1,
	ATM_STATE_CARD_INJECTED			= 3
};

enum ValidateResult
{
	ATM_CARD_VALIDATE_NONE			= 0,
	ATM_CARD_VALIDATE_SUCCESS		= 1,
	ATM_CARD_VALIDATE_FAIL_PIN		= 2,
	ATM_CARD_VALIDATE_CARD_BLOCKED	= 3
};

class ATM;

typedef std::vector<std::pair<uint32, uint8>> BanknoteVector;

class ATM
{
public:
	ATM();
	~ATM();

	void Initialization();
	static void PrintActions();

	BanknoteVector const& GetBanknotes() const { return _banknotes; }

	ValidateResult ValidateCard(uint16 pin);
	void InsertCard(std::unique_ptr<CreditCard> _card);

	// Actions
	bool EjectCard();
	bool WithdrawMoney(uint32 money);
	bool Balance();

	State GetState() const { return _state; }
	uint8 GetPinFailCounter() const { return _pinFailCounter;  }
private:
	std::vector<uint32> GetBanknoteList(uint32 money);
	void RemoveFromCassettes(std::vector<uint32> const& backnotes);

	std::unique_ptr<CreditCard> _currentCard;
	BanknoteVector _banknotes;

	State _state;
	uint8 _pinFailCounter;
};

