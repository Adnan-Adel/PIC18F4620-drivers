/* 
 * File:   spi.h
 * Author: Adnan
 *
 * Created on July 29, 2023, 11:19 AM
 */

#ifndef SPI_H
#define	SPI_H

/* ------------------------------------ Includes ------------------------------------ */
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"

/* ------------------------------------ Macro Declarations ------------------------------------ */
/* SPI Mode Select */
#define SPI_MODE_ENABLE_CFG                          1
#define SPI_MODE_DISABLE_CFG                         0

/* SPI Sample Select */
#define SPI_DATA_SAMPLE_MIDDLE_CFG                   0 /* Input data sampled at middle of data output time */
#define SPI_DATA_SAMPLE_END_CFG                      1 /* Input data sampled at end of data output time */

/* SPI Clock Select */
#define SPI_TRANSMIT_ACTIVE_TO_IDLE_CFG              1 /* Transmit occurs on transition from active to Idle clock state */
#define SPI_TRANSMIT_IDLE_TO_ACTIVE_CFG              0 /* Transmit occurs on transition from Idle to active clock state */

/* Buffer Full Status bit (Receive mode only) */ 
#define SPI_BUFFER_FULL_RECEIVE_COMPLETE_CFG         1 /* Receive complete, SSPBUF is full */
#define SPI_BUFFER_FULL_RECEIVE_NOT_COMPLETE_CFG     0 /* Receive not complete, SSPBUF is empty */

/* Receive Overflow Indicator */
#define SPI_SLAVE_RECEIVE_OVERFLOW_DETECTED_CFG      1 /* A new byte is received while the SSPBUF register is still holding 
                                                          the previous data.In case of overflow, the data in SSPSR is lost. */
#define SPI_SLAVE_RECEIVE_OVERFLOW_NOT_DETECTED_CFG  0 /* No overflow */

/* Clock Polarity Select */
#define SPI_IDLE_STATE_HIGH_LEVEL_CFG                1 /* Idle state for clock is a high level */
#define SPI_IDLE_STATE_LOW_LEVEL_CFG                 0 /* Idle state for clock is a low level */

/* Master Synchronous Serial Port Mode Select */
#define SPI_MASTER_FOSC_DIV4_CFG                     0 /* SPI Slave mode, clock = SCK pin, SS pin control disabled, 
                                                          SS can be used as I/O pin */
#define SPI_MASTER_FOSC_DIV16_CFG                    1 /* SPI Slave mode, clock = SCK pin, SS pin control enabled */
#define SPI_MASTER_FOSC_DIV64_CFG                    2 /* SPI Master mode, clock = TMR2 output/2 */
#define SPI_MASTER_FOSC_TMR2_CFG                     3 /* SPI Master mode, clock = FOSC/64 */
#define SPI_SLAVE_SS_ENABLE_CFG                      4 /* SPI Master mode, clock = FOSC/16 */
#define SPI_SLAVE_SS_DISABLE_CFG                     5 /* SPI Master mode, clock = FOSC/4 */

/* Write Collision Detect */
#define SPI_WRITE_COLLISION_DETECTED_CFG             1 /*The SSPBUF register is written while it is still transmitting the previous word*/
#define SPI_WRITE_COLLISION_NOT_DETECTED_CFG         0 /* No collision */

/* ------------------------------------ Macro Functions Declarations ------------------------------------ */
/* SPI Mode Select */
#define SPI_MODE_ENABLE_SELECT()                            SSPCON1bits.SSPEN = SPI_MODE_ENABLE_CFG
#define SPI_MODE_DISABLE_SELECT()                           SSPCON1bits.SSPEN = SPI_MODE_DISABLE_CFG

/* SPI Sample Select */
#define SPI_DATA_SAMPLE_AT_MIDDLE_SELECT()                  SSPSTATbits.SMP= SPI_DATA_SAMPLE_MIDDLE_CFG
#define SPI_DATA_SAMPLE_AT_END_SELECT()                     SSPSTATbits.SMP= SPI_DATA_SAMPLE_END_CFG

/* SPI Clock Select */
#define SPI_TRANSMIT_ACTIVE_TO_IDLE_SELECT()                SSPSTATbits.CKE= SPI_TRANSMIT_ACTIVE_TO_IDLE_CFG
#define SPI_TRANSMIT_IDLE_TO_ACTIVE_SELECT()                SSPSTATbits.CKE= SPI_TRANSMIT_IDLE_TO_ACTIVE_CFG

/* Clock Polarity Select */
#define SPI_IDLE_STATE_HIGH_LEVEL_SELECT()                  SSPCON1bits.CKP= SPI_IDLE_STATE_HIGH_LEVEL_CFG
#define SPI_IDLE_STATE_LOW_LEVEL_SELECT()                   SSPCON1bits.CKP= SPI_IDLE_STATE_LOW_LEVEL_CFG

/* Master Synchronous Serial Port Mode Select */
#define SYNCHRONOUS_SERIAL_PORT_MODE(_CONFIG)               SSPCON1bits.SSPM= _CONFIG

/* ------------------------------------ Data Type Declarations ------------------------------------ */
typedef struct
{
    uint8_t ClockPolarity : 2; /* Clock Polarity Select */
    uint8_t SampleSelect : 2;  /* SPI Sample Select */
    uint8_t ClockSelect : 2;   /* SPI Clock Select */
    uint8_t Reserved : 2;
}SPI_Control_Config;

typedef struct
{
#ifdef MSSP_SPI_INTERRUPT_FEATURE_ENABLE
    void (* MSSP_SPI_InterruptHandler)(void);
    Interrupt_priority_cfg priority;
#endif
    uint8_t spi_mode;
    SPI_Control_Config spi_config;
}SPI_Config;

/* ------------------------------------ Software Interface Declaration ------------------------------------ */
std_ReturnType SPI_Init(const SPI_Config *_Config);
std_ReturnType SPI_DeInit(const SPI_Config *_Config);
std_ReturnType SPI_Send_Byte(const SPI_Config *_Config, const uint8_t _data);
std_ReturnType SPI_Read_Byte(const SPI_Config *_Config, uint8_t *_data);
std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_Config *_Config, const uint8_t _data);
std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_Config *_Config, uint8_t *_data);

#endif	/* SPI_H */

