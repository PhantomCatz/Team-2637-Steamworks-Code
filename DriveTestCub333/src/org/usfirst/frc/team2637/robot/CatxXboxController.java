package org.usfirst.frc.team2637.robot;

import edu.wpi.first.wpilibj.XboxController;

public class CatxXboxController
{
	final int A_BUTTON = 1;
	final int B_BUTTON = 2;
	final int X_BUTTON = 3;
	final int Y_BUTTON = 4;
	final int LEFT_BUMPER = 5;
	final int RIGHT_BUMPER = 6;
	final int LEFT_STICK_X = 0;
	final int LEFT_STICK_Y = 1;
	final int LEFT_TRIGGER = 2;
	final int RIGHT_TRIGGER = 3;
	final int RIGHT_STICK_X = 4;
	final int RIGHT_STICK_Y = 5;
	final String NAME;
	XboxController xbox;
	
	
	public CatxXboxController(int port)
	{
		NAME = this.getClass().getSimpleName();
		xbox = new XboxController(port);
		
	}
	public boolean GetAButton()
	{

		return xbox.getRawButton(A_BUTTON);
	}
	public boolean GetBButton()
	{

		return xbox.getRawButton(B_BUTTON);
	}
	public boolean GetXButton()
	{

		return xbox.getRawButton(X_BUTTON);
	}
	public boolean GetYButton()
	{

		return xbox.getRawButton(Y_BUTTON);
	}
	public boolean GetLeftBumper()
	{

		return xbox.getRawButton(LEFT_BUMPER);
	}
	public boolean GetRightBumper()
	{

		return xbox.getRawButton(RIGHT_BUMPER);
	}
	public double GetLeftStickX()
	{

		return xbox.getRawAxis(LEFT_STICK_X);
	}
	public double GetLeftStickY()
	{

		return -xbox.getRawAxis(LEFT_STICK_Y);
	}
	public double GetRightStickX()
	{

		return -xbox.getRawAxis(RIGHT_STICK_X);
	}
	public double GetRightStickY()
	{

		return xbox.getRawAxis(RIGHT_STICK_Y);
	}
	public double GetRightTrigger()
	{

		return xbox.getRawAxis(RIGHT_TRIGGER);
	}
	public double GetLeftTrigger()
	{

		return xbox.getRawAxis(LEFT_TRIGGER);
	}
}
