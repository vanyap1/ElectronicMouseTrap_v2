
/*
 * cig14.c
 *
 * Created: 07.05.2023 18:57:42
 * Author: Ivan Prints 
 * Email: princ.va@gmail.com
 */ 


#include "../config.h"
#include <avr/io.h>

#include <stdbool.h>

enum pin_pull_mode {
	PORT_PULL_OFF,
	PORT_PULL_UP,
};

enum port_dir {
	PORT_DIR_IN,
	PORT_DIR_OUT,
	PORT_DIR_OFF,
};

typedef struct
{
	uint8_t *port;
	uint8_t pin ;
} gpio;

typedef struct gpio_port_setup
{
	uint8_t *port;
	uint8_t data ;
} gpio_port;

void set_port_direction(gpio_port *GPIOx, const uint8_t mask, const uint8_t init_value);
void set_pin_dir(gpio *GPIOx, const enum port_dir direction);
void set_pin_pull_mode(gpio *GPIOx, const enum pin_pull_mode pull_mode);
void set_port_level(gpio *GPIOx, const uint8_t port_value);
void set_pin_level(gpio *GPIOx, const bool level);
void toggle_pin_level(gpio *GPIOx);
uint8_t get_port_level(gpio *GPIOx);
bool get_port_pin_level(gpio *GPIOx);





