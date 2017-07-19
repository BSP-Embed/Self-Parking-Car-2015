#ifndef MAIN_H
#define MAIN_H

#define F_CPU 7327800l

//INCLUDE HEADER FILE
#include <avr/io.h>
#include<avr/eeprom.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#include "includes.h"


//DEFINE PORTS/PINS
#define MOTOR_DDR			DDRC
#define MOTOR_PORT			PORTC
#define MOTOR1_ANODE		PC2
#define MOTOR1_CATHODE		PC3

#define SERVO_DDR			DDRD
#define SERVO_PORT			PORTD
#define SERVO1				PD5

#define SENSOR_PORT		PORTB
#define SENSOR_DDR		DDRB
#define SENSOR_PIN		PINB

#define FRONT_IR		PB0
#define BACK_IR			PB1

#define ULT_SENSOR_DDR	DDRD
#define ULT_SENSOR_PORT	PORTD
#define ULT_SENSOR_PIN	PIND
#define ULTRASONIC		PD1

#define AL_SENSOR_DDR	DDRD
#define AL_SENSOR_PORT	PORTD
#define AL_SENSOR_PIN	PIND
#define ALCO_SEN		PD0


#define D_SEN_PORT		PORTD
#define D_SEN_DDR		DDRD
#define D_SEN_PIN		PIND

#define DOOR_SEN		PD2



//DEFINE CONSTANT
#define		 OUTPUTS		0xFF
#define 	ZERO_DEGREE		1400

//DEFINE USER DEFINED DATA TYPE
typedef unsigned char MainUbyte;

//DEFINE FUNCTION PROTOTYPE
int main(void);
static void MainSystemInitialize(void);
static void MainInitializePorts(void);
static void MainServoInitialize(void);
void Delay(unsigned int pause);
void  MainMoveForward(void);
void MainMoveBackward(unsigned int Delay);
void StopMotor(void);
static void MainMoveLeft(void);
static void MainMoveRight(void);
static void MainBlink(void);
static void InitializeTimer(void) ;
static void StartTimer(void) ;
unsigned int CalculateTime(void) ;
void BackwardDelay(unsigned int pause);
void ForwardDelay(unsigned int pause);
void BackwardDelay(unsigned int pause);
void MainMoveBackwardSlow(void);
static void inittest(void);
#endif

