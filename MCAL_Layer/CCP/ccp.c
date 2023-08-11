/* 
 * File:   ccp.c
 * Author: Adnan
 *
 * Created on July 16, 2023, 12:37 AM
 */
#include "ccp.h"

#ifdef CCP1_INTERRUPT_FEATURE_ENABLE
void (* CCP1_InterruptHandler)(void)= NULL;
#endif

#ifdef CCP2_INTERRUPT_FEATURE_ENABLE
void (* CCP2_InterruptHandler)(void)= NULL;
#endif

/* ------------------------- Helper Functions Declaration ------------------------- */
static void CCP_Interrupt_Config(const ccp_t* _ccp_obj);
static void CCP_PWM_Mode_Config(const ccp_t* _ccp_obj);
static void CCP_Mode_Timer_Select(const ccp_t* _ccp_obj);
static std_ReturnType CCP_Capture_mode_Config(const ccp_t* _ccp_obj);
static std_ReturnType CCP_Compare_mode_Config(const ccp_t* _ccp_obj);

/* ------------------------------------ Software Interface Definition ------------------------------------ */
std_ReturnType CCP_Init(const ccp_t* _ccp_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _ccp_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* CCP Module Disable */
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }
        
        /* CCP Capture Mode Initialization */
        if(CCP_CAPTURE_MODE_SELECT == _ccp_obj->ccp_mode)
        {
            ret_val= CCP_Capture_mode_Config(_ccp_obj);
        }
        
        /* CCP Compare Mode Initialization */
        else if(CCP_CAPTURE_MODE_SELECT == _ccp_obj->ccp_mode)
        {
            ret_val= CCP_Compare_mode_Config(_ccp_obj);
        }

        /* CCP PWM Mode Initialization */
    #if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED))
            else if(CCP_PWM_MODE_SELECT == _ccp_obj->ccp_mode)
            {
                CCP_PWM_Mode_Config(_ccp_obj);
            }
    #endif

        /* PIN Configurations */
        ret_val = gpio_pin_initialize(&(_ccp_obj->ccp_pin));
        
        /* Interrupt Configurations for CCP1 and CCP2 Modules */
        CCP_Interrupt_Config(_ccp_obj);
        
    }

    return ret_val;
}

std_ReturnType CCP_DeInit(const ccp_t* _ccp_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _ccp_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            CCP1_SET_MODE(CCP_MODULE_DISABLE); /* CCP1 Module Disable */
        /* Interrupt Configurations */ 
        #ifdef CCP1_INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
        #endif 
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            CCP2_SET_MODE(CCP_MODULE_DISABLE); /* CCP2 Module Disable */
        #ifdef CCP2_INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
        #endif  
        }
    }

    return ret_val;
}

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED))
std_ReturnType CCP_IsCapturedDataReady(const ccp_t* _ccp_obj, uint8_t *_capture_status)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _ccp_obj) || (NULL == _capture_status))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            if(CCP1_CAPTURE_READY == PIR1bits.CCP1IF)
            {
                *_capture_status= CCP1_CAPTURE_READY;
                CCP1_InterruptFlagClear();
            }
            else
            {
                *_capture_status= CCP1_CAPTURE_NOT_READY;
            }
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            if(CCP2_CAPTURE_READY == PIR2bits.CCP2IF)
            {
                *_capture_status= CCP2_CAPTURE_READY;
                CCP2_InterruptFlagClear();
            }
            else
            {
                *_capture_status= CCP2_CAPTURE_NOT_READY;
            }
        }
    }

    return ret_val;
}
    
std_ReturnType CCP_Capture_Mode_Read_Value(const ccp_t* _ccp_obj, uint16_t *capture_value)
{
    std_ReturnType ret_val= E_OK;
    ccp_reg_t capture_temp_val= {.ccp_reg_low= 0, .ccp_reg_high= 0};
    if((NULL == _ccp_obj) || (NULL == capture_value))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            capture_temp_val.ccp_reg_low= CCPR1L;
            capture_temp_val.ccp_reg_high= CCPR1H;
            *capture_value= capture_temp_val.ccp_reg_16bit;
        }
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            capture_temp_val.ccp_reg_low= CCPR2L;
            capture_temp_val.ccp_reg_high= CCPR2H;
            *capture_value= capture_temp_val.ccp_reg_16bit;
        }
    }

    return ret_val;
}
    
