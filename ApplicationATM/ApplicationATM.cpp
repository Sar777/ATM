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
				atm->EjectCard();
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

	std::cout << "ATM Banknotes..." << std::endl;
	for (auto banknote : atm->GetBanknotes())
		std::cout << banknote.first << " rub, " << int(banknote.second) << "x" << std::endl;

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

			switch (action)
			{
				case ACTION_BALANCE:
					atm->Balance();
					break;
				case ACTION_WITHDRAW_MONEY:
					int money;
					std::cout << "Money: ";
					std::cin >> money;
					atm->WithdrawMoney(money);
					break;
				case ACTION_EJECT_CARD:
					atm->EjectCard();
					break;
				default:
					break;
			}

			if (atm->GetState() == ATM_STATE_CARD_INJECTED)
				break;
		}
	}

	return 0;
}

