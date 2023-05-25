/* 
 * File:   mcal_external_interrupt.h
 * Author: Adnan
 *
 * Created on May 7, 2023, 5:19 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/* Section: Includes */
#include "mcal_interrupt_config.h"

/* Section: Macro Declarations */
#ifdef EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE

/* This routine clears the interrupt enable for the external interrupt, INT0 */
#define EXT_INT0_InterruptDisable()                  (INTCONbits.INT0IE = 0)
/* This routine sets the interrupt enable for the external interrupt, INT0 */
#define EXT_INT0_InterruptEnable()                   (INTCONbits.INT0IE = 1)
/* This routine clears the interrupt flag for the external interrupt, INT0 */
#define EXT_INT0_InterruptFlagClear()                (INTCONbits.INT0IF = 0)
/* This routine sets the edge detect of the external interrupt to negative edge */
#define EXTERNAL_IN0_RisingEdgeSet()                 (INTCON2bits.INTEDG0 = 1)
/* This routine sets the edge detect of the external interrupt to positive edge */
#define EXTERNAL_IN0_FallingEdgeSet()                (INTCON2bits.INTEDG0 = 0)

/* This routine clears the interrupt enable for the external interrupt, INT1 */
#define EXT_INT1_InterruptDisable()                  (INTCON3bits.INT1IE = 0)
/* This routine sets the interrupt enable for the external interrupt, INT1 */
#define EXT_INT1_InterruptEnable()                   (INTCON3bits.INT1IE = 1)
/* This routine clears the interrupt flag for the external interrupt, INT1 */
#define EXT_INT1_InterruptFlagClear()                (INTCON3bits.INT1IF = 0)
/* This routine sets the edge detect of the external interrupt to negative edge */
#define EXTERNAL_IN1_RisingEdgeSet()                 (INTCON2bits.INTEDG1 = 1)
/* This routine sets the edge detect of the external interrupt to positive edge */
#define EXTERNAL_IN1_FallingEdgeSet()                (INTCON2bits.INTEDG1 = 0)

/* This routine clears the interrupt enable for the external interrupt, INT2 */
#define EXT_INT2_InterruptDisable()                  (INTCON3bits.INT2IE = 0)
/* This routine sets the interrupt enable for the external interrupt, INT2 */
#define EXT_INT2_InterruptEnable()                   (INTCON3bits.INT2IE = 1)
/* This routine clears the interrupt flag for the external interrupt, INT2 */
#define EXT_INT2_InterruptFlagClear()                (INTCON3bits.INT2IF = 0)
/* This routine sets the edge detect of the external interrupt to negative edge */
#define EXTERNAL_IN2_RisingEdgeSet()                 (INTCON2bits.INTEDG2 = 1)
/* This routine sets the edge detect of the external interrupt to positive edge */
#define EXTERNAL_IN2_FallingEdgeSet()                (INTCON2bits.INTEDG2 = 0)

#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
/* This routine set the INT1 External Interrupt priority to be High priority */
#define EXT_INT1_HighPrioritySet()                   (INTCON3bits.INT1IP = 1)
/* This routine set the INT1 External Interrupt priority to be Low priority */
#define EXT_INT1_LowPrioritySet()                    (INTCON3bits.INT1IP = 0)

/* This routine set the INT2 External Interrupt priority to be High priority */
#define EXT_INT2_HighPrioritySet()                   (INTCON3bits.INT2IP = 1)
/* This routine set the INT2 External Interrupt priority to be Low priority */
#define EXT_INT2_LowPrioritySet()                    (INTCON3bits.INT2IP = 0)
#endif

#endif

#ifdef EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE

/* This routine clears the interrupt enable for the external interrupt, RBx */
#define EXT_RBx_InterruptDisable()                   (INTCONbits.RBIE = 0)
/* This routine sets the interrupt enable for the external interrupt, RBx */
#define EXT_RBx_InterruptEnable()                    (INTCONbits.RBIE = 1)
/* This routine clears the interrupt flag for the External interrupt, RBx */
#define EXT_RBx_InterruptFlagClear()                 (INTCONbits.RBIF = 0)

#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
/* This routine sets the RBx External Interrupt Priority to be High Priority */
#define EXT_RBx_Priority_High()                      (INTCON2bits.RBIP = 1)
/* This routine sets the RBx External Interrupt Priority to be Low Priority */
#define EXT_RBx_Priority_Low()                       (INTCON2bits.RBIP = 0)
#endif

#endif

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */
typedef enum
{
    INTERRUPT_FALLING_EDGE= 0,
    INTERRUPT_RISING_EDGE
}Interrupt_INTx_edge;

typedef enum
{
    INTERRUPT_EXTERNAL_INT0= 0,
    INTERRUPT_EXTERNAL_INT1,
    INTERRUPT_EXTERNAL_INT2
}Interrupt_INTx_src;

typedef struct
{
    void (* EXT_InterruptHandler) (void);
    pin_config_t mcu_pin;
    Interrupt_INTx_edge edge;
    Interrupt_INTx_src source;
    Interrupt_priority_cfg priority;
}Interrupt_INTx_t;

typedef struct
{
    void (* EXT_InterruptHandler_HIGH) (void);
    void (* EXT_InterruptHandler_LOW) (void);
    pin_config_t mcu_pin;
    Interrupt_priority_cfg priority;
}Interrupt_RBx_t;

/* Section: Software Interface Declaration*/

std_ReturnType Interrupt_INTx_Init(const Interrupt_INTx_t *int_obj);
std_ReturnType Interrupt_INTx_DeInit(const Interrupt_INTx_t *int_obj);

std_ReturnType Interrupt_RBx_Init(const Interrupt_RBx_t *int_obj);
std_ReturnType Interrupt_RBx_DeInit(const Interrupt_RBx_t *int_obj);

#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

