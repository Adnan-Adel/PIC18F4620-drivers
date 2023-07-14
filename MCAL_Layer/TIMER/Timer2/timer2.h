/* 
 * File:   timer2.h
 * Author: Adnan
 *
 * Created on July 13, 2023, 7:38 PM
 */

#ifndef TIMER2_H
#define	TIMER2_H

/* ------------------------------------ Includes ------------------------------------ */
#include "../../GPIO/hal_gpio.h"
#include "../../Interrupt/mcal_internal_interrupt.h"

/* ------------------------------------ Macro Declarations ------------------------------------ */

/* Timer2 Input Clock Pre-scaler */
#define TIMER2_PRESCALER_DIV_BY_1           0
#define TIMER2_PRESCALER_DIV_BY_4           1
#define TIMER2_PRESCALER_DIV_BY_16          2

/* Timer2 Input Clock Post-scaler */
#define TIMER2_POSTSCALER_DIV_BY_1          0
#define TIMER2_POSTSCALER_DIV_BY_2          1
#define TIMER2_POSTSCALER_DIV_BY_3          2
#define TIMER2_POSTSCALER_DIV_BY_4          3
#define TIMER2_POSTSCALER_DIV_BY_5          4
#define TIMER2_POSTSCALER_DIV_BY_6          5
#define TIMER2_POSTSCALER_DIV_BY_7          6
#define TIMER2_POSTSCALER_DIV_BY_8          7
#define TIMER2_POSTSCALER_DIV_BY_9          8
#define TIMER2_POSTSCALER_DIV_BY_10         9
#define TIMER2_POSTSCALER_DIV_BY_11         10
#define TIMER2_POSTSCALER_DIV_BY_12         11
#define TIMER2_POSTSCALER_DIV_BY_13         12
#define TIMER2_POSTSCALER_DIV_BY_14         13
#define TIMER2_POSTSCALER_DIV_BY_15         14
#define TIMER2_POSTSCALER_DIV_BY_16         15

/* ------------------------------------ Macro Functions Declarations ------------------------------------ */

/* Enable or Disable Timer2 Module */
#define TIMER2_MODULE_ENABLE()                (T2CONbits.TMR2ON= 1)
#define TIMER2_MODULE_DISABLE()               (T2CONbits.TMR2ON= 0)

/* Timer2 Input Clock Pre-scaler */
#define TIMER2_PRESCALER_SELECT(_PRESCALER)   (T2CONbits.T2CKPS= _PRESCALER)

/* Timer2 Input Clock Post-scaler */
#define TIMER2_POSTSCALER_SELECT(_POSTSCALER) (T2CONbits.TOUTPS= _POSTSCALER)

/* ------------------------------------ Data Type Declarations ------------------------------------ */
typedef struct
{
#ifdef TIMER2_INTERRUPT_FEATURE_ENABLE
    void (* TMR2_InterruptHandler)(void);
    Interrupt_priority_cfg priority;
#endif
    uint8_t timer2_preload_value;
    uint8_t timer2_postscaler_value;
    uint8_t timer2_prescaler_value;
    
}timer2_t;

/* ------------------------------------ Software Interface Declaration ------------------------------------ */
std_ReturnType Timer2_Init(const timer2_t *_timer);
std_ReturnType Timer2_DeInit(const timer2_t *_timer);
std_ReturnType Timer2_Write_Value(const timer2_t *_timer, uint8_t value);
std_ReturnType Timer2_Read_Value(const timer2_t *_timer, uint8_t *value);

#endif	/* TIMER2_H */

