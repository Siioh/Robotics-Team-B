#pragma config(Sensor, in1,    gyroscope,      sensorGyro)
#pragma config(Sensor, dgtl1,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncoder,    sensorQuadEncoder)
#pragma config(Motor,  port2,           left,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           right,         tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           mobileGoal,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           mainArm,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           clawArm,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           claw,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,           ,             tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

// -- Define Variables --
volatile int gyroReading, currentLeftEncoderReading, currentRightEncoderReading;

int normalSpeed = 127;
int slowSpeed = 90;

int userWaitTime = 50; //Time between starting and stopping movement when holding a button

int userMobileGoalSpeed = 127; //Speed of other motors
int userMainArmSpeed = 127;
int userClawArmSpeed = 127;
int userClawSpeed = 127;

string userBotOrientation = "forward"; //Bot starts forward
// -- End Define Variables --

// -- Define Controls --
string rightDrivetrainCtrl = Ch2;
string leftDrivetrainCtrl = Ch3;
string mobileGoalUpCtrl = Btn6U;
string mobileGoalDownCtrl = Btn6D;
string mainArmUpCtrl = Btn5UXmtr2;
string mainArmDownCtrl = Btn5DXmtr2;
string clawArmUpCtrl = Btn6UXmtr2;
string clawArmDownCtrl = Btn6DXmtr2;
string clawOpenCtrl = Btn8UXmtr2;
string clawCloseCtrl = Btn8DXmtr2;
string rotationSwitchCtrl = Btn8D;
// -- End Define Controls --

// -- Define functions --
void resetSensors() {
	SensorValue[leftEncoder] = 0; //Reset drivetrain encoders
	SensorValue[rightEncoder] = 0;
	SensorValue[gyroscope] = 0; //Reset gyro
}

/*void motorsForwardAutoStraighten(int drivetrainSpeed) {
if(SensorValue[rightEncoder] > SensorValue[leftEncoder]) { //If right moved more, slow it.
motor[rightFront] = drivetrainSpeed - errorSpeed;
motor[rightBack] = drivetrainSpeed - errorSpeed;
} else {
if(SensorValue[rightEncoder] < SensorValue[leftEncoder]) { //Left moved more?  Slow it.
motor[leftFront] = drivetrainSpeed - errorSpeed;
motor[leftBack] = drivetrainSpeed - errorSpeed;
} else {
motorsForward(drivetrainSpeed); //If totally straight, go straight.
}
}
}*/

/*void pidDrivetrainControl() {
runTimer = true; //Give timer permission to run
while(<active> for some reason) {
if(currentTimeReading != null) { //If generated a second reading
leftReadingDiff /= timeDiff; //Calculate velocity of the sides in terms of degrees per ms
rightReadingDiff /= timeDiff;

abs(leftReadingDiff - rightReadingDiff); //Error of velocities
}

}
runTimer = false; //Turn timer off
}*/

void drivetrainStop() { //Stop drivetrain
	motor[right] = 0;
	motor[left] = 0;
}

void leftMotors(int setSpeed) {
	motor[left] = setSpeed;
}

void rightMotors(int setSpeed) {
	motor[right] = setSpeed;
}

int rotationCalculate(int orientatedSpeed) {
	if (userBotOrientation == "forward") { //If forward, orientedSpeed is the normal.  Otherwise, make backwards be forwards.
		return orientatedSpeed;
		} else {
		orientatedSpeed *= -1;
		return orientatedSpeed;
	}
}

void joystickControl() { //Joystick control for drivetrain
	motor[right] = rotationCalculate(vexRT[rightDrivetrainCtrl]);
	motor[left] = rotationCalculate(vexRT[leftDrivetrainCtrl]);
}

void autonomousStraight(int degreesRotation) {
	while (currentLeftEncoderReading < degreesRotation) {
		if (SensorValue[leftEncoder] < SensorValue[rightEncoder]) {
			leftMotors(normalSpeed);
			rightMotors(slowSpeed);
		}

		if (SensorValue[leftEncoder] > SensorValue[rightEncoder]) {
			leftMotors(slowSpeed);
			rightMotors(normalSpeed);
		}

		if (SensorValue[leftEncoder] == SensorValue[rightEncoder]) {
			leftMotors(normalSpeed);
			rightMotors(normalSpeed);
		}
	}
}

void autonomousTurn(int degreesRotation) {
	if(degreesRotation > gyroReading) {
		while (gyroReading < degreesRotation) {
			leftMotors(normalSpeed);
			rightMotors(-normalSpeed);
		}
		} else {
		while (gyroReading > degreesRotation) {
			leftMotors(-normalSpeed);
			rightMotors(normalSpeed);
		}
	}
	drivetrainStop();

}
// -- End Define Functions --


