#include "StateController.h"

StateController::StateController(MotionController* MC, SensorController* SC)
{
	this->MC = MC;
	this->SC = SC;
	this->FPF = false;
}

void StateController::executeCommand()
{
	int charCount = 0;
	int moveTick = 0;
	unsigned char secondComm;

 
	Serial.setTimeout(50);
	while (true)
	{
		charCount = 0;
		if (Serial.available())
		{
			// get the new byte:
			String command = Serial.readString();

			do
			{
				switch (command[charCount])
				{
        case 'a':
          MC->printEncoder();
          break;
        
				case 'e':
					StateController::fowardExplore();
					break;

				case 'f':
					secondComm = command[++charCount] - 49;
					MC->fastmove(FORWARD, fastForwardMoveTick[secondComm] , fastSpeed);
					if (!FPF)
						StateController::doTurnCalibrate();
					break;

				case 'b':
					secondComm = command[++charCount] - 48;
					moveTick = (secondComm > 1) ? Constant::fastBackwardMoveTick*secondComm - 244 : Constant::forwardMoveTick;
					MC->move(BACKWARD, moveTick, fastSpeed);
					if (!FPF)
						StateController::doTurnCalibrate();
					break;

				case 'r':
					MC->turn(RIGHT, Constant::rightTurnTick, turnSpeed);
					stepCount = 4;
					if (!FPF)
						StateController::doTurnCalibrate();
					break;

				case 'l':
					MC->turn(LEFT, Constant::leftTurnTick, turnSpeed);
					stepCount = 4;
					if (!FPF)
						StateController::doTurnCalibrate();
					break;

				case 'c':
					StateController::calibrateMid2();
					break;

				case 'g':
					this->FPF = !this->FPF;

				case 's':
					SC->getAllGrid(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
					break;

				case 'z':
					Serial.println("pk");
					break;
				}

				delay(30);

			} while (command[++charCount] != '\0');

			Serial.flush();
		}
	}
}

void StateController::fowardExplore()
{

	MC->move(FORWARD, Constant::forwardMoveTick, fastSpeed);

	while (true)
	{
		//Get all IR and print
		SC->getAllGrid(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);


		if (this->LT != 0 && this->LB != 0)
		{
			break;
		}

		//Calibration
		if (this->FL == 0 || this->FM == 0 || this->FR == 0)
		{
			StateController::turnCalibrate();
			break;
		}

		//Move
		MC->move(FORWARD, Constant::forwardMoveTick, fastSpeed);
		if (++stepCount > 6)
		{
			int turn = StateController::calibrateMid();
			if (turn == 1)
			{
				MC->turn(LEFT, Constant::leftTurnTick, turnSpeed);
			}
			else if (turn == 0)
			{
				MC->turn(RIGHT, Constant::rightTurnTick, turnSpeed);
			}
			delay(10);
			SC->getAllGridWithoutPrint(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
			StateController::turnCalibrate();

			stepCount = 0;

		}
	}
}

void StateController::doTurnCalibrate()
{
	delay(10);
	SC->getAllGrid(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
	StateController::turnCalibrate();
}

void StateController::turnCalibrate()
{
	if (this->FL == 0 && this->FR == 0)
	{
		StateController::frontCalibrate();
	}
	else if (this->LT == 0 && this->LB == 0)
	{
		if (SC->needSideCal())
		{
			MC->turn(LEFT, Constant::rightTurnTick, turnSpeed);
			delay(10);
			StateController::frontCalibrate();
			delay(10);
			MC->turn(RIGHT, Constant::rightTurnTick, turnSpeed);
		}

		MC->calibrateTurn(LEFT);
	}
}

void StateController::frontCalibrate()
{
	MC->calibrateTurn(FORWARD);
	delay(10);
	MC->calibrateFront();
	delay(10);
}

int StateController::calibrateMid()
{
	int isRight;

	//Get all IR and print
	SC->getAllGridWithoutPrint(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
	if (this->FL == 0 && this->FR == 0)
	{
		StateController::frontCalibrate();
	}
    
	if (this->LT == 0 && this->LB == 0)
	{
		MC->turn(LEFT, Constant::rightTurnTick, turnSpeed);
		isRight = 0;
	}
	else if (this->R == 0)
	{
		MC->turn(RIGHT, Constant::rightTurnTick, turnSpeed);
		isRight = 1;
	}
	else
		return 2;


	delay(30);
	SC->getAllGridWithoutPrint(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
	if (this->FL == 0 && this->FR == 0)
		StateController::frontCalibrate();


	return isRight;
}

void StateController::calibrateMid2()
{

	//Get all IR and print
	SC->getAllGridWithoutPrint(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
	if (this->FL == 0 && this->FR == 0)
	{
		StateController::frontCalibrate();
	}

	if (this->R == 0)
	{
		MC->turn(RIGHT, Constant::rightTurnTick, turnSpeed);
	}
	else if (this->LT == 0 && this->LB == 0)
	{
		MC->turn(LEFT, Constant::rightTurnTick, turnSpeed);
	}


	delay(30);
	SC->getAllGridWithoutPrint(&this->FM, &this->FR, &this->FL, &this->LT, &this->LB, &this->R);
	if (this->FL == 0 && this->FR == 0)
		StateController::frontCalibrate();

}



