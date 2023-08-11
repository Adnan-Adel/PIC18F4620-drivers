/* 
 * File:   ccp.h
 * Author: Adnan
 *
 * Created on July 16, 2023, 12:37 AM
 */

#ifndef CCP_H
#define	CCP_H

/* ------------------------------------ Includes ------------------------------------ */
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "ccp_cfg.h"

/* ------------------------------------ Macro Declarations ------------------------------------ */

/* CCP Mode Select */
#define CCP_MODULE_DISABLE                              ((uint8_t)0x00)     //Capture/Compare/PWM disabled
#define CCP_CAPTURE_MODE_1_FALLING_EDGE                 ((uint8_t)0x04)     //Capture mode, every falling edge
#define CCP_CAPTURE_MODE_1_RISING_EDGE                  ((uint8_t)0x05)     //Capture mode, every rising edge
#define CCP_CAPTURE_MODE_4_RISING_EDGE                  ((uint8_t)0x06)     //Capture mode, every 4th rising edge
#define CCP_CAPTURE_MODE_16_RISING_EDGE                 ((uint8_t)0x07)     //Capture mode, every 16th rising edge
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH                ((uint8_t)0x02)     // Compare mode, toggle output on matchX
#define CCP_COMPARE_MODE_INIT_PIN_LOW                   ((uint8_t)0x08)     // Compare mode: initialize CCP2 pin low; on compare match, force CCP2 pin high
#define CCP_COMPARE_MODE_INIT_PIN_HIGH                  ((uint8_t)0x09)     // Compare mode: initialize CCP2 pin high; on compare match, force CCP2 pin low
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT               ((uint8_t)0x0A)     //Compare mode: generate software interrupt on compare match
#define CCP_COMPARE_MODE_TRIG_EVENT                     ((uint8_t)0x0B)      // Compare mode: trigger special event, reset timer, start A/D conversion on CCP2 match
#define CCP_PWM_MODE                                    ((uint8_t)0x0C)     // PWM mode

/* CCP1 Capture Mode States */
#define CCP1_CAPTURE_NOT_READY                          0X00
#define CCP1_CAPTURE_READY                              0X01

/* CCP1 Compare Mode States */
#define CCP1_COMPARE_NOT_READY                          0X00
#define CCP1_COMPARE_READY                              0X01

/* CCP2 Capture Mode States */
#define CCP2_CAPTURE_NOT_READY                          0X00
#define CCP2_CAPTURE_READY                              0X01

/* CCP2 Compare Mode States */
#define CCP2_COMPARE_NOT_READY                          0X00
#define CCP2_COMPARE_READY                              0X01

/* Timer2 Input Clock Pre-scaler */
#define CCP_TIMER2_PRESCALER_DIV_BY_1                   1
#define CCP_TIMER2_PRESCALER_DIV_BY_4                   4
#define CCP_TIMER2_PRESCALER_DIV_BY_16                  16

/* Timer2 Input Clock Post-scaler */
#define CCP_TIMER2_POSTSCALER_DIV_BY_1                  1
#define CCP_TIMER2_POSTSCALER_DIV_BY_2                  2
#define CCP_TIMER2_POSTSCALER_DIV_BY_3                  3
#define CCP_TIMER2_POSTSCALER_DIV_BY_4                  4
#define CCP_TIMER2_POSTSCALER_DIV_BY_5                  5
#define CCP_TIMER2_POSTSCALER_DIV_BY_6                  6
#define CCP_TIMER2_POSTSCALER_DIV_BY_7                  7
#define CCP_TIMER2_POSTSCALER_DIV_BY_8                  8
#define CCP_TIMER2_POSTSCALER_DIV_BY_9                  9
#define CCP_TIMER2_POSTSCALER_DIV_BY_10                 10
#define CCP_TIMER2_POSTSCALER_DIV_BY_11                 11
#define CCP_TIMER2_POSTSCALER_DIV_BY_12                 12
#define CCP_TIMER2_POSTSCALER_DIV_BY_13                 13
#define CCP_TIMER2_POSTSCALER_DIV_BY_14                 14
#define CCP_TIMER2_POSTSCALER_DIV_BY_15                 15
#define CCP_TIMER2_POSTSCALER_DIV_BY_16                 16

/* ------------------------------------ Macro Functions Declarations ------------------------------------ */

/* CCP1 Set Mode */
#define CCP1_SET_MODE(_MODE)        (CCP1CONbits.CCP1M= _MODE)
/* CCP2 Set Mode */
#define CCP2_SET_MODE(_MODE)        (CCP2CONbits.CCP2M= _MODE)

/* ------------------------------------ Data Type Declarations ------------------------------------ */
typedef enum
{
    CCP_CAPTURE_MODE_SELECT= 0,
    CCP_COMPARE_MODE_SELECT,
    CCP_PWM_MODE_SELECT
}ccp_mode_t;

typedef enum
{
    CCP1_INST= 0,
    CCP2_INST
}ccp_inst_t;

typedef enum{
    CCP1_CCP2_TIMER3 = 0,
    CCP1_TIMER1_CCP2_TIMER3,
    CCP1_CCP2_TIMER1
}ccp_capture_timer_t;

typedef union
{
    struct{
        uint8_t ccp_reg_low;    /* CCP Low Register */
        uint8_t ccp_reg_high;   /* CCP high Register */
    };
    struct{
        uint16_t ccp_reg_16bit;     /* Read CCP Register as a 16-bit value */
    };
    
}ccp_reg_t;

/* CCP Module Configurations */
typedef struct
{
    ccp_mode_t ccp_mode;
    pin_config_t ccp_pin;
    uint8_t ccp_mode_variant;
    ccp_inst_t ccp_inst;
    ccp_capture_timer_t ccp_capture_timer;

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED))
    uint32_t ccp_pwm_freq;
    uint8_t TMR2_postscaler_value;
    uint8_t TMR2_prescaler_value;
#endif
    
#ifdef CCP1_INTERRUPT_FEATURE_ENABLE
    void (* CCP1_InterruptHandler)(void);
    Interrupt_priority_cfg ccp1_priority;
#endif
    
#ifdef CCP2_INTERRUPT_FEATURE_ENABLE
    void (* CCP2_InterruptHandler)(void);
    Interrupt_priority_cfg ccp2_priority;
#endif
}ccp_t;

/* ------------------------------------ Software Interface Declaration ------------------------------------ */

std_ReturnType CCP_Init(const ccp_t* _ccp_obj);
std_ReturnType CCP_DeInit(const ccp_t* _ccp_obj);

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED))
    std_ReturnType CCP_IsCapturedDataReady(const ccp_t* _ccp_obj, uint8_t *_capture_status);
    std_ReturnType CCP_Capture_Mode_Read_Value(const ccp_t* _ccp_obj, uint16_t *capture_value);
#endif

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED))
    std_ReturnType CCP_IsCompareComplete(const ccp_t* _ccp_obj, uint8_t *_compare_status);
    std_ReturnType CCP_Compare_Mode_Set_Value(const ccp_t *_ccp_obj, uint16_t compare_value);
#endif

#if ((CCP1_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED) || (CCP2_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED))
    std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8_t _duty);
    std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj);
    std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj);
#endif

#endif	/* CCP_H */

