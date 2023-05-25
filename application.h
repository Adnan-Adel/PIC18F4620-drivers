/* 
 * File:   application.h
 * Author: LAP TECH
 *
 * Created on February 4, 2023, 2:19 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

#include "ECUAL_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"

/* Data Type Declaration */
extern keypad_t keypad_1;
extern led_t led1;
extern chr_lcd_4bit_t lcd_4bit_obj;
extern chr_lcd_8bit_t lcd_8bit_obj;

/* Functions Declaration */
void application_initialize(void);



#endif	/* APPLICATION_H */

