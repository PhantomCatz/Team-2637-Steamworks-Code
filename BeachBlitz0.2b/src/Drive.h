/*
 * Drive.h
 *
 *  Created on: Aug 19, 2017
 *      Author: Programming
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

#include "CANTalon.h"
#include "WPILib.h"

class Drive
{
private:
	RobotDrive* driver;
public:
	Drive();
	CANTalon *Rleft;
	CANTalon *Fleft;
	CANTalon *Fright;
	CANTalon *Rright;

	void AutoArcade(float x, float y);
	void AutoDrive(float x, float y);
	void setDriveControl(XboxController *xbox);
	void setDriveControl2(XboxController *xbox);
};



#endif /* SRC_DRIVE_H_ */
