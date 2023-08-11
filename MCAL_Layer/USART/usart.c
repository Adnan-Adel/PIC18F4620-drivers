/* 
 * File:   usart.c
 * Author: Adnan
 *
 * Created on July 25, 2023, 11:07 AM
 */
#include "usart.h"

#ifdef EUSART_TX_INTERRUPT_FEATURE_ENABLE
    static void (*EUSART_TxInterruptHandler)(void) = NULL;
#endif
#ifdef EUSART_RX_INTERRUPT_FEATURE_ENABLE
    static void (*EUSART_RxInterruptHandler)(void) = NULL;
    static void (*EUSART_FramingErrorHandler)(void) = NULL;
    static void (*EUSART_OverrunErrorHandler)(void) = NULL;
#endif  

/* -------------------- Helper Functions Declarations -------------------- */
static void EUSART_Baud_Rate_Calculation(const usart_t *_eusart);
static void EUSART_ASYNC_TX_Init(const usart_t *_eusart);
static void EUSART_ASYNC_RX_Init(const usart_t *_eusart);


/* -------------------- Software Interface Definitions -------------------- */

std_ReturnType EUSART_ASYNC_Init(const usart_t* _eusart_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _eusart_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable EUSART */
        EUSART_MODULE_DISABLE();
        
        /* Configure RC7 and RC6 pins to be INPUT */
        TRISCbits.RC6= 1;
        TRISCbits.RC7= 1;
        
        /* Configure BRG */
        EUSART_Baud_Rate_Calculation(_eusart_obj);
        
        /* Configure TX */
        EUSART_ASYNC_TX_Init(_eusart_obj);
        
        /* Configure RX */
        EUSART_ASYNC_RX_Init(_eusart_obj);
        
        /* Enable EUSART */
        EUSART_MODULE_ENABLE();
    }
    return ret_val;
}

std_ReturnType EUSART_ASYNC_DeInit(const usart_t* _eusart_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _eusart_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable EUSART */
        EUSART_MODULE_DISABLE();
    }
    return ret_val;
}


std_ReturnType EUSART_ASYNC_ReadChar_Blocking(uint8_t* data)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == data)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        while(!PIR1bits.RCIF);
        *data= RCREG;
    }
    return ret_val;
}

std_ReturnType EUSART_ASYNC_ReadChar_NonBlocking(uint8_t* data)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == data)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        if(1 == PIR1bits.RCIF)
        {
            *data= RCREG;
            ret_val= E_OK;
        }
        else    
        {
            ret_val= E_NOT_OK;
        }
    }
    return ret_val;
}

std_ReturnType EUSART_ASYNC_Rx_Restart(void)
{
    std_ReturnType ret_val= E_OK;
    RCSTAbits.CREN = 0; /* Disables receiver */
    RCSTAbits.CREN = 1; /* Enables receiver */
    
    return ret_val;
}


std_ReturnType EUSART_ASYNC_WriteChar_Blocking(uint8_t data)
{
    std_ReturnType ret_val= E_OK;
    
    while(!(TXSTAbits.TRMT));
    #ifdef EUSART_TX_INTERRUPT_FEATURE_ENABLE
    EUSART_TX_InterruptEnable();
    #endif
    TXREG= data;
    
    return ret_val;
}

std_ReturnType EUSART_ASYNC_WriteChar_NonBlocking(uint8_t data)
{
    std_ReturnType ret_val= E_OK;
    
    if(1 == TXSTAbits.TRMT)
    {
        TXREG= data;
        ret_val= E_OK;
    }
    else
    {
        ret_val= E_NOT_OK;
    }
    return ret_val;
}

std_ReturnType EUSART_ASYNC_WriteString_Blocking(uint8_t *data, uint16_t str_len)
{
    std_ReturnType ret_val= E_OK;
    uint16_t index_i;
    for(index_i= 0; index_i < str_len; index_i++)
    {
        ret_val= EUSART_ASYNC_WriteChar_Blocking(data[index_i]);
    }
    return ret_val;
}

std_ReturnType EUSART_ASYNC_WriteString_NonBlocking(uint8_t *data, uint16_t str_len)
{
    
}



/* -------------------- Helper Functions Definitions -------------------- */
static void EUSART_Baud_Rate_Calculation(const usart_t *_eusart)
{
    float BaudRate_temp =0;
    switch(_eusart->baudrate_gen_config)
    {
        case BAUDRATE_ASYNCH_8BIT_LOW_SPEED:
            TXSTAbits.SYNC= EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH= EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16= EUSART_8BIT_BAUDRATE_GEN;
            BaudRate_temp= ((_XTAL_FREQ / (float)_eusart->baudrate) / 64) - 1;
        break;
        
        case BAUDRATE_ASYNCH_8BIT_HIGH_SPEED:
            TXSTAbits.SYNC= EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH= EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16= EUSART_8BIT_BAUDRATE_GEN;
            BaudRate_temp= ((_XTAL_FREQ / (float)_eusart->baudrate) / 16) - 1;
        break;
        
        case BAUDRATE_ASYNCH_16BIT_LOW_SPEED:
            TXSTAbits.SYNC= EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH= EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16= EUSART_16BIT_BAUDRATE_GEN;
            BaudRate_temp= ((_XTAL_FREQ / (float)_eusart->baudrate) / 16) - 1;
        break;
        
        case BAUDRATE_ASYNCH_16BIT_HIGH_SPEED:
            TXSTAbits.SYNC= EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH= EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16= EUSART_16BIT_BAUDRATE_GEN;
            BaudRate_temp= ((_XTAL_FREQ / (float)_eusart->baudrate) / 4) - 1;
        break;
        
        case BAUDRATE_SYNCH_8BIT:
            TXSTAbits.SYNC= EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16= EUSART_8BIT_BAUDRATE_GEN;
            BaudRate_temp= ((_XTAL_FREQ / (float)_eusart->baudrate) / 4) - 1;
        break;
        
        case BAUDRATE_SYNCH_16BIT:
            TXSTAbits.SYNC= EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16= EUSART_16BIT_BAUDRATE_GEN;
            BaudRate_temp= ((_XTAL_FREQ / (float)_eusart->baudrate) / 4) - 1;
        break;
    }
    
    SPBRG= (uint8_t)((uint32_t)BaudRate_temp);
    SPBRGH= (uint8_t)(((uint32_t)BaudRate_temp) >> 8);
}

