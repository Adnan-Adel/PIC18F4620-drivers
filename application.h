/* 
 * File:   application.h
 * Author: Adnan
 *
 * Created on February 4, 2023, 2:19 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

#include "ECUAL_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/EEPROM/eeprom.h"
#include "MCAL_Layer/ADC/adc.h"
#include "MCAL_Layer/TIMER/Timer0/timer0.h"
#include "MCAL_Layer/TIMER/Timer1/timer1.h"
#include "MCAL_Layer/TIMER/Timer2/timer2.h"
#include "MCAL_Layer/TIMER/Timer3/timer3.h"
#include "MCAL_Layer/USART/usart.h"
#include "MCAL_Layer/SPI/spi.h"
#include "MCAL_Layer/I2C/I2C.h"

/* Data Type Declaration */
extern keypad_t keypad_1;
extern led_t led1;
extern led_t led2;
extern led_t led3;
extern button_t btn;
extern chr_lcd_4bit_t lcd_4bit_obj;
extern chr_lcd_8bit_t lcd_8bit_obj;

/* Functions Declaration */
void application_initialize(void);



#endif	/* APPLICATION_H */

