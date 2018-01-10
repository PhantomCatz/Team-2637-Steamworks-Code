
package org.usfirst.frc.team2637.robot;

import com.ctre.CANTalon;
import edu.wpi.first.wpilibj.IterativeRobot;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.RobotDrive;
import edu.wpi.first.wpilibj.Spark;
import edu.wpi.first.wpilibj.XboxController;

public class Robot extends IterativeRobot {
	CANTalon fright;
	CANTalon fleft;
	CANTalon rright;
	CANTalon rleft;
	CANTalon rshooter;
	CANTalon lshooter;
	XboxController xbox;
	RobotDrive driver;
	Joystick joy;
	Spark intake;
	Spark ljostler;
	Spark rjostler;
	Spark conveyor;
	Spark kicker;

	public Robot() 
	{
		fright = new CANTalon(4);
		fleft = new CANTalon(0);
		rright = new CANTalon(5);
		rleft = new CANTalon(1);
		rshooter = new CANTalon(3);
		lshooter = new CANTalon(2);
		xbox = new XboxController(1);
		driver = new RobotDrive(fleft, rleft, fright, rright);
		
		joy = new Joystick(0);
		intake = new Spark(2);
		ljostler = new Spark(4);
		rjostler = new Spark(5);
		kicker = new Spark(1);
		conveyor = new Spark(3);
	}


	@Override
	public void teleopPeriodic() 
	{
		driver.arcadeDrive(-(xbox.getRawAxis(1)), -(xbox.getRawAxis(4)));
		
		boolean shooterToggle = false;
		if (joy.getRawButton(2) == true) {
			shooterToggle = !shooterToggle;
		}
		
		if (shooterToggle) {
			rshooter.set(.65);
			lshooter.set(-.65);
			ljostler.set(.5);
			rjostler.set(.5);
		} 
		
		else {
			rshooter.set(0);
			lshooter.set(0);
			ljostler.set(0);
			rjostler.set(0);

		}

		if (joy.getRawButton(1)) {
			conveyor.set(1);
		} 
		else {
			conveyor.set(0);
		}

		if (xbox.getRawAxis(3)>0.5) {
			intake.set(-1);
		} 
		else {
			intake.set(0);
		}

	}
}
