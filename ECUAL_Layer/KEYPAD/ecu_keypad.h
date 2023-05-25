/* 
 * File:   ecu_keypad.h
 * Author: Adnan
 *
 * Created on March 2, 2023, 4:48 PM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declarations */
#define KEYPAD_ROWS         4
#define KEYPAD_COLUMNS      4

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef struct
{
    pin_config_t keypad_rows_pins[KEYPAD_ROWS];
    pin_config_t keypad_columns_pins[KEYPAD_COLUMNS];
}keypad_t;

/* Section: Software Interface Declaration*/
std_ReturnType keypad_initialize(const keypad_t *keypad_obj);
std_ReturnType keypad_get_value(const keypad_t *keypad_obj, uint8_t *value);

#endif	/* ECU_KEYPAD_H */

