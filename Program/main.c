//include main header file

#include"main.h"

# define P_DIS		27

//DEFINE GLOBAL VARIABLES
unsigned int elapsed_sec ;
unsigned int elapsed_min ;
unsigned int elapsed_time ;
unsigned int time ;


int main(void)
{
	unsigned char i;
	unsigned short angle;

	MainSystemInitialize();

	for(;;) {
			
		MainMoveForward();

		MainMoveBackward(7);

		for (i = 0; i < 8; i++) {
			angle = ZERO_DEGREE - (50 * i);
			OCR1A = angle;
			_delay_ms(150); 
		}  
			
		MainMoveBackward(17);
		
			
		for (i = 0; i < 16; i++) {
			angle += (50 * i); 
			OCR1A = angle;
			_delay_ms(100); 
		}  
		
		MainMoveBackward(23);

		for(i = 0; i < 9; i++)	{
			OCR1A = angle - (50 * i );
			_delay_ms(150); 
		} 

		OCR1A = ZERO_DEGREE;

		MOTOR_PORT |= _BV(MOTOR1_ANODE); 
		MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
		_delay_ms(500); 

		StopMotor();
		lcdclrr(1);
		lcdws(" Vehicle Parked");
		for(;;);
		
	}
}
static void MainBlink(void)
{
	ledon();
	Delay(50);
	ledoff();
	Delay(50);
}

static void MainSystemInitialize(void)
{
	MainInitializePorts();
	MainServoInitialize();
	inittest();
	lcdclr();
	lcdws("Auto Parking Veh");
}
static void MainServoInitialize(void)
{
	TCCR1A |= _BV(COM1A1)| _BV(WGM11) ;				  //FAST PWM ,TOP IS ICR1, CLEAR ON COMPARE MATCH
	TCCR1B |= _BV(WGM13)| _BV(WGM12)| _BV(CS11);      // CLOCK PRESCALAR BY 8
	ICR1 = 20000;					                  // FREQUENCY IS 50 HZ
	OCR1A =  ZERO_DEGREE;
}
void  MainMoveForward(void)
{
	lcdclrr(1);
	lcdws(" Vehicle Moving");
	MOTOR_PORT |= _BV(MOTOR1_ANODE); 
	MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
	ForwardDelay(3500);
	StopMotor();
} 
void MainMoveBackward(unsigned int Delay)
{
	 lcdclrr(1);
 	 lcdws("Vehicle Parking");
	 MOTOR_PORT &= ~(_BV(MOTOR1_ANODE));	
	 MOTOR_PORT |= _BV(MOTOR1_CATHODE);
	 BackwardDelay(Delay);
	 StopMotor();
}

void StopMotor(void)
{
	MOTOR_PORT &= ~(_BV(MOTOR1_ANODE));
	MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
}
static void MainInitializePorts(void)
{
	MOTOR_DDR  |= _BV(MOTOR1_ANODE);
	MOTOR_DDR  |= _BV(MOTOR1_CATHODE);
	
	MOTOR_PORT &= ~(_BV(MOTOR1_ANODE));
	MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
	
	SERVO_DDR  |= _BV(SERVO1)  ;
	SERVO_PORT &= ~(_BV(SERVO1));
	
	SENSOR_DDR &= ~(_BV(FRONT_IR) | _BV(BACK_IR)); 
	SENSOR_PORT |= (_BV(FRONT_IR) | _BV(BACK_IR)); 
	
	ULT_SENSOR_DDR &= ~(_BV(ULTRASONIC) | _BV(ALCO_SEN));
	ULT_SENSOR_PORT |=( _BV(ULTRASONIC) | _BV(ALCO_SEN));

	D_SEN_DDR &= ~(_BV(DOOR_SEN));
	D_SEN_PORT |= (_BV(DOOR_SEN));

	
	buzinit();
	beep(1,100);
	lcdinit();

	
}
void Delay(unsigned int pause)
{
	_delay_ms(pause); 
}
void ForwardDelay(unsigned int pause)
{
	unsigned int Distance = 0;
	
	while (bit_is_set(ULT_SENSOR_PIN, ULTRASONIC))
	{
		Delay(100); 
		if(bit_is_set(SENSOR_PIN, FRONT_IR)) {
			StopMotor();
			while(bit_is_set(SENSOR_PIN, FRONT_IR));
		} else {
			MOTOR_PORT |= _BV(MOTOR1_ANODE); 
			MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
		}
	}
	beep(1,250 );
		
	while(bit_is_clear(ULT_SENSOR_PIN, ULTRASONIC))
	{
		Delay(100);  
		if(bit_is_set(SENSOR_PIN, FRONT_IR)) {
			StopMotor();
			while(bit_is_set(SENSOR_PIN, FRONT_IR));
		} else {
			MOTOR_PORT |= _BV(MOTOR1_ANODE); 
			MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
		}
	}

	beep(1,250 );
	while(bit_is_set(ULT_SENSOR_PIN, ULTRASONIC))
	{
		Delay(100); 
		if(bit_is_set(SENSOR_PIN, FRONT_IR)) {
			StopMotor();
			while(bit_is_set(SENSOR_PIN, FRONT_IR));
		} else {
			MOTOR_PORT |= _BV(MOTOR1_ANODE); 
			MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
		}
		Distance++;
		
	}
	if (Distance < P_DIS){
		MOTOR_PORT |= _BV(MOTOR1_ANODE); 
		MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));
		Delay(2000);
		StopMotor();
		lcdclrr(1);
		lcdws("  No Park Dist");
		for(;;) {
			beep(1,50 );
			Delay(500);
			MainBlink();
		}
	}
	beep(2,200 );
	lcdclrr(1);
	lcdws("Park Dist Found");

	MOTOR_PORT |= _BV(MOTOR1_ANODE); 
	MOTOR_PORT &= ~(_BV(MOTOR1_CATHODE));

	Delay(1000);
	StopMotor();
	Delay(1000);

}
void BackwardDelay(unsigned int pause)
{
	while(pause--)
	{
		beep(1,50); 
		if(bit_is_set(SENSOR_PIN, BACK_IR))	{
			StopMotor();
			while(bit_is_set(SENSOR_PIN, BACK_IR));
		} else {
			MOTOR_PORT &= ~(_BV(MOTOR1_ANODE));	
			MOTOR_PORT |= _BV(MOTOR1_CATHODE);
		}
	}
}
static void inittest(void)
{
	lcdclr();
	lcdws("Chk Alcol:");
	if (AL_SENSOR_PIN & _BV(ALCO_SEN)) {
		lcdws("NOT OK");
		while (AL_SENSOR_PIN & _BV(ALCO_SEN)) {
			beep(1,100);
			dlyms(2000);
		}
	} else {
		lcdws("OK");
		dlyms(1000);
	}
	lcdr2();
	lcdws("Chk Doors:");
	if (D_SEN_PIN & _BV(DOOR_SEN)) {
		lcdws("NOT OK");
		while (D_SEN_PIN & _BV(DOOR_SEN)) {
			beep(1,100);
			dlyms(2000);
		}
	} else {
		lcdws("OK");
		dlyms(1000);
	}
}
