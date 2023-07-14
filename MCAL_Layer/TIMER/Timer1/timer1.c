/* 
 * File:   timer1.c
 * Author: Adnan
 *
 * Created on July 13, 2023, 1:24 AM
 */
#include"timer1.h"

#ifdef TIMER1_INTERRUPT_FEATURE_ENABLE
    static void (*TMR1_InterruptHandler)(void) = NULL;
#endif
    
static uint16_t timer1_preload = 0;

static inline void Timer1_Mode_Select(const timer1_t *_timer);

std_ReturnType Timer1_Init(const timer1_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the TMR1 */
        TIMER1_MODULE_DISABLE();
        
        /* Configure Pre-scaler */
        TIMER1_PRESCALER_SELECT(_timer->timer1_prescaler_value);
        /* Configure Mode */
        Timer1_Mode_Select(_timer);
        
        /* Initialization Value */
        TMR1H = (_timer->timer1_preload_value) >> 8;
        TMR1L = (uint8_t)(_timer->timer1_preload_value);
        timer1_preload = _timer->timer1_preload_value;

        /* Configure the interrupt */
#ifdef TIMER1_INTERRUPT_FEATURE_ENABLE
        TIMER1_InterruptEnable();
        TIMER1_InterruptFlagClear();
        TMR1_InterruptHandler = _timer->TMR1_InterruptHandler;

/* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER1_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER1_LowPrioritySet();
        }
        else{ /* Nothing */ }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
#endif
        

        TIMER1_MODULE_ENABLE();
    }
    return ret_val;
}

std_ReturnType Timer1_DeInit(const timer1_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable TMR1 */
        TIMER1_MODULE_DISABLE();
        
        /* Disable Interrupt */
        #ifdef   TIMER1_INTERRUPT_FEATURE_ENABLE
            TIMER1_InterruptDisable();
        #endif
        
    }
    return ret_val;
}

std_ReturnType Timer1_Write_Value(const timer1_t *_timer, uint16_t value)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        TMR1H = (value) >> 8;
        TMR1L = (uint8_t)(value);
    }
    return ret_val;
}

std_ReturnType Timer1_Read_Value(const timer1_t *_timer, uint16_t *value)
{
    std_ReturnType ret_val= E_OK;
    uint8_t l_tmr1_low= 0, l_tmr1_high= 0;
    if((NULL == _timer) || (NULL == value))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        l_tmr1_low= TMR1L;
        l_tmr1_high= TMR1H;
        *value= (uint16_t)(l_tmr1_low + (l_tmr1_high << 8));
    }
    return ret_val;
}

static inline void Timer1_Mode_Select(const timer1_t *_timer)
{
    if(TIMER1_TIMER_MODE == _timer->timer1_mode)
    {
        TIMER1_TIMER_MODE_ENABLE();
    }
    else if(TIMER1_COUNTER_MODE == _timer->timer1_mode)
    {
        TIMER1_COUNTER_MODE_ENABLE();
        if(TIMER1_ASYNC_COUNTER_MODE == _timer->timer1_counter_mode)
        {
            TIMER1_ASYNC_COUNTER_MODE_ENABLE();
        }
        else if(TIMER1_SYNC_COUNTER_MODE == _timer->timer1_counter_mode)
        {
            TIMER1_SYNC_COUNTER_MODE_ENABLE();
        }
    }
}

void TMR1_ISR(void)
{
    TIMER1_InterruptFlagClear();
    TMR1H = (timer1_preload) >> 8;
    TMR1L = (uint8_t)(timer1_preload);
    if(TMR1_InterruptHandler)
    {
        TMR1_InterruptHandler();
    }
}
