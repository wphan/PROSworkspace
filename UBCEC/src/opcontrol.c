/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2013, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

#define ARM 5 //Up 127
#define DRIVERR 1 //Forward 127
#define DRIVEFR 2
#define DRIVEFL 9 //Forward -127
#define DRIVERL 10

//Pot 500 is down, 1000 is up
//Line 2000 is concrete, 170 is paper

void drive(int pwr) {
	motorSet(DRIVERR, pwr);
	motorSet(DRIVEFR, pwr);
	motorSet(DRIVEFL, -pwr);
	motorSet(DRIVERL, -pwr);
}

/*
 * Pivot around center
 * @param pwr duh
 * @param dir 1 is left, -1 is right
 */
void driveTurn(int pwr, int dir) {
	motorSet(DRIVERR, dir*pwr);
	motorSet(DRIVEFR, dir*pwr);
	motorSet(DRIVEFL, dir*pwr);
	motorSet(DRIVERL, dir*pwr);
}

/*
 * Arrest drive motion
 * @param dir Direction of prior travel to arrest. 1 is forward, -1 is back
 */
void driveBrake( int dir ) {
	drive(-dir*50);
	delay(20);
	drive(0);
}

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the schedular is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {

	while (1) {
		int pot = analogRead(1);
		int lineCenter,
			lineRight,
			lineLeft;
		int stage = 0;

		//Raise arm to open
		while (pot < 1000) {
			motorSet(ARM, 127);
			pot = analogRead(1);
		}
		motorStop(ARM);

		/*printf("\nlineRight = %d", analogRead(4));
		delay(50);*/

		lineRight = analogRead(4);
		while (lineRight > 500) {
			drive(127);
			lineRight = analogRead(4);
		}
		drive(0);

		driveTurn(80, -1);
		delay(500);
		drive(0);

		lineCenter = analogRead(3);
		lineLeft = analogRead(2);
		lineRight = analogRead(4);
		motorSet(DRIVERR, 50);
		motorSet(DRIVEFR, 50);
		motorSet(DRIVEFL, -80);
		motorSet(DRIVERL, -80);
		while (stage == 0) {
			delay(20);

			lineCenter = analogRead(3);
			lineLeft = analogRead(2);
			lineRight = analogRead(4);

			if ((lineCenter || lineLeft || lineRight) < 300)
				stage = 1;
		}

		while (stage < 2) {
			if (lineCenter < 300) {
				drive(127);
			} else if (lineLeft < 300) {
				motorSet(DRIVERR, 80);
				motorSet(DRIVEFR, 80);
				motorSet(DRIVEFL, -50);
				motorSet(DRIVERL, -50);
			} else if (lineRight < 300) {
				motorSet(DRIVERR, 50);
				motorSet(DRIVEFR, 50);
				motorSet(DRIVEFL, -80);
				motorSet(DRIVERL, -80);
			} /*else {
				stage = 2;
			}*/
			delay(20);
			lineCenter = analogRead(3);
			lineLeft = analogRead(2);
			lineRight = analogRead(4);
		}
		drive(0);

		delay(1000);

		pot = analogRead(1);
		while (pot > 600) {
			motorSet(ARM, -127);
			pot = analogRead(1);
		}
		motorSet(ARM, 50);
		delay(20);
		motorStop(ARM);

		delay(20000);
	}
}
