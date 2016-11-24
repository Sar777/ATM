#pragma once
#include "Common.h"
#include "CreditCard.h"
#include <memory>

#define MAX_PIN_COUNTER 3

enum Actions : uint8
{
	ACTION_BALANCE			= 0,
	ACTION_INJECT_CARD		= 1,
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

typedef bool(ATM::*pAction)();

class ATM
{
public:
	ATM();
	~ATM();

	void Initialization();
	static void PrintActions();
	ValidateResult ValidateCard(uint16 pin);
	void InsertCard(std::unique_ptr<CreditCard> _card);
	bool Execute(uint8 action);
	bool InjectCard();
	bool Balance();
	State GetState() const { return _state; }
	uint8 GetPinFailCounter() const { return _pinFailCounter;  }
private:
	std::unique_ptr<CreditCard> _currentCard;
	State _state;
	uint8 _pinFailCounter;
};

