/* 
 * File:   ecu_7_Seg.h
 * Author: Adnan
 *
 * Created on February 16, 2023, 10:45 PM
 */

#ifndef ECU_7_SEG_H
#define	ECU_7_SEG_H

/* Section: Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declarations */
#define SEGMENT_PIN0    0
#define SEGMENT_PIN1    1
#define SEGMENT_PIN2    2
#define SEGMENT_PIN3    3

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef enum
{
    SEGMENT_COMMON_ANODE,
    SEGMENT_COMMON_CATHODE
}segment_type_t;

typedef struct
{
    pin_config_t segment_pins[4];
    segment_type_t segment_type;
}segment_t;

/* Section: Software Interface Declaration*/
std_ReturnType seven_segment_initialize(const segment_t *seg);
std_ReturnType seven_segment_write_number(const segment_t *seg, uint8_t number);

#endif	/* ECU_7_SEG_H */

