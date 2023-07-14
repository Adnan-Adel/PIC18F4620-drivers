/* 
 * File:   timer3.c
 * Author: Adnan
 *
 * Created on July 13, 2023, 9:30 PM
 */
#include "timer3.h"

#ifdef TIMER3_INTERRUPT_FEATURE_ENABLE
    static void (*TMR3_InterruptHandler)(void) = NULL;
#endif
    
static uint16_t timer3_preload = 0;

static inline void Timer3_Mode_Select(const timer3_t *_timer);


std_ReturnType Timer3_Init(const timer3_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the TMR3 */
        TIMER3_MODULE_DISABLE();
        
        /* Configure Pre-scaler */
        TIMER3_PRESCALER_SELECT(_timer->timer3_prescaler_value);
        /* Configure Mode */
        Timer3_Mode_Select(_timer);
        
        /* Initialization Value */
        TMR3H = (_timer->timer3_preload_value) >> 8;
        TMR3L = (uint8_t)(_timer->timer3_preload_value);
        timer3_preload = _timer->timer3_preload_value;

        /* Configure the interrupt */
#ifdef TIMER3_INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptEnable();
        TIMER3_InterruptFlagClear();
        TMR3_InterruptHandler = _timer->TMR3_InterruptHandler;

/* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER3_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER3_LowPrioritySet();
        }
        else{ /* Nothing */ }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
#endif
        

        TIMER3_MODULE_ENABLE();
    }
    return ret_val;
}

std_ReturnType Timer3_DeInit(const timer3_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable TMR3 */
        TIMER3_MODULE_DISABLE();
        
        /* Disable Interrupt */
        #ifdef   TIMER3_INTERRUPT_FEATURE_ENABLE
            TIMER3_InterruptDisable();
        #endif
        
    }
    return ret_val;
}

std_ReturnType Timer3_Write_Value(const timer3_t *_timer, uint16_t value)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        TMR3H = (value) >> 8;
        TMR3L = (uint8_t)(value);
    }
    return ret_val;
}

std_ReturnType Timer3_Read_Value(const timer3_t *_timer, uint16_t *value)
{
    std_ReturnType ret_val= E_OK;
    uint8_t l_tmr3_low= 0, l_tmr3_high= 0;
    if((NULL == _timer) || (NULL == value))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        l_tmr3_low= TMR3L;
        l_tmr3_high= TMR3H;
        *value= (uint16_t)(l_tmr3_low + (l_tmr3_high << 8));
    }
    return ret_val;
}


static inline void Timer3_Mode_Select(const timer3_t *_timer)
{
    if(TIMER3_TIMER_MODE == _timer->timer3_mode)
    {
        TIMER3_TIMER_MODE_ENABLE();
    }
    else if(TIMER3_COUNTER_MODE == _timer->timer3_mode)
    {
        TIMER3_COUNTER_MODE_ENABLE();
        if(TIMER3_ASYNC_COUNTER_MODE == _timer->timer3_counter_mode)
        {
            TIMER3_ASYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER3_SYNC_COUNTER_MODE == _timer->timer3_counter_mode)
        {
            TIMER3_SYNC_COUNTER_MODE_ENABLE();
        }
    }
}



void TMR3_ISR(void)
{
    TIMER3_InterruptFlagClear();
    TMR3H = (timer3_preload) >> 8;
    TMR3L = (uint8_t)(timer3_preload);
    if(TMR3_InterruptHandler)
    {
        TMR3_InterruptHandler();
    }
}