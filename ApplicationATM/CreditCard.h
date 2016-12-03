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
	CreditCard(uint32 id, uint16 pinCode) : _id(id), _pinCode(pinCode), _cardState(ATM_CARD_NONE)
	{ }

	std::string ToString() const;

	uint32 GetId() const { return _id; }

	uint32 GetPinCode() const { return _pinCode; }

	bool ValidatePin(uint16 pin) const;

	uint32 GetState() const { return _cardState;  }
	void AddState(uint32 state) { _cardState |= state;  }
private:
	uint32 _id;
	uint16 _pinCode;
	uint32 _cardState;
};

