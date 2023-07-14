/* 
 * File:   timer0.c
 * Author: Adnan
 *
 * Created on July 11, 2023, 5:44 PM
 */
#include"timer0.h"

#ifdef TIMER0_INTERRUPT_FEATURE_ENABLE
    static void (*TMR0_InterruptHandler)(void) = NULL;
#endif
    
static uint16_t timer0_preload = 0;

static inline void Timer0_Prescaler_Config(const timer0_t *_timer);
static inline void Timer0_Mode_Select(const timer0_t *_timer);
static inline void Timer0_Register_Size_Config(const timer0_t *_timer);

std_ReturnType Timer0_Init(const timer0_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the TMR0 */
        TIMER0_MODULE_DISABLE();
        
        /* Configure Prescaler */
        Timer0_Prescaler_Config(_timer);
        
        /* Configure Mode */
        Timer0_Mode_Select(_timer);
        
        /* Configure Reg-Size */
        Timer0_Register_Size_Config(_timer);

        /* Configure the interrupt */
#ifdef TIMER0_INTERRUPT_FEATURE_ENABLE
        TIMER0_InterruptEnable();
        TIMER0_InterruptFlagClear();
        TMR0_InterruptHandler = _timer->TMR0_InterruptHandler;

/* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            TIMER0_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            TIMER0_LowPrioritySet();
        }
        else{ /* Nothing */ }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
#endif
        
        TMR0H = (_timer->timer0_preload_value) >> 8;
        TMR0L = (uint8_t)(_timer->timer0_preload_value);
        timer0_preload = _timer->timer0_preload_value;
        
        TIMER0_MODULE_ENABLE();
    }
    return ret_val;
}

std_ReturnType Timer0_DeInit(const timer0_t *_timer)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable TMR0 */
        TIMER0_MODULE_DISABLE();
        
        /* Disable Interrupt */
        #ifdef TIMER0_INTERRUPT_FEATURE_ENABLE   
            TIMER0_InterruptDisable();
        #endif
        
    }
    return ret_val;
}

std_ReturnType Timer0_Write_Value(const timer0_t *_timer, uint16_t value)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _timer)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        TMR0H = (value) >> 8;
        TMR0L = (uint8_t)(value);
    }
    return ret_val;
}

std_ReturnType Timer0_Read_Value(const timer0_t *_timer, uint16_t *value)
{
    std_ReturnType ret_val= E_OK;
    uint8_t l_tmr0l= 0, l_tmr0h= 0;
    if((NULL == _timer) || (NULL == value))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        l_tmr0l= TMR0L;
        l_tmr0h= TMR0H;
        *value= (uint16_t)(l_tmr0l + (l_tmr0h << 8));
    }
    return ret_val;
}

void TMR0_ISR(void)
{
    TIMER0_InterruptFlagClear();
    TMR0H = (timer0_preload) >> 8;
    TMR0L = (uint8_t)(timer0_preload);
    if(TMR0_InterruptHandler)
    {
        TMR0_InterruptHandler();
    }
}

/* ----------------------------------------- Helper Functions Implementation ----------------------------------------- */
static inline void Timer0_Prescaler_Config(const timer0_t *_timer)
{
    if(TIMER0_PRESCALER_ENABLE_CFG == _timer->timer0_prescaler_enable)
    {
        TIMER0_PRESCALER_ENABLE();
        T0CONbits.T0PS= _timer->prescaler_value;
    }
    else if(TIMER0_PRESCALER_DISABLE_CFG == _timer->timer0_prescaler_enable)
    {
        TIMER0_PRESCALER_DISABLE();
    }
}

static inline void Timer0_Mode_Select(const timer0_t *_timer)
{
    if(TIMER0_TIMER_MODE == _timer->timer0_mode)
    {
        TIMER0_TIMER_MODE_ENABLE();
    }
    else if(TIMER0_COUNTER_MODE == _timer->timer0_mode)
    {
        TIMER0_COUNTER_MODE_ENABLE();
        if(TIMER0_COUNTER_FALLING_EDGE_CFG == _timer->timer0_counter_edge)
        {
            TIMER0_FALLING_EDGE_ENABLE();
        }
        else if(TIMER0_COUNTER_RISING_EDGE_CFG == _timer->timer0_counter_edge)
        {
            TIMER0_RISING_EDGE_ENABLE();
        }
    }
}

static inline void Timer0_Register_Size_Config(const timer0_t *_timer)
{
    if(TIMER0_8BIT_REG_MODE == _timer->timer0_register_size){
        TIMER0_8BIT_REG_MODE_ENABLE();
    }
    else if(TIMER0_16BIT_REG_MODE == _timer->timer0_register_size){
        TIMER0_16BIT_REG_MODE_ENABLE();
    }
}