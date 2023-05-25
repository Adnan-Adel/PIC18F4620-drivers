/* 
 * File:   mcal_interrupt_config.h
 * Author: Adnan
 *
 * Created on May 7, 2023, 5:20 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H

/* Section: Includes */
#include "../GPIO/hal_gpio.h"
#include "mcal_interrupt_gen_cfg.h"

/* Section: Macro Declarations */
#define INTERRUPT_ENABLE                     1
#define INTERRUPT_DISABLE                    0
#define INTERRUPT_OCCUR                      1
#define INTERRUPT_NOT_OCCUR                  0
#define INTERRUPT_PRIORITY_ENABLE            1
#define INTERRUPT_PRIORITY_DISABLE           0

/* Section: Macro Functions Declarations */
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
/* This macro will enable priority levels on interrupts. */
#define INTERRUPT_PriorityLevelsEnable()         (RCONbits.IPEN = 1)

/* This macro will disable priority levels on interrupts. */
#define INTERRUPT_PriorityLevelsDisable()        (RCONbits.IPEN = 0)

// We will use those comming 4 Functions like Macros only if We Enabled priority levels
/* This macro will enable high priority global interrupt. */
#define INTERRUPT_GlobalInterruptHighEnable()    (INTCONbits.GIEH = 1)

/* This macro will disable high priority global interrupt. */
#define INTERRUPT_GlobalInterruptHighDisable()   (INTCONbits.GIEH = 0)

/* This macro will enable low priority global interrupt. */
#define INTERRUPT_GlobalInterruptLowEnable()     (INTCONbits.GIEL = 1)

/* This macro will disable low priority global interrupt. */
#define INTERRUPT_GlobalInterruptLowDisable()    (INTCONbits.GIEL = 0)

#else

/* This macro will enable global interrupt. */
#define INTERRUPT_GlobalInterruptEnable()    (INTCONbits.GIE = 1)

/* This macro will disable global interrupt. */
#define INTERRUPT_GlobalInterruptDisable()   (INTCONbits.GIE = 0)

/* This macro will enable peripheral interrupts. */
#define INTERRUPT_PeripheralInterruptEnable()    (INTCONbits.PEIE = 1)

/* This macro will disable peripheral interrupts. */
#define INTERRUPT_PeripheralInterruptDisable()   (INTCONbits.PEIE = 0)

#endif

/* Section: Data Type Declarations */
typedef enum
{
    INTERRUPT_LOW_PRIORITY= 0,
    INTERRUPT_HIGH_PRIORITY
}Interrupt_priority_cfg;

/* Section: Software Interface Declaration*/


#endif	/* MCAL_INTERRUPT_CONFIG_H */

