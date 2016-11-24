#include "CreditCard.h"
#include <sstream>

CreditCard::CreditCard(): _pinCode(0)
{
	_numberCode = uint32(rand() % 100000000 + 1);
	_balance = uint32(rand() % 10000 + 1); // TEMP
}

std::string CreditCard::ToString() const
{
	std::ostringstream stream;
	stream << "NumberCode: " << _numberCode << " Pin: " << _pinCode;
	return stream.str();
}

bool CreditCard::ValidatePin(uint16 pin) const
{
	return pin == _pinCode;
}
