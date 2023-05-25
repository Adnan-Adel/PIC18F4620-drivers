/* 
 * File:   ecu_led.h
 * Author: LAP TECH
 *
 * Created on February 4, 2023, 2:17 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/* Section: Includes */

#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_led_cfg.h"

/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef enum
{
    LED_OFF= 0,
    LED_ON
}led_status;

typedef struct
{
    uint8_t port : 4;
    uint8_t pin : 3;
    uint8_t led_status : 1;
}led_t;

/* Section: Software Interface Declaration*/
std_ReturnType led_initialize(const led_t *led);

std_ReturnType led_turn_on(const led_t *led);

std_ReturnType led_turn_off(const led_t *led);

std_ReturnType led_toggle(const led_t *led);

#endif	/* ECU_LED_H */

