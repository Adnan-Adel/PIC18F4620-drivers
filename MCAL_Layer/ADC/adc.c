/* 
 * File:   adc.c
 * Author: Adnan
 *
 * Created on 25 ?????, 2023, 06:46 ?
 */
#include "adc.h"

#ifdef ADC_INTERRUPT_FEATURE_ENABLE
static void(*ADC_InterruptHandler)(void)= NULL;
#endif

static inline void adc_input_channel_port_configure(adc_channel_select_t channel);
static inline void adc_select_result_format(const adc_cfg_t *_adc_obj);
static inline void adc_configure_voltage_reference(const adc_cfg_t *_adc_obj);

/**
 * 
 * @param _adc_obj
 * @return 
 */
std_ReturnType ADC_Init(const adc_cfg_t *_adc_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _adc_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        
        /* Configure the acquisition time */
        ADCON2bits.ACQT= _adc_obj->acquisition_time;
        
        /* Configure the conversion clock */
        ADCON2bits.ADCS= _adc_obj->conversion_clock;
        
        /* Configure the default channel */
        ADCON0bits.CHS= _adc_obj->adc_channel;
        adc_input_channel_port_configure(_adc_obj->adc_channel);
        
        /* Configure the interrupt */
        #ifdef ADC_INTERRUPT_FEATURE_ENABLE   
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        ADC_InterruptEnable();
        ADC_InterruptFlagClear();
        #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        if(INTERRUPT_HIGH_PRIORITY == _adc_obj->priority)
        {
            ADC_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == _adc_obj->priority)
        {
            ADC_LowPrioritySet();
        }
        #endif
        ADC_InterruptHandler= _adc_obj->ADC_InterruptHandler;
        #endif
        
        /* Configure the result format */
        adc_select_result_format(_adc_obj);
        
        /* Configure the voltage reference */
        adc_configure_voltage_reference(_adc_obj);
        
        /* Enable the ADC */
        ADC_CONVERTER_ENABLE();
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @return 
 */
std_ReturnType ADC_DeInit(const adc_cfg_t *_adc_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _adc_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable ADC */
        ADC_CONVERTER_DISABLE();
        
        /* Disable Interrupt */
        #ifdef ADC_INTERRUPT_FEATURE_ENABLE   
            ADC_InterruptDisable();
        #endif
        
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @param channel
 * @return 
 */
std_ReturnType ADC_SelectChannel(const adc_cfg_t *_adc_obj, adc_channel_select_t channel)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _adc_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ADCON0bits.CHS= channel;
        adc_input_channel_port_configure(channel);
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @return 
 */
std_ReturnType ADC_StartConversion(const adc_cfg_t *_adc_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _adc_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ADC_START_CONVERSION();
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @param conversion_status
 * @return 
 */
std_ReturnType ADC_IsConversionDone(const adc_cfg_t *_adc_obj, uint8_t *conversion_status)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _adc_obj) || (NULL == conversion_status))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *conversion_status= (uint8_t)(!(ADCON0bits.GODONE));
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @param conversion_result
 * @return 
 */
std_ReturnType ADC_GetConversionResult(const adc_cfg_t *_adc_obj, uint16_t *conversion_result)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _adc_obj) || (NULL == conversion_result))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(ADC_RESULT_RIGHT == _adc_obj->result_format)
        {
            *conversion_result= (uint16_t)((ADRESH << 8) + ADRESL);
        }
        else if(ADC_RESULT_LEFT == _adc_obj->result_format)
        {
            *conversion_result= (uint16_t)(((ADRESH << 8) + ADRESL) >> 6);
        }
        else
        {
            *conversion_result= (uint16_t)((ADRESH << 8) + ADRESL);
        }
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @param channel
 * @param conversion_result
 * @return 
 */
std_ReturnType ADC_Blocking_GetConversion(const adc_cfg_t *_adc_obj, adc_channel_select_t channel, uint16_t *conversion_result)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _adc_obj) || (NULL == conversion_result))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Select A/D channel */
        ret_val= ADC_SelectChannel(_adc_obj, channel);
        
        /* Start Conversion */
        ret_val= ADC_StartConversion(_adc_obj);
        
        /* Check if conversion is completed */
        while(ADCON0bits.GODONE);
        ret_val= ADC_GetConversionResult(_adc_obj, conversion_result);
    }
    return ret_val;
}

/**
 * 
 * @param _adc_obj
 * @param channel
 * @return 
 */
std_ReturnType ADC_StartConversion_Interrupt(const adc_cfg_t *_adc_obj, adc_channel_select_t channel)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _adc_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Select A/D channel */
        ret_val= ADC_SelectChannel(_adc_obj, channel);
        
        /* Start Conversion */
        ret_val= ADC_StartConversion(_adc_obj);
    }
    return ret_val;
}

static inline void adc_input_channel_port_configure(adc_channel_select_t channel)
{
    switch(channel)
    {
        case ADC_CHANNEL_AN0:  SET_BIT(TRISA, _TRISA_RA0_POSN); break;
        case ADC_CHANNEL_AN1:  SET_BIT(TRISA, _TRISA_RA1_POSN); break;
        case ADC_CHANNEL_AN2:  SET_BIT(TRISA, _TRISA_RA2_POSN); break;
        case ADC_CHANNEL_AN3:  SET_BIT(TRISA, _TRISA_RA3_POSN); break;
        case ADC_CHANNEL_AN4:  SET_BIT(TRISA, _TRISA_RA5_POSN); break;
        case ADC_CHANNEL_AN5:  SET_BIT(TRISE, _TRISE_RE0_POSN); break;
        case ADC_CHANNEL_AN6:  SET_BIT(TRISE, _TRISE_RE1_POSN); break;
        case ADC_CHANNEL_AN7:  SET_BIT(TRISE, _TRISE_RE2_POSN); break;
        case ADC_CHANNEL_AN8:  SET_BIT(TRISB, _TRISB_RB2_POSN); break;
        case ADC_CHANNEL_AN9:  SET_BIT(TRISB, _TRISB_RB3_POSN); break;
        case ADC_CHANNEL_AN10: SET_BIT(TRISB, _TRISB_RB1_POSN); break;
        case ADC_CHANNEL_AN11: SET_BIT(TRISB, _TRISB_RB4_POSN); break;
        case ADC_CHANNEL_AN12: SET_BIT(TRISB, _TRISB_RB0_POSN); break;
    }
}

static inline void adc_select_result_format(const adc_cfg_t *_adc_obj)
{
    if(ADC_RESULT_RIGHT == _adc_obj->result_format)
    {
        ADC_RESULT_RIGHT_FORMAT();
    }
    else if(ADC_RESULT_LEFT == _adc_obj->result_format)
    {
        ADC_RESULT_LEFT_FORMAT();
    }
    else
    {
        ADC_RESULT_RIGHT_FORMAT();
    }
}

static inline void adc_configure_voltage_reference(const adc_cfg_t *_adc_obj)
{
    if(ADC_VOLTAGE_REFERENCE_ENABLED == _adc_obj->voltage_reference)
    {
        ADC_ENABLE_VOLTAGE_REFERENCE();
    }
    else if(ADC_VOLTAGE_REFERENCE_DISABLED == _adc_obj->voltage_reference)
    {
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }
    else
    {
        ADC_DISABLE_VOLTAGE_REFERENCE();
    }
}

void ADC_ISR(void)
{
    ADC_InterruptFlagClear();
    if(ADC_InterruptHandler)
    {
        ADC_InterruptHandler();
    }
}