// -- Define Tasks --
task checkTimer1() {
	/*while(true) {
	if(runTimer == true) {
	ClearTimer[T1]; //Reset Timer
	initialTimeReading = time1[T1]; //Take start time reading
	resetSensors(); //Reset Encoders
	initialLeftEncoderReading = SensorValue[leftEncoder]; //Take start encoder readings
	initialRightEncoderReading = SensorValue[rightEncoder];
	while(runTimer == true) {
	if( (time1[T1] - initialTimeReading) > 100) { //If time difference is greater than 100ms
	currentTimeReading = time1[T1]; //Take time value
	currentLeftEncoderReading = SensorValue[leftEncoder]; //Take encoder values
	currentRightEncoderReading = SensorValue[rightEncoder];

	timeDiff = currentTimeReading - initialTimeReading; //Time diff
	leftReadingDiff = currentLeftEncoderReading - initialLeftEncoderReading; //Encoders diff
	rightReadingDiff = currentRightEncoderReading - initialRightEncoderReading;

	ClearTimer[T1]; //Reset Timer
	initialTimeReading = time1[T1]; //Take start time reading
	resetSensors(); //Reset Encoders
	initialLeftEncoderReading = SensorValue[leftEncoder]; //Take start encoder readings
	initialRightEncoderReading = SensorValue[rightEncoder];
	}
	wait1Msec(10); //Wait 10 ms before restarting this loop
	}
	} else {
	wait1Msec(10); //Wait 10 ms before restarting this loop
	}*/
}

task simultaneousMainArm() {
	while(true) {
		if (vexRT[mainArmUpCtrl] == 1) {
			motor[mainArm] = userMainArmSpeed;
			} else if (vexRT[mainArmDownCtrl] == 1) {
			motor[mainArm] = -userMainArmSpeed;
			} else {
			motor[mainArm] = 0;
		}
	}
}

task simultaneousClawArm() {
	while(true) {
		if (vexRT[clawArmUpCtrl] == 1) {
			motor[clawArm] = userClawArmSpeed;
			} else if (vexRT[clawArmDownCtrl] == 1) {
			motor[clawArm] = -userClawArmSpeed;
			} else {
			motor[clawArm] = 0;
		}
	}
}

task simultaneousRotation() {
	while(true) {
		if (vexRT[rotationSwitchCtrl] == 1) { //Swap rotation
			if(userBotOrientation == "forward") { //If forward, switch to back.  Otherwise, switch back to forward.
				userBotOrientation = "back";
				} else {
				userBotOrientation = "forward";
			}
			wait1Msec(userWaitTime);
		}
	}
}

task simultaneousClaw() {
	while(true) {
		if (vexRT[clawOpenCtrl] == 1) {
			motor[claw] = userClawSpeed;
			} else if (vexRT[clawCloseCtrl] == 1) {
			motor[claw] = -userClawSpeed;
			} else {
			motor[claw] = 0;
		}
	}
}

task simultaneousMobileGoal() {
	while (true) {
		if (vexRT[mobileGoalUpCtrl] == 1) {             //If control to move mobile goal up, then move it up!
			motor[mobileGoal] = userMobileGoalSpeed;
			} else if (vexRT[mobileGoalDownCtrl] == 1) {    //Else, check if it should go down
			motor[mobileGoal] = -userMobileGoalSpeed;
			} else {                                        //If neither up nor down, keep it off.
			motor[mobileGoal] = 0;
		}
	}
}

task getSensorValues() {
	while (true) {
		gyroReading = SensorValue[gyroscope];
		currentRightEncoderReading = SensorValue[rightEncoder];
		currentLeftEncoderReading = -SensorValue[leftEncoder];
		wait1Msec(50);
	}
}
// -- End define tasks --

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

// -- Pre-autonomous task --
void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...

	resetSensors();
	SensorValue[gyroscope] = 0;

	startTask(checkTimer1); //Start checking the timer
}
// -- End Pre-autonomous task --

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// -- Main autonomous task --
task autonomous()
{
	startTask(getSensorValues);
	while (true) {

		autonomousStraight(360); //Go straight for 360 deg of rotation
		autonomousTurn(360); //Turn 360 deg.

	}
	drivetrainStop();
}
// -- End autonomous task --

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// -- Start User Control Task --
task usercontrol()
{
startTask(simultaneousMainArm);
startTask(simultaneousClawArm);
startTask(simultaneousClaw);
startTask(simultaneousRotation);
startTask(simultaneousMobileGoal);
resetSensors();
startTask(getSensorValues);
while (true) {
	joystickControl();
}
}
// -- End User Control Task --
