/* 
 * File:   I2C.h
 * Author: Adnan
 *
 * Created on August 10, 2023, 10:57 PM
 */

#ifndef I2C_H
#define	I2C_H

/* ------------------------------------ Includes ------------------------------------ */
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* ------------------------------------ Macro Declarations ------------------------------------ */
/* Slew Rate Enable/Disable */
#define I2C_SLEW_RATE_ENABLE_CFG                            0
#define I2C_SLEW_RATE_DISABLE_CFG                           1

/* SMBUS Enable/Disable */
#define I2C_SMBUS_ENABLE_CFG                                1
#define I2C_SMBUS_DISABLE_CFG                               0

/* Slave Mode Data/Address Indication */
#define I2C_LAST_BYTE_DATA                                  1
#define I2C_LAST_BYTE_ADDRESS                               0

/* Stop Condition Indication */
#define STOP_BIT_DETECTED                                   1
#define STOP_BIT_NOT_DETECTED                               0

/* Start Condition Indication */
#define START_BIT_DETECTED                                  1
#define START_BIT_NOT_DETECTED                              0

/* I2C Mode: Master or Slave */
#define I2C_MASTER_MODE                                     1
#define I2C_SLAVE_MODE                                      0

/* I2C Mode SSPM<3:0> */
#define I2C_SLAVE_MODE_7BIT_ADDRESS_CFG                     0x06U
#define I2C_SLAVE_MODE_10BIT_ADDRESS_CFG                    0x07U
#define I2C_SLAVE_MODE_7BIT_ADDRESS_SS_INT_ENABLE_CFG       0x0EU    
#define I2C_SLAVE_MODE_10BIT_ADDRESS_SS_INT_ENABLE_CFG      0x0FU       
#define I2C_MASTER_MODE_DEFINED_CLOCK_CFG                   0x08U
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED_CFG             0x0BU

/* General Call Enable */
#define I2C_GENERAL_CALL_ENABLE_CFG                         1
#define I2C_GENERAL_CALL_DISABLE_CFG                        0

/* Master Mode Receive Enable */
#define I2C_MASTER_RECEIVE_ENABLE_CFG                       1
#define I2C_MASTER_RECEIVE_DISABLE_CFG                      0

/* Acknowledge Status bit (Master Transmit mode only) */
#define I2C_ACK_RECEIVED_FROM_SLAVE     0
#define I2C_ACK_NOT_RECEIVED_FROM_SLAVE 1

/* Acknowledge Data bit (Master Receive mode only) */
#define I2C_MASTER_SEND_ACK  0
#define I2C_MASTER_SEND_NACK 1

/* ------------------------------------ Macro Functions Declarations ------------------------------------ */
/* I2C Mode Select */
#define MSSP_MODULE_I2C_ENABLE()                         (SSPCON1bits.SSPEN = 1) 
#define MSSP_MODULE_I2C_DISABLE()                        (SSPCON1bits.SSPEN = 0)

/* Slew Rate */
#define I2C_SLEW_RATE_ENABLE()                           (SSPSTATbits.SMP= I2C_SLEW_RATE_ENABLE_CFG)
#define I2C_SLEW_RATE_DISABLE()                          (SSPSTATbits.SMP= I2C_SLEW_RATE_DISABLE_CFG)

/* SMBUS */
#define I2C_SMBUS_ENABLE()                               (SSPSTATbits.CKE= I2C_SMBUS_ENABLE_CFG)
#define I2C_SMBUS_DISABLE()                              (SSPSTATbits.CKE= I2C_SMBUS_DISABLE_CFG)

