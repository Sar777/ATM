#include "ATM.h"
#include "CreditCard.h"

#include <iostream>
#include <memory>

#define MAX_ERROR_CARD 3

pAction Actions[MAX_ACTION] =
{
	&ATM::Balance,
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

bool ATM::Execute(uint8 action)
{
	if (action >= MAX_ACTION)
		throw std::exception("ATM: Fail execute operation. Action not found");

	return (this->*Actions[action])();
}

// Actions
bool ATM::Balance()
{
	if (!_currentCard)
		throw std::exception("ATM: Not found card");

	if ((_currentCard->GetState() & ATM_CARD_VALIDATED) == 0)
		throw std::exception("ATM: Fail, card not validated");

	std::cout << "You balance: " << _currentCard->GetBalance() << std::endl;
	return true;
}

bool ATM::InjectCard()
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
