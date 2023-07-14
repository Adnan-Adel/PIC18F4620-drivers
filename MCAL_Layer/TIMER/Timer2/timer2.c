/* 
 * File:   timer2.c
 * Author: Adnan
 *
 * Created on July 13, 2023, 7:38 PM
 */
#include "timer2.h"

#ifdef TIMER2_INTERRUPT_FEATURE_ENABLE
    static void (*TMR2_InterruptHandler)(void) = NULL;
#endif
    
static uint8_t timer2_preload = 0;

std_ReturnType Timer2_Init(const timer2_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the TMR2 */
        TIMER2_MODULE_DISABLE();
        
        /* Configure Pre-scaler */
        TIMER2_PRESCALER_SELECT(_timer->timer2_prescaler_value);
        
        /* Configure Post-scaler */
        TIMER2_POSTSCALER_SELECT(_timer->timer2_postscaler_value);
        
        /* Initialization Value */
        TMR2 = _timer->timer2_preload_value;
        timer2_preload = _timer->timer2_preload_value;

        /* Configure the interrupt */
#ifdef TIMER2_INTERRUPT_FEATURE_ENABLE
        TIMER2_InterruptEnable();
        TIMER2_InterruptFlagClear();
        TMR2_InterruptHandler = _timer->TMR2_InterruptHandler;

/* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER2_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER2_LowPrioritySet();
        }
        else{ /* Nothing */ }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
#endif
        

        TIMER2_MODULE_ENABLE();
    }
    return ret_val;
}

std_ReturnType Timer2_DeInit(const timer2_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable TMR2 */
        TIMER2_MODULE_DISABLE();
        
        /* Disable Interrupt */
        #ifdef   TIMER2_INTERRUPT_FEATURE_ENABLE
            TIMER2_InterruptDisable();
        #endif
        
    }
    return ret_val;
}

std_ReturnType Timer2_Write_Value(const timer2_t *_timer, uint8_t value)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        TMR2 = value;
    }
    return ret_val;
}

std_ReturnType Timer2_Read_Value(const timer2_t *_timer, uint8_t *value)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _timer) || (NULL == value))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *value= TMR2;
    }
    return ret_val;
}

void TMR2_ISR(void)
{
    TIMER2_InterruptFlagClear();
    TMR2= timer2_preload;
    if(TMR2_InterruptHandler)
    {
        TMR2_InterruptHandler();
    }
}