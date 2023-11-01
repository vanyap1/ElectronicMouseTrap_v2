
/*
 * cig14.c
 *
 * Created: 07.05.2023 18:57:42
 * Author: Ivan Prints 
 * Email: princ.va@gmail.com
 */ 


#include "gpio_driver.h"


/*
int = 2;
do {
	
	
	set_pin_level(&dev3_ld_grn, false);
	_delay_ms(100);
	set_pin_level(&dev3_ld_grn, true);
	_delay_ms(50);
} while (--i);

*/


void set_port_direction(gpio_port *GPIOx, const uint8_t mask, const uint8_t init_value){
	uint8_t *tmp_port = GPIOx->port-1;
	*tmp_port=mask;
	uint8_t *default_value = GPIOx->port;
	*default_value=~init_value;
	
}


void set_pin_dir(gpio *GPIOx, const enum port_dir direction)
{
	uint8_t *tmp_port = GPIOx->port-1;
	switch (direction) {
		case PORT_DIR_IN:
		*tmp_port &= ~(1 << GPIOx->pin);
		break;
		case PORT_DIR_OUT:
		*tmp_port |= 1 << GPIOx->pin;
		break;
		case PORT_DIR_OFF:
		*tmp_port &= ~(1 << GPIOx->pin);
		*GPIOx->port &= ~(1 << GPIOx->pin);
		break;
		default:
		break;
	}
}


void set_pin_pull_mode(gpio *GPIOx, const enum pin_pull_mode pull_mode)
{
	uint8_t *tmp_port = GPIOx->port-1;
	if (pull_mode == PORT_PULL_UP) {
		*tmp_port &= ~(1 << GPIOx->pin);
		*GPIOx->port |= 1 << GPIOx->pin;
		} else if (pull_mode == PORT_PULL_OFF) {
		*GPIOx->port &= ~(1 << GPIOx->pin);
	}
}

void set_port_level(gpio *GPIOx, const uint8_t port_value){
	*GPIOx->port=port_value;
}




void set_pin_level(gpio *GPIOx , const bool level)
{
	if (level) {
		*GPIOx->port |= 1 << GPIOx->pin;
		} else {
		*GPIOx->port &= ~(1 << GPIOx->pin);
	}
}




void toggle_pin_level(gpio *GPIOx)
{
	uint8_t *tmp_port = GPIOx->port-2;
	if (*tmp_port & (1 << GPIOx->pin)) {
		*GPIOx->port &= ~(1 << GPIOx->pin);
		} else {
		*GPIOx->port |= 1 << GPIOx->pin;
	}	
}





uint8_t get_port_level(gpio *GPIOx){
	uint8_t *tmp_port = GPIOx->port-2;
	return *tmp_port;
}

bool get_port_pin_level(gpio *GPIOx){
	uint8_t *tmp_port = GPIOx->port-2;
	return *tmp_port & (1 << GPIOx->pin);
}
