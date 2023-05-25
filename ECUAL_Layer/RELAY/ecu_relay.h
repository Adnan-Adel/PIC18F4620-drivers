/* 
 * File:   ecu_relay.h
 * Author: Adnan
 *
 * Created on February 12, 2023, 6:12 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declarations */
#define RELAY_ON_STATUS         0x01U
#define RELAY_OFF_STATUS        0x00U

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef struct
{
    uint8_t relay_port : 4;
    uint8_t relay_pin : 3;
    uint8_t relay_status : 1;
}relay_t;

/* Section: Software Interface Declaration*/
std_ReturnType relay_initialize(const relay_t *relay);
std_ReturnType relay_turn_on(const relay_t *relay);
std_ReturnType relay_turn_off(const relay_t *relay);

#endif	/* ECU_RELAY_H */

