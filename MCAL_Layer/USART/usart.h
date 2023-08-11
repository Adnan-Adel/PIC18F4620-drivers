/* 
 * File:   usart.h
 * Author: Adnan
 *
 * Created on July 25, 2023, 11:07 AM
 */

#ifndef USART_H
#define	USART_H

/* ------------------------------------ Includes ------------------------------------ */
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "usart_cfg.h"

/* ------------------------------------ Macro Declarations ------------------------------------ */
/* EUSART Module Status */
#define EUSART_MODULE_ENABLE_CFG                            1
#define EUSART_MODULE_DISABLE_CFG                           0
/* EUSART Working Mode */
#define EUSART_SYNCHRONOUS_MODE                         1
#define EUSART_ASYNCHRONOUS_MODE                        0
/* Asynchronous Baud Rate Generator Speed Mode */
#define EUSART_ASYNCHRONOUS_HIGH_SPEED                  1
#define EUSART_ASYNCHRONOUS_LOW_SPEED                   0
/* Asynchronous Baud Rate Generator Register Size */
#define EUSART_16BIT_BAUDRATE_GEN                       1
#define EUSART_8BIT_BAUDRATE_GEN                        0

/* EUSART Transmit Status */
#define EUSART_ASYNCHRONOUS_TX_ENABLE                   1
#define EUSART_ASYNCHRONOUS_TX_DISABLE                  0
/* EUSART Transmit Interrupt Status */
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE         1
#define EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE        0
/* EUSART 9-Bit Transmit Status */
#define EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE              1
#define EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE             0

/* EUSART Receiver Status */
#define EUSART_ASYNCHRONOUS_RX_ENABLE                   1
#define EUSART_ASYNCHRONOUS_RX_DISABLE                  0
/* EUSART Receiver Interrupt Status */
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE         1
#define EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE        0
/* EUSART 9-Bit Receiver Status */
#define EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE              1
#define EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE             0

/* EUSART Framing Error */
#define EUSART_FRAMING_ERROR_DETECTED                   1
#define EUSART_FRAMING_ERROR_CLEARED                    0
/* EUSART Overrun Error */
#define EUSART_OVERRUN_ERROR_DETECTED                   1
#define EUSART_OVERRUN_ERROR_CLEARED                    0

/* ------------------------------------ Macro Functions Declarations ------------------------------------ */
/* EUSART Module Enable */
#define EUSART_MODULE_ENABLE()                       RCSTAbits.SPEN= EUSART_MODULE_ENABLE_CFG
#define EUSART_MODULE_DISABLE()                      RCSTAbits.SPEN= EUSART_MODULE_DISABLE_CFG

/* EUSART Transmit Enable */
#define EUSART_ASYNCH_TX_ENABLE()                       TXSTAbits.TXEN= EUSART_ASYNCHRONOUS_TX_ENABLE
#define EUSART_ASYNCH_TX_DISABLE()                      TXSTAbits.TXEN= EUSART_ASYNCHRONOUS_TX_DISABLE

/* EUSART Transmit Interrupt Enable Configuration */
#define EUSART_ASYNCH_INTERRUPT_TX_ENABLE()             PIE1bits.TXIE= EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE
#define EUSART_ASYNCH_INTERRUPT_TX_DISABLE()            PIE1bits.TXIE= EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE
/* EUSART 9-bit Transmit Enable Configuration */
#define EUSART_ASYNCH_9BIT_TX_ENABLE()                  TXSTAbits.TX9= EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE;
#define EUSART_ASYNCH_9BIT_TX_DISABLE()                 TXSTAbits.TX9= EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE;

/* EUSART Receive Enable */
#define EUSART_ASYNCH_RX_ENABLE()                       RCSTAbits.CREN= EUSART_ASYNCHRONOUS_RX_ENABLE
#define EUSART_ASYNCH_RX_DISABLE()                      RCSTAbits.CREN= EUSART_ASYNCHRONOUS_RX_DISABLE

/* EUSART Receive Interrupt Enable Configuration */
#define EUSART_ASYNCH_INTERRUPT_RX_ENABLE()             PIE1bits.RCIE= EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE
#define EUSART_ASYNCH_INTERRUPT_RX_DISABLE()            PIE1bits.RCIE= EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE
/* EUSART 9-bit Receive Enable Configuration */
#define EUSART_ASYNCH_9BIT_RX_ENABLE()                  RCSTAbits.RX9= EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE;
#define EUSART_ASYNCH_9BIT_RX_DISABLE()                 RCSTAbits.RX9= EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE;

/* ------------------------------------ Data Type Declarations ------------------------------------ */
typedef enum
{
    BAUDRATE_ASYNCH_8BIT_LOW_SPEED,
    BAUDRATE_ASYNCH_8BIT_HIGH_SPEED,
    BAUDRATE_ASYNCH_16BIT_LOW_SPEED,
    BAUDRATE_ASYNCH_16BIT_HIGH_SPEED,
    BAUDRATE_SYNCH_8BIT,
    BAUDRATE_SYNCH_16BIT        
}baudrate_gen_t;

typedef struct
{
    Interrupt_priority_cfg usart_tx_interrupt_priority;
    uint8_t usart_tx_enable : 1;
    uint8_t usart_tx_interrupt_enable : 1;
    uint8_t usart_tx_9bit_enable : 1;
    uint8_t usart_tx_reserved : 5;
}usart_tx_cfg_t;

typedef struct
{
    Interrupt_priority_cfg usart_rx_interrupt_priority;
    uint8_t usart_rx_enable : 1;
    uint8_t usart_rx_interrupt_enable : 1;
    uint8_t usart_rx_9bit_enable : 1;
    uint8_t usart_rx_reserved : 5;
}usart_rx_cfg_t;

typedef union
{
    struct
    {
        uint8_t usart_ferr : 1;
        uint8_t usart_oerr : 1;
    };
    uint8_t error_state;
}usart_error_status_t;

typedef struct
{
    uint32_t baudrate;
    baudrate_gen_t baudrate_gen_config;
    usart_tx_cfg_t usart_tx_cfg;
    usart_rx_cfg_t usart_rx_cfg;
    usart_error_status_t usart_error_status;
    
    void (*EUSART_TxDefaultInterruptHandler)(void);
    void (*EUSART_RxDefaultInterruptHandler)(void);
    void (*EUSART_FramingErrorHandler)(void);
    void (*EUSART_OverrunErrorHandler)(void);
    
}usart_t;

/* ------------------------------------ Software Interface Declaration ------------------------------------ */
std_ReturnType EUSART_ASYNC_Init(const usart_t* _eusart_obj);
std_ReturnType EUSART_ASYNC_DeInit(const usart_t* _eusart_obj);

std_ReturnType EUSART_ASYNC_ReadChar_Blocking(uint8_t* data);
std_ReturnType EUSART_ASYNC_ReadChar_NonBlocking(uint8_t* data);
std_ReturnType EUSART_ASYNC_Rx_Restart(void);

std_ReturnType EUSART_ASYNC_WriteChar_Blocking(uint8_t data);
std_ReturnType EUSART_ASYNC_WriteChar_NonBlocking(uint8_t data);
std_ReturnType EUSART_ASYNC_WriteString_Blocking(uint8_t *data, uint16_t str_len);
std_ReturnType EUSART_ASYNC_WriteString_NonBlocking(uint8_t *data, uint16_t str_len);

#endif	/* USART_H */

