/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
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
#include "api.h"

#define DRIVE_FL 6 //Left drive 127 is forward
#define DRIVE_ML 7
#define DRIVE_MR 8 //right drive -127 is forward
#define DRIVE_FR 9

#define ARM_TL 2 //-127 is up? lolsure
#define ARM_TR 3 //-127 is up
#define ARM_BL 4 //-127 is up
#define ARM_BR 5 //127 is up
#define arm_idle_speed 8

#define IN_L 1 //127 should be intake
#define IN_R 10

#define led_r 6
#define led_g 8
#define limit_top 3
#define limit_bot 2
#define arm_pot 1

void drivefwd(int ime, int speed);
void drivebwd(int ime, int speed);
void cw90();
void ccw90();

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonomous() {
	int count;
	imeReset(0);

		//drive forward
		drivefwd(-920, 50); //"920clicks" 50 speed
		delay(400);
		//drive back
		drivebwd(620, 50); //"480clicks" 50speed
		delay(400);
		//rotate cw 90
		cw90();
		delay(400);

		drivefwd(-700,50);
		delay(400);

		cw90();
		delay(400);

		drivefwd(-700,50);
		delay(400);

		drivebwd(-700,50);
		delay(400);

		ccw90();
		delay(400);

		drivefwd(-1500,60);
		delay(400);

		imeReset(0);
while(1){
		imeGet(0, &count);
		printf("%d\r\n", count);
		delay(50);
}
}

/////functions
void drivefwd(int ime, int speed){ //negative imes::0 to -620= i rotation
	int count;

	imeReset(0);
	do{
		imeGet(0, &count);
		motorSet(DRIVE_FL, speed);
		motorSet(DRIVE_ML, speed);
		motorSet(DRIVE_FR, -speed);
		motorSet(DRIVE_MR, -speed);
		motorSet(IN_L, (2*speed));
		motorSet(IN_R, -(2*speed));
	}while(count>(ime));

	motorStop(DRIVE_FL);
	motorStop(DRIVE_ML);
	motorStop(DRIVE_FR);
	motorStop(DRIVE_MR);
	delay(1000);
	motorStop(IN_L);
	motorStop(IN_R);
	return;
}

void drivebwd(int ime, int speed){ //positive imes:: 0 to 620 = 1 rotation
	int count;

	imeReset(0);
	do{
		imeGet(0, &count);
		motorSet(DRIVE_FL, -speed);
		motorSet(DRIVE_ML, -speed);
		motorSet(DRIVE_FR, speed);
		motorSet(DRIVE_MR, speed);
	}while(count<ime);

	motorStop(DRIVE_FL);
	motorStop(DRIVE_ML);
	motorStop(DRIVE_FR);
	motorStop(DRIVE_MR);
}
void cw90(){
	int count;
	imeReset(0);
	do{
		imeGet(0, &count);
		motorSet(DRIVE_FL, 50);
		motorSet(DRIVE_ML, 50);
		motorSet(DRIVE_FR, 50);
		motorSet(DRIVE_MR, 50);
	}while(count>(-640));
	motorStop(DRIVE_FL);
	motorStop(DRIVE_ML);
	motorStop(DRIVE_FR);
	motorStop(DRIVE_MR);
}
void ccw90(){
	int count;
	imeReset(0);
	do{
		imeGet(0, &count);
		motorSet(DRIVE_FL, -50);
		motorSet(DRIVE_ML, -50);
		motorSet(DRIVE_FR, -50);
		motorSet(DRIVE_MR, -50);
	}while(count<(640));
	motorStop(DRIVE_FL);
	motorStop(DRIVE_ML);
	motorStop(DRIVE_FR);
	motorStop(DRIVE_MR);
}
