/* 
 * File:   ecu_motor.h
 * Author: LAP TECH
 *
 * Created on February 13, 2023, 1:33 PM
 */

#ifndef ECU_MOTOR_H
#define	ECU_MOTOR_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declarations */
#define DC_MOTOR_ON_STATUS       0x01U
#define DC_MOTOR_OFF_STATUS      0x00U

#define DC_MOTOR_PIN_1 0x00U
#define DC_MOTOR_PIN_2 0x01U

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */

typedef struct
{
    pin_config_t dc_motor_pins[2];
}dc_motor_t;

/* Section: Software Interface Declaration*/
std_ReturnType dc_motor_initialize(const dc_motor_t *dc_motor);
std_ReturnType dc_motor_move_right(const dc_motor_t *dc_motor);
std_ReturnType dc_motor_move_left(const dc_motor_t *dc_motor);
std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor);

#endif	/* ECU_MOTOR_H */

