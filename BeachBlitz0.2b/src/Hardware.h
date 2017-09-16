/*
 * Hardware.h
 *
 *  Created on: Aug 19, 2017
 *      Author: Programming
 */

#ifndef SRC_HARDWARE_H_
#define SRC_HARDWARE_H_

#include "WPILib.h"
#include "CANTalon.h"
#include "AHRS.h"


class Hardware
{
public:
	Hardware();
	CANTalon*rightFlywheel;
	CANTalon*leftFlywheel;
	Spark*conveyor;
	Spark*kicker;
	Spark*climber;
	Spark*intake;
	Servo*hopper;
	Spark*hopperBouncerLeft;
	Spark*hopperBouncerRight;

	Solenoid*flaps;
	Compressor*comp;

	AHRS*navx;
	Encoder*shooterEncoder;
	Encoder*wheelEncoder;
	DigitalInput*kickerLimit;
	DigitalInput*gearButton;

	Timer*kickTimer;

 };



#endif /* SRC_HARDWARE_H_ */
