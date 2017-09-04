## Arduino
1. mains is the core of the code where all initialisation and looping occur (main_code.ino)
2. Constant.h stores constants that is used in rumtime and initialisation.
3. PID ensures that the wheel movement are equal (PID.h & PID.cpp)
4. Sensor Controlller controls and manages all sensor functions like initialising, reading & etc. (SensorController.h & SensorController.cpp)
5. Sensor Motion controls and manages all movement like initialising, going straight & turning (SensorController.h & SensorController.cpp)
6. State Controller is the coommunication decoder, auto calibration and etc. (StateController.h, StateController.cpp)


## Library Dependencies:  
* DualVNH5019MotorShield.h  
* MemoryFree.h	
* PinChangeInt.h  

All available in /arduino_library.zip

## Communication
### Machine Code
Command to be executed is using one char machine code and another char for parameter (optional) since it saves memory on Arduino.
  
List of function codes currently available:

| Instruction | Effect                      |
| :---------- | :-------------------------- |
| e | Foward Explore - Keep going foward & send sensor data until meeting a wall |
| fx | Go Forward for x grid |
| bx | Go Backward for x grid |
|||
| r | Trun right |
| l	| Trun left |
|||
| c	| Calibrate Middle to ensure robot is in the middle of 9 grids|
|||
| g | Alter the fastest path flag which will disable all sensor when true (Default disable) |
|||
| s | Print all sensor reading to Serial (For debugging purpose) |
| z | Arduino Initialise paring command |


## Calibration
Front Calibrate : Calibrate the distance between robot and front wall. This calibration will use either the Front Mid, Front Right, Front Left Sensor for calibration.

Turn Calibrate : Calibrate the orientation of the robot. This calibration will use either the Front Left & Front Right Sensors or Left Top & Left Bottom Sensor for calibration.

Robot will always try to do a mid calibration whenever it's is possible. 
Mid Calibration consist of :
1. Front Calibrate
2. Turn Right or Turn Left
3. Front Calibrate
4. Turn back to the original orientation
5. Turn Calibrate

Calibration is auto determine by Arduino rather than Algorithm team.
Calibration Criteria:
* When foward explore is done
* For every 5 steps. Directional Movement: step +1 , Turning: step +2
* When the distance of the robot is at the unsafe range


****

***Disclaimer:*** This repo is no longer maintained and was submitted as part of the coursework assignment for MDP

***Note:*** This project is just for reference purposes. Please do not copy blindly. There are other factors that will affect the outcome of your robot.
