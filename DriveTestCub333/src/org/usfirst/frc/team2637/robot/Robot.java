package org.usfirst.frc.team2637.robot;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.Compressor;
import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.IterativeRobot;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.RobotDrive;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.Spark;
import edu.wpi.first.wpilibj.SpeedControllerGroup;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;

public class Robot extends IterativeRobot {
	WPI_TalonSRX fright;
	WPI_TalonSRX fleft;
	WPI_TalonSRX rright;
	WPI_TalonSRX rleft;
	SpeedControllerGroup right;
	SpeedControllerGroup left;
	DifferentialDrive driver;
	
	Spark kicker;
	CatxXboxController xbox;
	//RobotDrive driver;
	Joystick joy;
	//Compressor comp;
	Solenoid gear;
	
	DigitalInput gearButton;
	
	Timer kickTimer;
	
	boolean shooterToggle;
	boolean gearMechOpen;
	
	public Robot() 
	{
		fright = new WPI_TalonSRX(4);
		fleft = new WPI_TalonSRX(0);
		rright = new WPI_TalonSRX(5);
		rleft = new WPI_TalonSRX(1);
		right = new SpeedControllerGroup(fright,rright);
		left = new SpeedControllerGroup(fleft,rleft);
		driver = new DifferentialDrive(left, right);
		xbox = new CatxXboxController(1);
		
		
		//driver = new RobotDrive(fleft, rleft, fright, rright);
		
		joy = new Joystick(0);
		kicker = new Spark(1);
		//comp = new Compressor(6);
		gear = new Solenoid(1);
		
		gearButton = new DigitalInput(9);
		
		kickTimer = new Timer();
		
		
		
	}


	public void teleopInit() {
		driver.arcadeDrive(xbox.GetLeftStickY(), -xbox.GetRightStickX());
		//comp.setClosedLoopControl(true);
		gearMechOpen = false;
	}
	
	public void toggleGearMech() {
		gearMechOpen = !gearMechOpen;
		gear.set(gearMechOpen);
		
	}
	
	public void kick() {
		kickTimer.reset();
		while (kickTimer.get() < 0.5) //0.4
		{
			kicker.set(1);
			driver.arcadeDrive(xbox.GetLeftStickY(), -xbox.GetRightStickX());
		}
		kickTimer.reset();
		while (kickTimer.get() < .5)
		{
			kicker.set(-0.5);       //original speed -0.4  DD 10-7-17
			driver.arcadeDrive(xbox.GetLeftStickY(), -xbox.GetRightStickX());
		}
		kicker.set(0);
		kickTimer.stop();
	}
	
	void gearSequence()
	{
		
		driver.arcadeDrive(xbox.GetLeftStickY(), -xbox.GetRightStickX());
		this.toggleGearMech();
		kickTimer.reset();
		kickTimer.start();
		while(kickTimer.get()<.4)
		{
			
		}
		this.kick();
		kickTimer.reset();
		kickTimer.start();
		while(kickTimer.get()<.4)
		{
			
		}
		this.toggleGearMech();

	}

	
	@Override
	public void teleopPeriodic() 
	{
		driver.arcadeDrive(xbox.GetLeftStickY(), -xbox.GetRightStickX());
		if(xbox.GetRightBumper()) {
			this.gearSequence();
		}

	}
}
