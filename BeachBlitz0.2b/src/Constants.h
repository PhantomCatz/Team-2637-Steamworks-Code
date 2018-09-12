/*
 * Constants.h
 *
 *  Created on: Aug 19, 2017
 *      Author: Programming
 *       converted: Matt
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_


	static final int T_RESET = 5;
	static final double AUTO_SPEED = .47;
	enum{BLUE_ALLIANCE,RED_ALLIANCE};


	static final bool SOLENOID_CLOSED = false;
	static final bool SOLENOID_OPEN = true;


	static final double BANGBANG_THRESHOLD = 2;
	double MIN_POWER = .2;
	double MAX_POWER = .8;
	bool testMode = true;


	static final double DRIVE_T_OUT = 5.0;
	static final double TURN_T_OUT = 5.0;
	static final double TWO_SEC_TOUT = 2.0;

	//put similar variables and code together

	enum {A_BUTTON=1,B_BUTTON,X_BUTTON,Y_BUTTON,LEFT_BUMPER,RIGHT_BUMPER,XBOX_TAB,XBOX_START,LEFT_JOYBUTTON,RIGHT_JOYBUTTON};
	enum {LEFT_X,LEFT_Y,LEFT_TRIGGER,RIGHT_TRIGGER,RIGHT_X,RIGHT_Y};

	enum {JOYSTICK_TRIGGER=1,SIDE_BUTTON,THUMB_BOTlEFT,THUMB_BOTRIGHT,THUMB_TOPLEFT,THUMB_TOPRIGHT};
	enum {X_AXIS,Y_AXIS,TWIST_AXIS,SLIDER};

	static final double straightkP = .18;
	static final double straightkD = .23;  //ORIGINALLY .18

	double TurnkP = 0.118, TurnkD = 0.174;

	#define VAR_1_BUFFER_SIZE 20
	#define DEBUG_1_BUFFER_SIZE 500
	#define USE_GEAR_BUTTON true
	#define DONT_USE_GEAR_BUTTON false




#endif /* SRC_CONSTANTS_H_ */
