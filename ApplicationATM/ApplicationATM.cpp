// ApplicationATM.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "ATM.h"
#include <memory>
#include <ctime>
#include <iostream>

bool CardInsertValidate(std::shared_ptr<ATM> atm)
{
	auto _card = std::make_unique<CreditCard>();
	atm->InsertCard(std::move(_card));

	uint16 pin = 0;
	do
	{
		std::cout << "You pin: ";
		std::cin >> pin;

		ValidateResult result = atm->ValidateCard(pin);
		switch (result)
		{
			case ATM_CARD_VALIDATE_FAIL_PIN:
			{
				std::cout << "Fail pin" << std::endl;
				continue;
			}
			case ATM_CARD_VALIDATE_CARD_BLOCKED:
			{
				std::cout << "Fail: Pin put limit. Card Injected" << std::endl;
				atm->InjectCard();
				return false;
			}
			case ATM_CARD_VALIDATE_SUCCESS:
			case ATM_CARD_VALIDATE_NONE:
			default: return true;
		}

	} while (true);
}

int main()
{
	srand(static_cast<unsigned>(time(nullptr)));
	auto atm = std::make_shared<ATM>();
	atm->Initialization();

	while (true)
	{
		if (!CardInsertValidate(atm))
			continue;

		while (true)
		{
			atm->PrintActions();
			
			int action = 0;
			std::cout << "Action: ";
			std::cin >> action;

			if (!atm->Execute(action))
				break;

			if (atm->GetState() == ATM_STATE_CARD_INJECTED)
				break;
		}
	}

	return 0;
}

