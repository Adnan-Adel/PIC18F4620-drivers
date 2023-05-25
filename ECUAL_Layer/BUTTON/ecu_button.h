/* 
 * File:   ecu_button.h
 * Author: Adnan
 *
 * Created on February 11, 2023, 1:21 PM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declarations */


/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef enum
{
    BUTTON_PRESSED= 0,
    BUTTON_RELEASED
}button_state_t;

typedef enum
{
    BUTTON_ACTIVE_HIGH= 0,        // Pull Down
    BUTTON_ACTIVE_LOW             // Pull Up
}button_connection_state_t;

typedef struct
{
    pin_config_t button_pin;
    button_state_t button_state;
    button_connection_state_t button_connection_state;
}button_t;

/* Section: Software Interface Declaration*/
std_ReturnType button_initialize(const button_t *btn);

std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state);

#endif	/* ECU_BUTTON_H */

