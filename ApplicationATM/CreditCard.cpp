#include "CreditCard.h"
#include <sstream>

CreditCard::CreditCard(): _pinCode(0), _cardState(0)
{
	_id = 1;
}

std::string CreditCard::ToString() const
{
	std::ostringstream stream;
	stream << "NumberCode: " << _id << " Pin: " << _pinCode;
	return stream.str();
}

bool CreditCard::ValidatePin(uint16 pin) const
{
	return pin == _pinCode;
}