/* I2C Mode */
#define I2C_SLAVE_MODE_7BIT_ADDRESS()                    (SSPCON1bits.SSPM= I2C_SLAVE_MODE_7BIT_ADDRESS_CFG)
#define I2C_SLAVE_MODE_10BIT_ADDRESS()                   (SSPCON1bits.SSPM= I2C_SLAVE_MODE_10BIT_ADDRESS_CFG)
#define I2C_SLAVE_MODE_7BIT_ADDRESS_SS_INT_ENABLE()      (SSPCON1bits.SSPM= I2C_SLAVE_MODE_7BIT_ADDRESS_SS_INT_ENABLE_CFG)
#define I2C_SLAVE_MODE_10BIT_ADDRESS_SS_INT_ENABLE()     (SSPCON1bits.SSPM= I2C_SLAVE_MODE_10BIT_ADDRESS_SS_INT_ENABLE_CFG)
#define I2C_MASTER_MODE_DEFINED_CLOCK()                  (SSPCON1bits.SSPM= I2C_MASTER_MODE_DEFINED_CLOCK_CFG)
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED()            (SSPCON1bits.SSPM= I2C_MASTER_MODE_FIRMWARE_CONTROLLED_CFG)
#define I2C_MODE_SELECT(_MODE_)                          (SSPCON1bits.SSPM= _MODE_)
        
/* General Call Enable */
#define I2C_GENERAL_CALL_ENABLE()                        (SSPCON2bits.GCEN= I2C_GENERAL_CALL_ENABLE_CFG)
#define I2C_GENERAL_CALL_DISABLE()                       (SSPCON2bits.GCEN= I2C_GENERAL_CALL_DISABLE_CFG)

/* Master Mode Receive Enable */
#define I2C_MASTER_RECEIVE_ENABLE()                      (SSPCON2bits.RCEN= I2C_MASTER_RECEIVE_ENABLE_CFG)   
#define I2C_MASTER_RECEIVE_DISABLE()                     (SSPCON2bits.RCEN= I2C_MASTER_RECEIVE_DISABLE_CFG)

/* Clock Stretch */
#define I2C_CLOCK_STRETCH_ENABLE()                       (SSPCON1bits.CKP = 0)
#define I2C_CLOCK_STRETCH_DISABLE()                      (SSPCON1bits.CKP = 1)

/* Acknowledge Sequence */
#define I2C_INITIATE_ACK_SEQUENCE()                      (SSPCON2bits.ACKEN= 1)
#define I2C_STOP_ACK_SEQUENCE()                          (SSPCON2bits.ACKEN= 0)
    
/* ------------------------------------ Data Type Declarations ------------------------------------ */
typedef struct
{
    uint8_t i2c_mode_cfg;
    uint8_t i2c_slave_address;
    uint8_t i2c_mode : 1;
    uint8_t i2c_slew_rate : 1;
    uint8_t i2c_SMBUS_control : 1;
    uint8_t i2c_general_call : 1;
    uint8_t i2c_master_rec_mode : 1;
    uint8_t i2c_reserved : 3;
    #ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
        Interrupt_priority_cfg mssp_i2c_priority;
        Interrupt_priority_cfg mssp_i2c_bc_priority;
    #endif
    
}i2c_config_t;
typedef struct
{
    uint32_t i2c_clock;                             /* Master Clock Frequency */
    i2c_config_t i2c_cfg;
    #ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
        void (*I2C_Report_Write_Collision)(void);       /* Write Collision Indicator */
        void (*I2C_Report_Receive_Overflow)(void);      /* Receive Overflow Indicator */
        void (*I2C_InterruptHandler)(void);             /* Default Interrupt Handler */
    #endif
}i2c_t;

/* ------------------------------------ Software Interface Declaration ------------------------------------ */
std_ReturnType I2C_Init(const i2c_t* _i2c_obj);
std_ReturnType I2C_DeInit(const i2c_t* _i2c_obj);
std_ReturnType I2C_Master_Send_Start(const i2c_t* _i2c_obj);
std_ReturnType I2C_Master_Send_Repeated_Start(const i2c_t* _i2c_obj);
std_ReturnType I2C_Master_Send_Stop(const i2c_t* _i2c_obj);
std_ReturnType I2C_Write(const i2c_t* _i2c_obj, uint8_t _data, uint8_t* _ack);
std_ReturnType I2C_Read(const i2c_t* _i2c_obj, uint8_t* _data, uint8_t _ack);


#endif	/* I2C_H */

