/* 
 * File:   timer3.h
 * Author: Adnan
 *
 * Created on July 13, 2023, 9:30 PM
 */

#ifndef TIMER3_H
#define	TIMER3_H

/* ------------------------------------ Includes ------------------------------------ */
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/mcal_internal_interrupt.h"

/* ------------------------------------ Macro Declarations ------------------------------------ */

/* 16-bit Read/Write Mode */
#define TIMER3_RW_REG_8BIT_MODE             0
#define TIMER3_RW_REG_16BIT_MODE            1

/* Timer3 Input Clock Pre-scaler */
#define TIMER3_PRESCALER_DIV_BY_1           0
#define TIMER3_PRESCALER_DIV_BY_2           1
#define TIMER3_PRESCALER_DIV_BY_4           2
#define TIMER3_PRESCALER_DIV_BY_8           3

/* Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE           1
#define TIMER3_SYNC_COUNTER_MODE            0

/* Timer3 Clock Source */
#define TIMER3_COUNTER_MODE                 1     // External clock input from Timer1 oscillator or T13CKI (on the rising edge after the first falling edge)
#define TIMER3_TIMER_MODE                   0     //  Internal clock (FOSC/4)


/* ------------------------------------ Macro Functions Declarations ------------------------------------ */

/* Enable or Disable Timer2 Module */
#define TIMER3_MODULE_ENABLE()                (T3CONbits.TMR3ON= 1)
#define TIMER3_MODULE_DISABLE()               (T3CONbits.TMR3ON= 0)

/* 16-Bit Read/Write Mode */
#define TIMER3_RW_REG_8BIT_MODE_ENABLE        (T3CONbits.RD16= 0)
#define TIMER3_RW_REG_16BIT_MODE_ENABLE       (T3CONbits.RD16= 1)

/*  Timer3 External Clock Input Synchronization */
#define TIMER3_ASYNC_COUNTER_MODE_ENABLE()    (T3CONbits.T3SYNC= 1)
#define TIMER3_SYNC_COUNTER_MODE_ENABLE()     (T3CONbits.T3SYNC= 0)

/*  Timer3 Clock Source (Timer or Counter) */
#define TIMER3_TIMER_MODE_ENABLE()            (T3CONbits.TMR3CS= 0)
#define TIMER3_COUNTER_MODE_ENABLE()          (T3CONbits.TMR3CS= 1)

/* Timer3 Input Clock Pre-scaler */
#define TIMER3_PRESCALER_SELECT(_PRESCALER)   (T3CONbits.T3CKPS= _PRESCALER)

/* ------------------------------------ Data Type Declarations ------------------------------------ */
typedef struct
{
#ifdef TIMER3_INTERRUPT_FEATURE_ENABLE
    void (* TMR3_InterruptHandler)(void);
    Interrupt_priority_cfg priority;
#endif
    uint16_t timer3_preload_value;
    uint8_t timer3_prescaler_value: 2;
    uint8_t timer3_mode : 1;
    uint8_t timer3_counter_mode : 1;
    uint8_t timer3_reg_wr_mode : 1;
    uint8_t timer1_reserved : 3; 
}timer3_t;

/* ------------------------------------ Software Interface Declaration ------------------------------------ */
std_ReturnType Timer3_Init(const timer3_t *_timer);
std_ReturnType Timer3_DeInit(const timer3_t *_timer);
std_ReturnType Timer3_Write_Value(const timer3_t *_timer, uint16_t value);
std_ReturnType Timer3_Read_Value(const timer3_t *_timer, uint16_t *value);

#endif	/* TIMER3_H */