static void EUSART_ASYNC_TX_Init(const usart_t *_eusart)
{
    if(EUSART_ASYNCHRONOUS_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_enable)
    {
        EUSART_ASYNCH_TX_ENABLE();
        EUSART_TxInterruptHandler = _eusart->EUSART_TxDefaultInterruptHandler;
        if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable)
        {
            EUSART_ASYNCH_INTERRUPT_TX_ENABLE();
             /* Interrupt Configurations */ 
            #ifdef EUSART_TX_INTERRUPT_FEATURE_ENABLE
                EUSART_TX_InterruptEnable();
                /* Interrupt Priority Configurations */
            #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_tx_cfg.usart_tx_int_priority){
                    /* Enables all high-priority interrupts */
                    INTERRUPT_GlobalInterruptHighEnable();
                    EUSART_TX_HighPrioritySet();
                }
                else if(INTERRUPT_LOW_PRIORITY == _eusart->usart_tx_cfg.usart_tx_int_priority){
                    /* Enables all unmasked peripheral interrupts */
                    INTERRUPT_GlobalInterruptLowEnable();
                    EUSART_TX_LowPrioritySet();
                }
                else{ /* Nothing */ }
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
            #endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable)
        {
            EUSART_ASYNCH_INTERRUPT_TX_DISABLE();
        }

        if(EUSART_ASYNCHRONOUS_9BIT_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_9bit_enable)
        {
            EUSART_ASYNCH_9BIT_TX_ENABLE();
        }
        else if(EUSART_ASYNCHRONOUS_9BIT_TX_DISABLE == _eusart->usart_tx_cfg.usart_tx_9bit_enable)
        {
            EUSART_ASYNCH_9BIT_TX_DISABLE();
        }
    }
    
}

static void EUSART_ASYNC_RX_Init(const usart_t *_eusart)
{
    if(EUSART_ASYNCHRONOUS_TX_ENABLE == _eusart->usart_rx_cfg.usart_rx_enable)
    {
        EUSART_ASYNCH_RX_ENABLE();
        EUSART_RxInterruptHandler = _eusart->EUSART_RxDefaultInterruptHandler;
        EUSART_FramingErrorHandler = _eusart->EUSART_FramingErrorHandler;
        EUSART_OverrunErrorHandler = _eusart->EUSART_OverrunErrorHandler;
        
        if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable)
        {
            EUSART_ASYNCH_INTERRUPT_RX_ENABLE();
            /* Interrupt Configurations */ 
            #ifdef EUSART_RX_INTERRUPT_FEATURE_ENABLE
                EUSART_RX_InterruptEnable();
                /* Interrupt Priority Configurations */
            #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_rx_cfg.usart_rx_int_priority){
                    /* Enables all high-priority interrupts */
                    INTERRUPT_GlobalInterruptHighEnable();
                    EUSART_RX_HighPrioritySet();
                }
                else if(INTERRUPT_LOW_PRIORITY == _eusart->usart_rx_cfg.usart_rx_int_priority){
                    /* Enables all unmasked peripheral interrupts */
                    INTERRUPT_GlobalInterruptLowEnable();
                    EUSART_RX_LowPrioritySet();
                }
                else{ /* Nothing */ }
            #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
            #endif
            #endif
        }
        else if(EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable)
        {
            EUSART_ASYNCH_INTERRUPT_RX_DISABLE();
        }

        if(EUSART_ASYNCHRONOUS_9BIT_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_9bit_enable)
        {
            EUSART_ASYNCH_9BIT_RX_ENABLE();
        }
        else if(EUSART_ASYNCHRONOUS_9BIT_RX_DISABLE == _eusart->usart_rx_cfg.usart_rx_9bit_enable)
        {
            EUSART_ASYNCH_9BIT_RX_DISABLE();
        }
    }
}

void EUSART_TX_ISR(void){
    EUSART_TX_InterruptDisable();
    if(EUSART_TxInterruptHandler)
    {
        EUSART_TxInterruptHandler();
    }
    else { /* Nothing */}
}

void EUSART_RX_ISR(void){
    if(EUSART_RxInterruptHandler)
    {
        EUSART_RxInterruptHandler();
    }
    else { /* Nothing */}
    if(EUSART_FramingErrorHandler)
    {
        EUSART_FramingErrorHandler();
    }
    else { /* Nothing */}
    if(EUSART_OverrunErrorHandler)
    {
        EUSART_OverrunErrorHandler();
    }
    else { /* Nothing */}
}
