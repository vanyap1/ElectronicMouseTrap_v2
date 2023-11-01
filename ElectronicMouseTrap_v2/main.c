/*
 * Mouses_catcher_v2.c
 *
 * Created: 30.08.2023 16:41:36
 * Author : Ivan Prints
 */ 

#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "string.h"
#include "stdbool.h"
#include "lib/uart_hal.h"
#include "lib/gpio_driver.h"
#include "lib/adc_hal.h"

gpio open_btn = {(uint8_t *)&PORTD , PORTD2};		//Button for opening (charge) door	
gpio ir_sens = {(uint8_t *)&PORTD , PORTD3};		//IR sensor interrup input (for door activating)
gpio ir_led = {(uint8_t *)&PORTD , PORTD4};			//IR LED
gpio status_led = {(uint8_t *)&PORTB , PORTB5};		//Status led, used for debugging
gpio servo = {(uint8_t *)&PORTB , PORTB1};			//Servo PWM output dedicated to PWM TIMER1 output
gpio buzer = {(uint8_t *)&PORTE, PORTE2};			//Buzer output pin, wit software vawe generating.
	
	
#define debugFlg			false				//true if need debug messages
#define closeValue			38					//PWM value for servo when state i close 
#define openValue			25					//PWM value for servo when state i open
#define servoTime			1500				//Servo timeout, after this time MCU going to sleep mode
#define OPEN				true				//predefinned status value
#define CLOSE				false				//predefinned status value

#define buzerEnable			true				//true if buzzer is need
#define buzerPulsesCount	1000				//buser pulses
#define buserPulseTime		100					//buser pulses period in uS

bool openRequest = false;						
bool closeReques = false;
bool currentState = CLOSE;
uint8_t servoValue;


ISR(INT0_vect){
	openRequest=true; 
}


ISR(INT1_vect){
	closeReques=true; 
}

void sound(void);

int main(void)
{
    uart_init(250000,0);
	set_pin_dir(&open_btn , PORT_DIR_IN); set_pin_level(&open_btn, true);
	set_pin_dir(&ir_sens , PORT_DIR_IN); //set_pin_level(&ir_sens, true);
	set_pin_dir(&ir_led , PORT_DIR_OUT); set_pin_level(&ir_led, true);
	set_pin_dir(&buzer , PORT_DIR_OUT); set_pin_level(&buzer, false);
	set_pin_dir(&status_led , PORT_DIR_OUT); set_pin_level(&status_led, false);
	set_pin_dir(&servo , PORT_DIR_OUT);
	
	EICRA |= (0b10 << ISC00);
	EICRA |= (0b01 << ISC10);
	EIMSK |= 1 << INT0 | 1 << INT1;
	
	TCCR1A = (0b10 << COM1A0) | (0b10 << COM1B0) | (0b01 << WGM10);
	TCCR1B = (0b0 << WGM12) | (0b100 << CS10);
	OCR1A = 128;
	sleep_enable();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	
	sei();
    while (1) 
    {
		if (openRequest){
			debugFlg ? uart_send_string((uint8_t *)"open_request\n\r") : NULL; 
			OCR1A = openValue;
			sound();
			openRequest=false;
			currentState = OPEN;
			_delay_ms(servoTime);				
		}
		
		if (closeReques & currentState == OPEN){
			set_pin_level(&status_led, true);
			debugFlg ? uart_send_string((uint8_t *)"close_request\n\r") : NULL; 
			closeReques=false;
			OCR1A = closeValue;
			sound();
			currentState = CLOSE;
			_delay_ms(servoTime);
			set_pin_level(&status_led, false);
		}
		
		if(buzerEnable & currentState == CLOSE){
			sound();
		}
		sleep_cpu();
    }
}



void sound(void){
	debugFlg ? uart_send_string((uint8_t *)"sound_request\n\r") : NULL;
	for (int i = 0; i < buzerPulsesCount; i++) {
		_delay_us(buserPulseTime);
		toggle_pin_level(&buzer);
	}
	set_pin_level(&buzer, false);
}