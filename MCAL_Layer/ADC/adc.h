/* 
 * File:   adc.h
 * Author: Adnan
 *
 * Created on 25 ?????, 2023, 06:46 ?
 */

#ifndef ADC_H
#define	ADC_H
/* Section: Includes */
#include "adc_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* Section: Macro Declarations */

/**
 * @brief: A/D port configuration control
 * @note : when ADC_ANn_ANALOG_FUNCTIONALITY is configured this means
 *          ANn, ANn-1,..., AN0 are analog functionality
 */
#define ADC_AN0_ANALOG_FUNCTIONALITY         0x0E
#define ADC_AN1_ANALOG_FUNCTIONALITY         0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY         0x0C
#define ADC_AN3_ANALOG_FUNCTIONALITY         0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY         0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY         0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY         0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY         0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY         0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY         0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY        0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY        0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY        0x02
#define ADC_ALL_ANALOG_FUNCTIONALITY         0x00
#define ADC_ALL_DIGITAL_FUNCTIONALITY        0x0F

#define ADC_RESULT_RIGHT                     0x01U
#define ADC_RESULT_LEFT                      0x00U

#define ADC_VOLTAGE_REFERENCE_ENABLED         0x01U
#define ADC_VOLTAGE_REFERENCE_DISABLED        0x00U

/* Section: Macro Functions Declarations */

/* A/D conversion status: in progress or idle */
#define ADC_CONVERSION_STATUS()     (ADCON0bits.GODONE)

/* 
 * @brief: start the A/D Conversion
 */
#define ADC_START_CONVERSION()      (ADCON0bits.GODONE= 1)

/*
 * @brief: Analog to digital control
 * @note: ADC_CONVERTER_ENABLE(): enable A/D
 *        ADC_CONVERTER_DISABLE(): disable A/D
 */
#define ADC_CONVERTER_ENABLE()      (ADCON0bits.ADON= 1)
#define ADC_CONVERTER_DISABLE()     (ADCON0bits.ADON= 0)

/*
 * @brief: Voltage Reference Configuration
 * @note: ADC_ENABLE_VOLTAGE_REFERENCE(): Voltage Reference is VREF- and VREF+
 *        ADC_DISABLE_VOLTAGE_REFERENCE(): Voltage Reference is VSS and VDD
 */
#define ADC_ENABLE_VOLTAGE_REFERENCE()     do{ ADCON1bits.VCFG1= 1;\
                                               ADCON1bits.VCFG0= 1;\
                                              }while(0)   
#define ADC_DISABLE_VOLTAGE_REFERENCE()    do{ ADCON1bits.VCFG1= 0;\
                                               ADCON1bits.VCFG0= 0;\
                                              }while(0)

/**
 * @brief: A/D port configuration control
 * @note : when ADC_ANn_ANALOG_FUNCTIONALITY is configured this means
 *          ANn, ANn-1,..., AN0 are analog functionality
 * @ref  : A/D port configuration control
 */
#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_CONFIG)    (ADCON1bits.PCFG= _CONFIG)

/*
 * @brief: A/D Result Format Select
 */
#define ADC_RESULT_RIGHT_FORMAT()   (ADCON2bits.ADFM= 1)
#define ADC_RESULT_LEFT_FORMAT()    (ADCON2bits.ADFM= 0)

/* Section: Data Type Declarations */

/**
 * @brief: Analog Channel Select
 */
typedef enum
{
    ADC_CHANNEL_AN0= 0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,        
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,         
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,        
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,        
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,        
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12                 
}adc_channel_select_t;

/**
 * @brief  A/D Acquisition Time Select 
 * @note   Acquisition time (sampling time) is the time required for the (ADC) to 
 *         capture the input voltage during sampling.
 * @note   Acquisition time of a Successive Approximation Register (SAR) ADC is the amount of time 
 *         required to charge the holding capacitor (CHOLD) on the front end of an ADC
 */
typedef enum
{
    ADC_0_TAD= 0,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,  
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD  
}adc_acquisition_time_t;

/**
 * @brief  A/D Conversion Clock Select
 * @note   If the A/D FRC clock source is selected, a delay of one TCY (instruction cycle) 
 *         is added before the A/D clock starts.
 * @note   This allows the SLEEP instruction to be executed before starting a conversion.
 */
typedef enum{
    ADC_CONVERSION_CLOCK_FOSC_DIV_2 = 0,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32,
    ADC_CONVERSION_CLOCK_FOSC_DIV_FRC,
    ADC_CONVERSION_CLOCK_FOSC_DIV_4,
    ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64
}adc_conversion_clock_t;

typedef struct
{
    #ifdef ADC_INTERRUPT_FEATURE_ENABLE   
            void (*ADC_InterruptHandler)(void);
            Interrupt_priority_cfg priority;
    #endif
    adc_channel_select_t adc_channel;               /* @ref: adc_channel_select_t */
    adc_acquisition_time_t acquisition_time;        /* @ref: adc_acquisition_time_t */
    adc_conversion_clock_t conversion_clock;        /* @ref: adc_conversion_clock_t */
    uint8_t voltage_reference : 1;                  /* Voltage Reference Configuration */
    uint8_t result_format : 1;                      /* A/D Result Format Select */
    uint8_t ADC_Reserved : 6;
    
}adc_cfg_t;

/* Section: Software Interface Declaration*/
std_ReturnType ADC_Init(const adc_cfg_t *_adc_obj);
std_ReturnType ADC_DeInit(const adc_cfg_t *_adc_obj);
std_ReturnType ADC_SelectChannel(const adc_cfg_t *_adc_obj, adc_channel_select_t channel);
std_ReturnType ADC_StartConversion(const adc_cfg_t *_adc_obj);
std_ReturnType ADC_IsConversionDone(const adc_cfg_t *_adc_obj, uint8_t *conversion_status);
std_ReturnType ADC_GetConversionResult(const adc_cfg_t *_adc_obj, uint16_t *conversion_result);
std_ReturnType ADC_Blocking_GetConversion(const adc_cfg_t *_adc_obj, adc_channel_select_t channel, uint16_t *conversion_result);

std_ReturnType ADC_StartConversion_Interrupt(const adc_cfg_t *_adc_obj, adc_channel_select_t channel);

#endif	/* ADC_H */

