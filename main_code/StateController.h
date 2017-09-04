#ifndef StateController_h
#define StateControlle_h

#include "Arduino.h"
#include "Constant.h"
#include "Stream.h"
#include "MotionController.h"
#include "SensorController.h"

class StateController
{
public:
	//Constructor
	StateController(MotionController* MC, SensorController* SC);

	//Execut command return from serial
	void executeCommand();

private:
	MotionController* MC;
	SensorController* SC;

	void doTurnCalibrate();
	void frontCalibrate();
	void turnCalibrate();
	void fowardExplore();
	int calibrateMid();
	void calibrateMid2();

	int stepCount;

	int fastForwardMoveTick[9] = {365, 914, 1475, 2045, 2640, 3265, 3840, 4500, 5090};

	//FastestPathFlag
	bool FPF;

	//Sensor Reading
	int FM, FR, FL, LT, LB, R;

};
#endif