#endif

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED))
    std_ReturnType CCP_IsCompareComplete(const ccp_t* _ccp_obj, uint8_t *_compare_status)
    {
        std_ReturnType ret_val= E_OK;
        if(NULL == _compare_status)
        {
            ret_val= E_NOT_OK;
        }
        else
        {
            if(CCP1_INST == _ccp_obj->ccp_inst)
            {
                if(CCP1_CAPTURE_READY == PIR1bits.CCP1IF)
                {
                    *_compare_status= CCP1_COMPARE_READY;
                    CCP1_InterruptFlagClear();
                }
                else
                {
                    *_compare_status= CCP1_COMPARE_NOT_READY;
                }
            }
            else if(CCP2_INST == _ccp_obj->ccp_inst)
            {
                if(CCP2_CAPTURE_READY == PIR2bits.CCP2IF)
                {
                    *_compare_status= CCP2_COMPARE_READY;
                    CCP2_InterruptFlagClear();
                }
                else
                {
                    *_compare_status= CCP2_COMPARE_NOT_READY;
                }
            }
        }

        return ret_val;
    }
    
    std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16_t compare_value)
    {
        std_ReturnType ret_val= E_OK;
        ccp_reg_t compare_temp_val= {.ccp_reg_low= 0, .ccp_reg_high= 0};
        if(NULL == _ccp_obj)
        {
            ret_val= E_NOT_OK;
        }
        else
        {
            compare_temp_val.ccp_reg_16bit= compare_value;
            if(CCP1_INST == _ccp_obj->ccp_inst)
            {
                CCPR1L= compare_temp_val.ccp_reg_low;
                CCPR1H= compare_temp_val.ccp_reg_high;
            }
            else if(CCP2_INST == _ccp_obj->ccp_inst)
            {
                CCPR2L= compare_temp_val.ccp_reg_low;
                CCPR2H= compare_temp_val.ccp_reg_high;
            }
        }
        return ret_val;
    }
    
#endif

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED))
    std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8_t _duty)
    {
        std_ReturnType ret_val= E_OK;
        uint16_t l_temp_duty= 0;
        if(NULL == _ccp_obj)
        {
            ret_val= E_NOT_OK;
        }
        else
        {
            l_temp_duty= (uint16_t)((float)4 * ((float)PR2 + 1.0) * ((float)_duty / 100.0));
            if(CCP1_INST == _ccp_obj->ccp_inst)
            {
                CCP1CONbits.DC1B = (uint8_t)(l_temp_duty & 0x03);
                CCPR1L = (uint8_t)(l_temp_duty >> 2);
            }
            else if(CCP2_INST == _ccp_obj->ccp_inst)
            {
                CCP2CONbits.DC2B = (uint8_t)(l_temp_duty & 0x03);
                CCPR2L = (uint8_t)(l_temp_duty >> 2);
            }
        }

        return ret_val;
    }
    
    std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj)
    {
        std_ReturnType ret_val= E_OK;
        if(NULL == _ccp_obj)
        {
            ret_val= E_NOT_OK;
        }
        else
        {
            if(CCP1_INST == _ccp_obj->ccp_inst)
            {
                CCP1CONbits.CCP1M = CCP_PWM_MODE;
            }
            else if(CCP2_INST == _ccp_obj->ccp_inst)
            {
                CCP2CONbits.CCP2M = CCP_PWM_MODE;
            }
        }

        return ret_val;
    }
    
    std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj)
    {
        std_ReturnType ret_val= E_OK;
        if(NULL == _ccp_obj)
        {
            ret_val= E_NOT_OK;
        }
        else
        {
            if(CCP1_INST == _ccp_obj->ccp_inst)
            {
                CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
            }
            else if(CCP2_INST == _ccp_obj->ccp_inst)
            {
                CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
            }
        }

        return ret_val;
    }
    
#endif
    
    
/* ------------------------- Helper Functions Definition ------------------------- */
static void CCP_Interrupt_Config(const ccp_t* _ccp_obj)
{
/* ----------------- CCP1 Interrupt Configurations ----------------- */
    #ifdef CCP1_INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptEnable();
            CCP1_InterruptFlagClear();
            CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;

    /* Interrupt Priority Configurations */
    #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if(INTERRUPT_HIGH_PRIORITY == _timer->ccp1_priority){
                /* Enables all high-priority interrupts */
                INTERRUPT_GlobalInterruptHighEnable();
                CCP1_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == _timer->ccp1_priority){
                /* Enables all unmasked peripheral interrupts */
                INTERRUPT_GlobalInterruptLowEnable();
                CCP1_LowPrioritySet();
            }
            else{ /* Nothing */ }
    #else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
    #endif

    #endif

/* ----------------- CCP2 Interrupt Configurations ----------------- */
    #ifdef CCP2_INTERRUPT_FEATURE_ENABLE
        CCP2_InterruptEnable();
        CCP2_InterruptFlagClear();
        CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;

    /* Interrupt Priority Configurations */
    #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
            INTERRUPT_PriorityLevelsEnable();
            if(INTERRUPT_HIGH_PRIORITY == _timer->ccp2_priority){
                /* Enables all high-priority interrupts */
                INTERRUPT_GlobalInterruptHighEnable();
                CCP2_HighPrioritySet();
            }
            else if(INTERRUPT_LOW_PRIORITY == _timer->ccp2_priority){
                /* Enables all unmasked peripheral interrupts */
                INTERRUPT_GlobalInterruptLowEnable();
                CCP2_LowPrioritySet();
            }
            else{ /* Nothing */ }
    #else
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
    #endif

    #endif
}

