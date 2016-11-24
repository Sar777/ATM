#pragma once
#include "Common.h"
#include <string>

enum CardState
{
	ATM_CARD_NONE			= 0x0,
	ATM_CARD_INSERTED		= 0x1,
	ATM_CARD_VALIDATED		= 0x2,
};

class CreditCard
{
public:
	CreditCard();
	CreditCard(uint32 numberCode, uint16 pinCode) : _numberCode(numberCode), _pinCode(pinCode), _balance(0), _cardState(ATM_CARD_NONE)
	{ }

	std::string ToString() const;
	uint32 GetBalance() const { return _balance; }

	uint32 GetNumberCode() const { return _numberCode; }
	void SetNumberCode(uint32 number) { _numberCode = number;  }

	uint32 GetPinCode() const { return _pinCode; }
	void SetPinCode(uint16 number) { _pinCode = number; }

	bool ValidatePin(uint16 pin) const;

	uint32 GetState() const { return _cardState;  }
	void AddState(uint32 state) { _cardState |= state;  }
private:
	uint32 _numberCode;
	uint16 _pinCode;
	uint32 _balance;
	uint32 _cardState;
};

