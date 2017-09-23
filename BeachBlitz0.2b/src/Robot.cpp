#include "Drive.h"
#include "WPILib.h"
#include "hardware.h"
#include "math.h"
#include <timer.h>
#include "iostream"
#include "Constants.h"
#include  <stdio.h>

/**
 * CHANGES TO MAKE
 * put gearSequenceWaitTime back to .4
 * REPLACE FLAP POSITIONS
 * put smartdashboards back into robotperiodic from encoderstraightdrive
 */


class Robot:
public IterativeRobot
{
private:
	Drive* driver = new Drive();               //defines variables
	Hardware* hardware = new Hardware();
	Timer* teleopTimer = new Timer();
	Timer* functionTimer = new Timer();
	Timer* gearResetTimer = new Timer();
	Timer* PDTimer = new Timer();
	XboxController* xboxDrive = new XboxController(0);
	Joystick * joystickAux = new Joystick(1);
	Joystick* joystickTest = new Joystick(2);

	double circumference;

	bool gearMechReady = true;
	bool gearMechOpen = false;

	double shooterEncoderRate;

	double fineAdjust = 0.0;
	bool flyWheelOn = false;
	double shooter = .68;
	double shooterSlider = shooter;
	double flyWheelSpeed;

	double flywheelSpeed1,flywheelSpeed2,flywheelSpeed3,flywheelSpeed4,flywheelSpeed5;
	int flyWheelCount=1;
	double flyWheelSpeedArray[5];
	double encoderArray[VAR_1_BUFFER_SIZE];

	char charArray1[2000];
	char charArray2[2000];

public:
	Robot()
	{

	}


	void toggleFlywheel()
	{
		if (flyWheelOn==false)
			flyWheelOn = true;
		else
			flyWheelOn = false;
		Wait(0.3);
	}
	void BANGBANG(double target)
	{
		double actualTarget = target*100.0+5.0;

		shooterEncoderRate = hardware->shooterEncoder->GetRate();   //slidingWindowDamp(flyWheelCount);

		if (shooterEncoderRate <= actualTarget-BANGBANG_THRESHOLD)
		{
			hardware->rightFlywheel->Set(MAX_POWER);
			hardware->leftFlywheel->Set(-MAX_POWER);
		}

		else if (shooterEncoderRate >= actualTarget+BANGBANG_THRESHOLD)
		{
			hardware->rightFlywheel->Set(MIN_POWER);
			hardware->leftFlywheel->Set(-MIN_POWER);
		}
		flyWheelCount++;
	}


	void slidingWindowDamp(int count)
	{
		switch(count)
		{
		case(1):
			flywheelSpeed1=hardware->shooterEncoder->GetRate();//overSamplingDamp();
			break;
		case(2):
			flywheelSpeed2=hardware->shooterEncoder->GetRate();//overSamplingDamp();
			break;
		case(3):
			flywheelSpeed3=hardware->shooterEncoder->GetRate();//overSamplingDamp();
			break;
		case(4):
			flywheelSpeed4=hardware->shooterEncoder->GetRate();//overSamplingDamp();
			break;
		case(5):
			flywheelSpeed5=hardware->shooterEncoder->GetRate();//overSamplingDamp();
			shooterEncoderRate = (flywheelSpeed1+flywheelSpeed2+flywheelSpeed3+flywheelSpeed4+flywheelSpeed5)/5;
			break;
		default:
			flywheelSpeed1=flywheelSpeed2;
			flywheelSpeed2=flywheelSpeed3;
			flywheelSpeed3=flywheelSpeed4;
			flywheelSpeed4=flywheelSpeed5;
			flywheelSpeed5=hardware->shooterEncoder->GetRate();//overSamplingDamp();
			shooterEncoderRate = (flywheelSpeed1+flywheelSpeed2+flywheelSpeed3+flywheelSpeed4+flywheelSpeed5)/5;
			break;
		}
	}
	double overSamplingDamp()
	{
		flyWheelSpeedArray[0]=hardware->shooterEncoder->GetRate();
		flyWheelSpeedArray[1]=hardware->shooterEncoder->GetRate();
		flyWheelSpeedArray[2]=hardware->shooterEncoder->GetRate();
		flyWheelSpeedArray[3]=hardware->shooterEncoder->GetRate();
		flyWheelSpeedArray[4]=hardware->shooterEncoder->GetRate();
		return (flyWheelSpeedArray[0]+flyWheelSpeedArray[1]+flyWheelSpeedArray[2]+flyWheelSpeedArray[3]+flyWheelSpeedArray[4]);
	}


	void toggleGearMech()
	{
		if(gearMechOpen)
		{
			hardware->flaps->Set(SOLENOID_CLOSED);
			gearMechOpen=false;
		}
		else
		{
			hardware->flaps->Set(SOLENOID_OPEN);
			gearMechOpen=true;
		}
		Wait(.5);  //0.3
	}

	void gearSequence(bool closeFlaps)
	{
		driver->setDriveControl(xboxDrive);
		if(gearMechOpen==false)
			toggleGearMech();
		Wait(.2);
		kick();
		if(closeFlaps == true){
			toggleGearMech();
		}
		else{}
	}

	void kick ()
	{

		hardware->kickTimer->Reset();
		hardware->kickTimer->Start();
		while (hardware->kickTimer->Get() < 0.1) //0.2
		{
			hardware->kicker->Set(.4);  //0.05
			driver->setDriveControl(xboxDrive);
		}
		hardware->kickTimer->Reset();
		hardware->kickTimer->Start();
		while (hardware->kickTimer->Get() < 0.2) //0.2
		{
			hardware->kicker->Set(-0.4); //0.05
			driver->setDriveControl(xboxDrive);
		}
		hardware->kicker->Set(0);
	}


	void RobotPeriodic()
	{
		//shooterEncoderRate=hardware->shooterEncoder->GetRate();
		shooter = .55+.3*(-.5*(joystickAux->GetRawAxis(SLIDER)-1));     //Control shooter speed with slider

		if(shooter>.7)
		{
			MAX_POWER = .8;
			MIN_POWER = .65;
		}
		else
		{
			MAX_POWER = .75;
			MIN_POWER = .5;
		}

		flyWheelSpeed = hardware->shooterEncoder->GetRate();
		SmartDashboard::PutNumber("Shooter Input Speed", shooter);
		SmartDashboard::PutNumber("Current Flywheel Speed",flyWheelSpeed);

		SmartDashboard::PutNumber("wheelEncoder distance",hardware->wheelEncoder->GetDistance());
		SmartDashboard::PutNumber("wheelEncoder get",hardware->wheelEncoder->Get());

		SmartDashboard::PutNumber("shooterEncoder get",hardware->shooterEncoder->Get());
		if(joystickTest->GetRawButton(8)==1)
			hardware->shooterEncoder->Reset();
		if(joystickTest->GetRawButton(9)==1)
			hardware->wheelEncoder->Reset();

		SmartDashboard::PutBoolean("Shooter on",flyWheelOn);
		SmartDashboard::PutBoolean("Gear mech Closed",!gearMechOpen);
		SmartDashboard::PutBoolean("GearButton",hardware->gearButton->Get());
		SmartDashboard::PutNumber("NavXAngle",hardware->navx->GetAngle());
		SmartDashboard::PutNumber("WheelEncoder getRate",hardware->wheelEncoder->GetRate());

	}


	void RobotInit()
	{
		//autonomous variables
		SmartDashboard::PutNumber("starting position",1);
		SmartDashboard::PutString("alliance color", "blue");
		SmartDashboard::PutNumber("Autonomous v1 v2 v3",1);
		SmartDashboard::PutNumber("shooter speed",shooter);
		SmartDashboard::PutBoolean("Check if alliance has ground gear getter", false);

		// pulses per rev, circumference of wheel,
		//or drive for 2 seconds get number of pulses and total distance   the dist/pulses     //// plug that # for distance per pulse
		// 0 is controller   1 is aux main 2 is aux test


	}

	void wiggle()
	{
		AutoDrive(-1,1);
		Wait(.12);
		AutoDrive(1,-1);
		Wait(.18);
		AutoDrive(0,0);
	}
	void autoShootSequ( double autoShooterSpeed)
	{
		functionTimer->Reset();
		functionTimer->Start();
		while (functionTimer->Get()<1.5)
		{
			BANGBANG(autoShooterSpeed);
		}
		hardware->conveyor->Set(.6);
		functionTimer->Reset();
		while (functionTimer->Get()<1.5)
		{
			BANGBANG(autoShooterSpeed);
		}
		hardware->intake->Set(-1);
		while (functionTimer->Get()<3.5)
		{
			BANGBANG(autoShooterSpeed);
		}
		hardware->intake->Set(0);
		hardware->leftFlywheel->Set(0);
		hardware->rightFlywheel->Set(0);
		hardware->conveyor->Set(0);

	}
	void AutoDrive(double l,double r)
	{
		driver->Fleft->Set(l);
		driver->Rleft->Set(l);
		driver->Fright->Set(-r);
		driver->Rright->Set(-r);
	}



	void PDTurnThreshold(double turnDegrees, int timeout)

	{
		//hardware->navx->Reset();
		Wait(.3);

		bool done = false;

		double TurnToDegrees = turnDegrees+hardware->navx->GetAngle();
		double turnThreshold = 1.0;
		double fineTuneThreshold = 7.0;
		//double kP = 0.039, kD = 0.0147;
		double currentError,previousError = 0;
		double deltaError,derivative,deltaT;
		double power;

		functionTimer->Reset();
		functionTimer->Start();
		PDTimer->Reset();

		while(fabs(hardware->navx->GetAngle()) < fabs(TurnToDegrees) + turnThreshold && done!=true)
		{
			// make data array for deltaT,currentError; loop of about 100 + counter for how many loops
			PDTimer->Stop();
			deltaT = PDTimer->Get();
			PDTimer->Reset();
			PDTimer->Start();




			currentError = TurnToDegrees-hardware->navx->GetAngle();
			deltaError = currentError-previousError;
			derivative = deltaError/deltaT;

			power = (TurnkP*currentError)+(TurnkD*derivative);

			if (currentError < fineTuneThreshold)
			power = 0.7;


			if(turnDegrees>0)
			driver->AutoDrive(power,-power);

			else // if turn degrees< 0
			driver->AutoDrive(-power,power);


			previousError = currentError;

			if (functionTimer->Get() > timeout)
				done = true;
		}

		driver->AutoDrive(0,0);
		functionTimer->Start();
		functionTimer->Reset();
		PDTimer->Stop();
		PDTimer->Reset();
	}



	void swingTurn(double turnDegrees, double leftSpeed, double rightSpeed,int timeout)
	{
		bool done = false;
		functionTimer->Reset();
		functionTimer->Start();
		hardware->navx->Reset();
		hardware->navx->Reset();
		hardware->navx->Reset();
		Wait(.1);
		while(fabs(hardware->navx->GetAngle())<fabs(turnDegrees)&&!done)
		{
			AutoDrive(leftSpeed,rightSpeed);
			if(functionTimer->Get()>timeout)
				done=true;

		}
		AutoDrive(0,0);
		functionTimer->Stop();
	}


	void EncoderStraightDrive(double speed, double distance, double sampleTime,double timeout,bool useGearButton)
	{
		int loopCount = 0;
		double encoderIssues=0;
		int dbgCount1=0;

		bool done=false;
		bool gearState=true;		//DATA CHECK VARIABLES

		double previousAngle = 0.0;
		double currentAngle;
		double deltaAngle;			//FUNCTION VARIABLES
		double derivative;
		double deltaTime=sampleTime/1000;

		double encoderCheckNumber;
		double lastEncoderValue=0;



		hardware->navx->Reset();
		hardware->wheelEncoder->Reset();
		Wait(.2);
		//hardware->wheelEncoder->Reset();

		functionTimer->Reset();
		functionTimer->Start();





		while(fabs(hardware->wheelEncoder->GetDistance()) < distance+.5 && gearState==true && done != true)
		{
			if (useGearButton == true)
				gearState = hardware->gearButton->Get();



			//SmartDashboard::PutNumber("wheelEncoder distance",hardware->wheelEncoder->GetDistance());
			//SmartDashboard::PutNumber("wheelEncoder get", encoderCheckNumber);

			currentAngle = hardware->navx->GetAngle();

			deltaAngle = currentAngle-previousAngle;

			derivative = deltaAngle/deltaTime;

			//driver->AutoArcade(speed, kP*(currentAngle + kD*derivative));
			driver->AutoArcade(speed, straightkP*currentAngle + straightkD*derivative);

			previousAngle = currentAngle;

			if (functionTimer->Get() > timeout)
				done = true;



			encoderCheckNumber = hardware->wheelEncoder->Get();
			if(lastEncoderValue==encoderCheckNumber)
				encoderIssues++;
			lastEncoderValue=encoderCheckNumber;

			loopCount++;


			encoderArray[dbgCount1] = hardware->wheelEncoder->Get();
			dbgCount1++;
			if (dbgCount1== VAR_1_BUFFER_SIZE)
				dbgCount1=0;

		}

		driver->AutoArcade(0,0);
		functionTimer->Stop();


		SmartDashboard::PutNumber("Function timer value",functionTimer->Get());
		SmartDashboard::PutBoolean("gearState",gearState);
		SmartDashboard::PutNumber("encoderCheck",encoderIssues);
		SmartDashboard::PutNumber("drive straight loop count",loopCount);


		//DISPLAYS FIRST 10 MEASUREMENTS OF ENCODER->GET()
		sprintf(charArray1, "%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,",
				encoderArray[0],
				encoderArray[1],
				encoderArray[2],
				encoderArray[3],
				encoderArray[4],
				encoderArray[5],
				encoderArray[6],
				encoderArray[7],
				encoderArray[8],
				encoderArray[9]
				);
		SmartDashboard::PutString("characterArray1",charArray1);

		sprintf(charArray2, "%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,%9.5f,",
				encoderArray[10],
				encoderArray[11],
				encoderArray[12],
				encoderArray[13],
				encoderArray[14],
				encoderArray[15],
				encoderArray[16],
				encoderArray[17],
				encoderArray[18],
				encoderArray[19]
				);
		SmartDashboard::PutString("characterArray2",charArray2);
	}
	void middleAuto(int allianceColor)
	{
		EncoderStraightDrive(.7,80,T_RESET,1.1,USE_GEAR_BUTTON);
		EncoderStraightDrive(.45,50,T_RESET,1,USE_GEAR_BUTTON);

		if(hardware->gearButton->Get()==true)
		{
			wiggle();
			EncoderStraightDrive(.45,9,T_RESET,0.8,USE_GEAR_BUTTON);
		}
		if(hardware->gearButton->Get()==false)
			EncoderStraightDrive(-.45,7,T_RESET,1,USE_GEAR_BUTTON);
		gearSequence(false);


		EncoderStraightDrive(-.7,33,T_RESET,.43,DONT_USE_GEAR_BUTTON); //  4/21/17 9:00 MW Original TimeOut Value  .5 Original Encoder Distance 40 Match 1 Value
		if(allianceColor == BLUE_ALLIANCE)
			PDTurnThreshold(-107,5);  //100
		else
			PDTurnThreshold(107,5); //100

		EncoderStraightDrive(.7,56,T_RESET,2,DONT_USE_GEAR_BUTTON);//  4/21/17 8:57 MW  Original TimeOut Value 1.97  Original Encoder Distance 53 Match 1 Value
																		//3:02 changed .7,49,1.87 to .7,56,2
		autoShootSequ(.7);											// 4/21/17 9:58 MW Original TimeOut Value 1.8 Original Encoder Distance 44 Match 2 Values
		if(gearMechOpen)
			toggleGearMech();

	}
	void outsideGearAuto(int allianceColor, bool alwaysKick)
	{
		EncoderStraightDrive(.9,84.0,T_RESET,1,DONT_USE_GEAR_BUTTON);  //1. 90,0.7;  2.88.1.5;   time 1.64
		if(allianceColor == BLUE_ALLIANCE)
		{
			PDTurnThreshold(-50,4);
		}															//Navigate to gear
		else
		{
			PDTurnThreshold(50,4);
		}
		EncoderStraightDrive(.6,60.0,T_RESET,1.2,USE_GEAR_BUTTON);



		if(hardware->gearButton->Get()==true)
		{
			wiggle();
			EncoderStraightDrive(.45,8,T_RESET,0.8,USE_GEAR_BUTTON);			//If gear button is not pressed then wiggle and drive forward
		}
		if(hardware->gearButton->Get()==false)
		{
			EncoderStraightDrive(-.45,2,T_RESET,0.3,USE_GEAR_BUTTON);			//if the gear button is pressed then drive backward a little
			gearSequence(false);
			EncoderStraightDrive(-.6,20.0,T_RESET,2,DONT_USE_GEAR_BUTTON);        //back up
			if(allianceColor == BLUE_ALLIANCE)
				PDTurnThreshold(50,2.5);
			else																//turn towards neutral zone
				PDTurnThreshold(-50,2.5);
			EncoderStraightDrive(.7,320.0,T_RESET,3.7,DONT_USE_GEAR_BUTTON);     //drive into neutral zone previously .7,250,3.7 AL
		}
		else                                                                  //if gear button is still not pressed
		{
			EncoderStraightDrive(-.6,20.0,T_RESET,2,DONT_USE_GEAR_BUTTON);        //back up
			if(alwaysKick)                                                      //kick if we have a ground gear mech
			{
				gearSequence(true);
				if(allianceColor == BLUE_ALLIANCE)
					PDTurnThreshold(60,2.5);
				else																//turn towards neutral zone
					PDTurnThreshold(-60,2.5);
				EncoderStraightDrive(1,250.0,T_RESET,3.5,DONT_USE_GEAR_BUTTON);     //drive into neutral zone
			}
		}
		if(gearMechOpen)
			toggleGearMech();


	}
	void boilerAuto1(int allianceColor, bool alwaysKick)
	{
		//measured 40 in from boiler
		autoShootSequ(.66);                              //Match 7: used .7 speed (4/21/17 11:29) AL
		AutoDrive(-.5,-.5);      //back up
		Wait(.3);
		AutoDrive(0,0);


		if(allianceColor == BLUE_ALLIANCE)
			swingTurn(52,.8,-.15,8);
		else //if RED_ALLIANCE             //turn out
			swingTurn(-52,-.15,.8,8);
		Wait(.3);


		EncoderStraightDrive(.6,47.0,T_RESET,1.75,false);     //Match 7: used 37 in and 1.4 sec (4/21/17 11:16) AL


		if(allianceColor == BLUE_ALLIANCE)
			swingTurn(85,.6,-.5,3);                          //Match 7: used 90 degree turn (4/21/17 11:26) AL
		else //if RED_ALLIANCE
			swingTurn(-85,-.5,.6,3);
		Wait(.3);
		EncoderStraightDrive(.6,90.0,T_RESET,3,true);


		if(hardware->gearButton->Get()==true)
		{
			wiggle();
			EncoderStraightDrive(.45,8,T_RESET,.8,USE_GEAR_BUTTON);			//If gear button is not pressed then wiggle and drive forward
		}
		if(hardware->gearButton->Get()==false)
		{
			EncoderStraightDrive(-.45,2,T_RESET,0.3,USE_GEAR_BUTTON);			//if the gear button is pressed then drive backward a little
			gearSequence(false);
			EncoderStraightDrive(-.6,20.0,T_RESET,2,DONT_USE_GEAR_BUTTON);        //back up
			if(allianceColor == BLUE_ALLIANCE)
				PDTurnThreshold(-50,2.5);
			else																//turn towards neutral zone
				PDTurnThreshold(50,2.5);
			EncoderStraightDrive(.7,250.0,T_RESET,3,DONT_USE_GEAR_BUTTON);     //drive into neutral zone
		}
		else                                                                  //if gear button is still not pressed
		{
			EncoderStraightDrive(-.6,20.0,T_RESET,2,DONT_USE_GEAR_BUTTON);        //back up
			if(alwaysKick)                                                      //kick if we have a ground gear mech
			{
				gearSequence(true);
				if(allianceColor == BLUE_ALLIANCE)
					PDTurnThreshold(-60,2.5);
				else																//turn towards neutral zone
					PDTurnThreshold(60,2.5);
				EncoderStraightDrive(1,250.0,T_RESET,3.5,DONT_USE_GEAR_BUTTON);     //drive into neutral zone
			}
		}
	}

	void AutonomousInit()
	{
		if(hardware->comp->GetClosedLoopControl()==false)
			hardware->comp->SetClosedLoopControl(true);
		if(gearMechOpen)
			toggleGearMech();
		int startPos = SmartDashboard::GetNumber("starting position", 1.0);
		bool kickGear = SmartDashboard::GetBoolean("Check if alliance has ground gear getter", false);

		if (SmartDashboard::GetNumber("Autonomous v1 v2 v3",1)==1)        //v1 is EncoderStraightDrive, v2 is Timers, v3 is nothing
		{
			if (SmartDashboard::GetString("alliance color","blue")=="blue"){
				switch(startPos){
				case 1:// left blue (boiler)
					boilerAuto1(BLUE_ALLIANCE,kickGear);
					break;
				case 2:// middle blue
					middleAuto(BLUE_ALLIANCE);
					break;
				case 3://right blue
					outsideGearAuto(BLUE_ALLIANCE,kickGear);
					break;
				default:
					 break;
				}
			}

			else if (SmartDashboard::GetString("alliance color","blue")=="red"){
				switch(startPos){
				case 1: // left red
					outsideGearAuto(RED_ALLIANCE,kickGear);
					break;
				case 2:// middle red
					middleAuto(RED_ALLIANCE);
					break;
				case 3: //right red(boiler)
					boilerAuto1(RED_ALLIANCE,kickGear);
					break;
				default:
					 break;
				}
			}
		}
		else if(SmartDashboard::GetNumber("Autonomous v1 v2 v3",1)==2)
		{
			EncoderStraightDrive(.6,300,T_RESET,3,USE_GEAR_BUTTON);
			if(hardware->gearButton->Get()==false)
				gearSequence(false);
		}
		else
		{

		}
	}



	void TeleopInit()
	{
		if(hardware->comp->GetClosedLoopControl()==false)
			hardware->comp->SetClosedLoopControl(true);
		if(gearMechOpen)
			toggleGearMech();
	}
	void TeleopPeriodic()
	{
		//HOPPER FLAP
		if(joystickAux->GetRawButton(THUMB_BOTRIGHT)==1||xboxDrive->GetRawButton(A_BUTTON)==1)
			hardware->hopper->Set(1);
		else
			hardware->hopper->Set(.1);



		//BOUNCERS AND INTAKE
		driver->setDriveControl(xboxDrive);
		hardware->climber->Set(-xboxDrive->GetRawAxis(LEFT_TRIGGER));
		if(xboxDrive->GetRawAxis(RIGHT_TRIGGER)>0.1||flyWheelOn||xboxDrive->GetBButton())
		{
			hardware->hopperBouncerLeft->Set(1);
			hardware->hopperBouncerRight->Set(-1);
			hardware->intake->Set(-1);
		}
		else
		{
			hardware->hopperBouncerLeft->Set(0);
			hardware->hopperBouncerRight->Set(0);
			hardware->intake->Set(0);
		}


		//SHOOTER
		if (joystickAux->GetRawButton(SIDE_BUTTON)==1)   //button 2 toggles intake, jostlers, and flywheel
			toggleFlywheel();
		if (joystickTest->GetRawButton(5)==1)
			toggleFlywheel();
		if(flyWheelOn)
		{
			//hardware->leftFlywheel->Set(-shooter);
			//hardware->rightFlywheel->Set(shooter);
			BANGBANG(.75);
		}
		if(xboxDrive->GetBButton()){
			BANGBANG(.55 );
		}

		else
		{
			hardware->rightFlywheel->Set(0);
			hardware->leftFlywheel->Set(0);
			flyWheelCount=1;
		}


		//CONVEYOR
		if(joystickAux->GetRawButton(JOYSTICK_TRIGGER)==1||xboxDrive->GetBButton())   //button 1 activates conveyor
			hardware->conveyor->Set(1);
		else
			hardware->conveyor->Set(0);




		//REVERSE CONVEYOR AND INTAKE
		if (joystickAux->GetRawButton(THUMB_BOTlEFT)==1)
		{
			hardware->intake->Set(1);
			hardware->conveyor->Set(-1);
		}


		//SHOOTER FINE ADJUSTMENT

		if(joystickAux->GetPOV() == 0)
			fineAdjust += 0.05;
		if(joystickAux->GetPOV() == 90)
			fineAdjust += 0.1;
		if(joystickAux->GetPOV() == 180)
			fineAdjust -= 0.05;
		if(joystickAux->GetPOV() == 270)
			fineAdjust -= 0.1;




		//MANUAL GEAR FUNCTIONS
		if(xboxDrive->GetRawButton(RIGHT_BUMPER)==1&&xboxDrive->GetRawButton(LEFT_BUMPER)==1)
			gearSequence(true);
		if (joystickAux->GetRawButton(THUMB_TOPRIGHT)==1)
			toggleGearMech();
		if (joystickAux->GetRawButton(THUMB_TOPLEFT)==1&&gearMechOpen)
			kick();


		//AUTOMATIC GEAR FUNCTIONS
		if(hardware->gearButton->Get()==false && gearMechReady && xboxDrive->GetRawButton(RIGHT_BUMPER)==1)
		{
			teleopTimer->Start();
			if(teleopTimer->Get()<2)
				gearSequence(true);
			else
				gearMechReady=false;
		}
		if(joystickAux->GetRawButton(7)==1)
		{
			teleopTimer->Stop();
			teleopTimer->Reset();
			gearMechReady=true;
		}
		if(teleopTimer->Get()>4)
		{
			teleopTimer->Stop();
			teleopTimer->Reset();
			gearMechReady=true;
		}


		if(joystickTest->GetRawButton(1)==1)
			EncoderStraightDrive(.5, 100, 5.0,5,true); //speed 10 distance 100 in, 5ms update time,5 sec timeout

		if(joystickTest->GetRawButton(2)==1)
			PDTurnThreshold(90, 3);

		if(joystickTest->GetRawButton(3)==1)
			PDTurnThreshold(-90, 3);

		if(joystickTest->GetRawButton(4)==1)
		{
			hardware->leftFlywheel->Set(-.5);
			hardware->rightFlywheel->Set(.5);
		}
		else if(joystickTest->GetRawButton(6)==1)
		{
			hardware->leftFlywheel->Set(-.7);
			hardware->rightFlywheel->Set(.7);
		}
		else if(joystickTest->GetRawButton(7)==1)
		{
			hardware->leftFlywheel->Set(-.9);
			hardware->rightFlywheel->Set(.9);
		}
		else if (joystickTest->GetRawButton(9)==1)
		{
			hardware->kicker->Set(.1);
		}
		else if(joystickTest->GetRawButton(10)==1)
		{
			hardware->kicker->Set(-.1);
		}
		else
			hardware->kicker->Set(0);


	}

};

START_ROBOT_CLASS(Robot);
