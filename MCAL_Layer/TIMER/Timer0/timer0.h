/* 
 * File:   timer0.h
 * Author: Adnan
 *
 * Created on July 11, 2023, 5:44 PM
 */

#ifndef TIMER0_H
#define	TIMER0_H

/* Section: Includes */
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */
#define TIMER0_8BIT_REG_MODE                1
#define TIMER0_16BIT_REG_MODE               0

#define TIMER0_COUNTER_MODE                 1       // Transition on T0CKI pin
#define TIMER0_TIMER_MODE                   0       // Internal instruction cycle clock (CLKOUT)

#define TIMER0_COUNTER_FALLING_EDGE_CFG     0
#define TIMER0_COUNTER_RISING_EDGE_CFG      1

#define TIMER0_PRESCALER_ENABLE_CFG         0
#define TIMER0_PRESCALER_DISABLE_CFG        1


/* Section: Macro Functions Declarations */
#define TIMER0_8BIT_REG_MODE_ENABLE()       (T0CONbits.T08BIT= 1)
#define TIMER0_16BIT_REG_MODE_ENABLE()      (T0CONbits.T08BIT= 0)

#define TIMER0_COUNTER_MODE_ENABLE()        (T0CONbits.T0CS= 1)
#define TIMER0_TIMER_MODE_ENABLE()          (T0CONbits.T0CS= 0)

#define TIMER0_FALLING_EDGE_ENABLE()        (T0CONbits.T0SE= 1)
#define TIMER0_RISING_EDGE_ENABLE()         (T0CONbits.T0SE= 0)

#define TIMER0_PRESCALER_DISABLE()          (T0CONbits.PSA= 1)
#define TIMER0_PRESCALER_ENABLE()           (T0CONbits.PSA= 0)

#define TIMER0_MODULE_ENABLE()                (T0CONbits.TMR0ON= 1)
#define TIMER0_MODULE_DISABLE()               (T0CONbits.TMR0ON= 0)

/* Section: Data Type Declarations */
/*
 * Timer0 Pre-Scaler Select
 */
typedef enum
{
    TIMER0_PRESCALER_DIV_BY_2= 0,
    TIMER0_PRESCALER_DIV_BY_4,
    TIMER0_PRESCALER_DIV_BY_8,
    TIMER0_PRESCALER_DIV_BY_16,
    TIMER0_PRESCALER_DIV_BY_32,
    TIMER0_PRESCALER_DIV_BY_64,
    TIMER0_PRESCALER_DIV_BY_128,
    TIMER0_PRESCALER_DIV_BY_256
            
}timer0_prescaler_select_t;

typedef struct
{
#ifdef TIMER0_INTERRUPT_FEATURE_ENABLE
    void(* TMR0_InterruptHandler)(void);
    Interrupt_priority_cfg priority;
#endif
    timer0_prescaler_select_t prescaler_value;
    uint16_t timer0_preload_value;
    uint8_t timer0_prescaler_enable: 1;
    uint8_t timer0_counter_edge: 1;
    uint8_t timer0_mode: 1;
    uint8_t timer0_register_size: 1;
    uint8_t timer0_reserved: 4;
    
}timer0_t;

/* Section: Software Interface Declaration*/
std_ReturnType Timer0_Init(const timer0_t *_timer);
std_ReturnType Timer0_DeInit(const timer0_t *_timer);
std_ReturnType Timer0_Write_Value(const timer0_t *_timer, uint16_t value);
std_ReturnType Timer0_Read_Value(const timer0_t *_timer, uint16_t *value);

#endif	/* TIMER0_H */