#if (CCP1_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE==CCP_CFG_PWM_MODE_SELECTED)
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj){
    /* PWM Frequency Initialization */
    PR2 = (uint8_t)(((float)_XTAL_FREQ / ((float)_ccp_obj->ccp_pwm_freq * 4.0 * (float)_ccp_obj->TMR2_prescaler_value * 
                    (float)_ccp_obj->TMR2_postscaler_value)) - 1);
    
    if(CCP1_INST == _ccp_obj->ccp_inst){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP1_SET_MODE(CCP_PWM_MODE);
        }
        else {/* Nothing */}
    }
    else if(CCP2_INST == _ccp_obj->ccp_inst){
        if(CCP_PWM_MODE == _ccp_obj->ccp_mode_variant){
            CCP2_SET_MODE(CCP_PWM_MODE);
        }
        else {/* Nothing */}
    }
    else{ /* Nothing */ }
}
#endif

static void CCP_Mode_Timer_Select(const ccp_t* _ccp_obj)
{
    if(CCP1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0; 
        T3CONbits.T3CCP2 = 1;
    }
    else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->ccp_capture_timer){
        /* Timer3 is the capture/compare clock source for CCP2 */
        /* Timer1 is the capture/compare clock source for CCP1 */
        T3CONbits.T3CCP1 = 1;
        T3CONbits.T3CCP2 = 0;
    }
    else if(CCP1_CCP2_TIMER1 == _ccp_obj->ccp_capture_timer){
        /* Timer1 is the capture/compare clock source for the CCP modules */
        T3CONbits.T3CCP1 = 0;
        T3CONbits.T3CCP2 = 0;
    }
}

static std_ReturnType CCP_Capture_mode_Config(const ccp_t* _ccp_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _ccp_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_1_RISING_EDGE:
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_4_RISING_EDGE:
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_16_RISING_EDGE:
                    CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                    break;
            }
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_1_RISING_EDGE:
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_4_RISING_EDGE:
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);
                    break;
                    
                case CCP_CAPTURE_MODE_16_RISING_EDGE:
                    CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);
                    break;
            }
        }
        CCP_Mode_Timer_Select(_ccp_obj);
    }

    return ret_val;
}

static std_ReturnType CCP_Compare_mode_Config(const ccp_t* _ccp_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _ccp_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(CCP1_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
                    CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                    break;
                    
                case CCP_COMPARE_MODE_INIT_PIN_LOW:
                    CCP1_SET_MODE(CCP_COMPARE_MODE_INIT_PIN_LOW);
                    break;
                    
                case CCP_COMPARE_MODE_INIT_PIN_HIGH:
                    CCP1_SET_MODE(CCP_COMPARE_MODE_INIT_PIN_HIGH);
                    break;
                    
                case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
                    CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                    break;
                
                case CCP_COMPARE_MODE_TRIG_EVENT:
                    CCP1_SET_MODE(CCP_COMPARE_MODE_TRIG_EVENT);
                    break;
            }
        }
        
        else if(CCP2_INST == _ccp_obj->ccp_inst)
        {
            switch(_ccp_obj->ccp_mode_variant)
            {
                case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
                    CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);
                    break;
                    
                case CCP_COMPARE_MODE_INIT_PIN_LOW:
                    CCP2_SET_MODE(CCP_COMPARE_MODE_INIT_PIN_LOW);
                    break;
                    
                case CCP_COMPARE_MODE_INIT_PIN_HIGH:
                    CCP2_SET_MODE(CCP_COMPARE_MODE_INIT_PIN_HIGH);
                    break;
                    
                case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
                    CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT);
                    break;
                
                case CCP_COMPARE_MODE_TRIG_EVENT:
                    CCP2_SET_MODE(CCP_COMPARE_MODE_TRIG_EVENT);
                    break;
            }
        }
        CCP_Mode_Timer_Select(_ccp_obj);
    }

    return ret_val;
}

/* ------------------------- ISR ------------------------- */
void CCP1_ISR(void)
{
    CCP1_InterruptFlagClear();
    if(CCP1_InterruptHandler)
    {
        CCP1_InterruptHandler();
    }
}

void CCP2_ISR(void)
{
    CCP2_InterruptFlagClear();
    if(CCP2_InterruptHandler)
    {
        CCP2_InterruptHandler();
    }
}