/* 
 * File:   spi.c
 * Author: Adnan
 *
 * Created on July 29, 2023, 11:19 AM
 */
#include "spi.h"

/* ----------------------- Helper Functions Declaration ----------------------- */
#ifdef MSSP_SPI_INTERRUPT_FEATURE_ENABLE
    static void (*SPI_InterruptHandler)(void) = NULL;
#endif

static void MSSP_SPI_Interrupt_Init(const SPI_Config *_Config);
static void MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations(const SPI_Config *_Config);
static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *_Config);

/* ------------------------------------ Software Interface Definitions ------------------------------------ */
std_ReturnType SPI_Init(const SPI_Config *_Config)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _Config)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable SPI Module */
        SPI_MODE_DISABLE_SELECT();
        
        /* MSSP SPI Mode Select */
        SYNCHRONOUS_SERIAL_PORT_MODE(_Config->spi_mode);
        
        /* GPIO PIN Configurations */
        if((_Config->spi_mode == SPI_MASTER_FOSC_DIV4_CFG) || (_Config->spi_mode == SPI_MASTER_FOSC_DIV16_CFG) ||
            (_Config->spi_mode == SPI_MASTER_FOSC_DIV64_CFG) || (_Config->spi_mode == SPI_MASTER_FOSC_TMR2_CFG)){
            MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations(_Config);
        }
        else if((_Config->spi_mode == SPI_SLAVE_SS_ENABLE_CFG) || (_Config->spi_mode == SPI_SLAVE_SS_DISABLE_CFG)){
            MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(_Config);
        }
        
        /* Clock Polarity Select */
        if(SPI_IDLE_STATE_HIGH_LEVEL_CFG == _Config->spi_config.ClockPolarity)
        {
            SPI_IDLE_STATE_HIGH_LEVEL_SELECT();
        }
        else if(SPI_IDLE_STATE_LOW_LEVEL_CFG == _Config->spi_config.ClockPolarity)
        {
            SPI_IDLE_STATE_LOW_LEVEL_SELECT();
        }
        
        /* SPI Sample Select */
        if(SPI_DATA_SAMPLE_MIDDLE_CFG == _Config->spi_config.SampleSelect)
        {
            SPI_DATA_SAMPLE_AT_MIDDLE_SELECT();
        }
        else if(SPI_DATA_SAMPLE_END_CFG == _Config->spi_config.SampleSelect)
        {
            SPI_DATA_SAMPLE_AT_END_SELECT();
        }
        
        /* SPI Clock Select */
        if(SPI_TRANSMIT_ACTIVE_TO_IDLE_CFG == _Config->spi_config.ClockSelect)
        {
            SPI_TRANSMIT_ACTIVE_TO_IDLE_SELECT();
        }
        else if(SPI_TRANSMIT_IDLE_TO_ACTIVE_CFG == _Config->spi_config.ClockSelect)
        {
            SPI_TRANSMIT_IDLE_TO_ACTIVE_SELECT();
        }
        
        /* MSSP SPI Interrupt Configurations*/
        MSSP_SPI_Interrupt_Init(_Config);
        
        /* Enable SPI Module */
        SPI_MODE_ENABLE_SELECT();
        
    }
    return ret_val;
}

std_ReturnType SPI_DeInit(const SPI_Config *_Config)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _Config)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable SPI Module */
        SPI_MODE_DISABLE_SELECT();
        /* Interrupt Configurations */        
        #ifdef MSSP_SPI_INTERRUPT_FEATURE_ENABLE
                MSSP_SPI_InterruptDisable();
        #endif
        
    }
    return ret_val;
}

std_ReturnType SPI_Send_Byte(const SPI_Config *_Config, const uint8_t _data)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _Config)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        SSPBUF = _data;
        while(!(PIR1bits.SSPIF)); /* Waiting to transmit */
        PIR1bits.SSPIF = 0;
    }
    return ret_val;
}

std_ReturnType SPI_Read_Byte(const SPI_Config *_Config, uint8_t *_data)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _Config) || (NULL == _data))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        while(SSPSTATbits.BF == 0); /* Receive not complete, SSPBUF is empty */
        *_data = SSPBUF;
    }
    return ret_val;
}

std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_Config *_Config, const uint8_t _data)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _Config)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        
        
    }
    return ret_val;
}

std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_Config *_Config, uint8_t *_data)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _Config) || (NULL == _data))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        
        
    }
    return ret_val;
}

/* ----------------------- Helper Functions Definitions ----------------------- */
static void MSSP_SPI_Interrupt_Init(const SPI_Config *_Config)
{
    /* Interrupt Configurations */        
#ifdef MSSP_SPI_INTERRUPT_FEATURE_ENABLE
    MSSP_SPI_InterruptEnable();
    MSSP_SPI_InterruptFlagClear();
    SPI_InterruptHandler = _Config->MSSP_SPI_InterruptHandler;
/* Interrupt Priority Configurations */
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE 
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == Config->priority){
        /* Enables all high-priority interrupts */
        INTERRUPT_GlobalInterruptHighEnable();
        MSSP_SPI_HighPrioritySet();
    }
    else if(INTERRUPT_LOW_PRIORITY == Config->priority){
        /* Enables all unmasked peripheral interrupts */
        INTERRUPT_GlobalInterruptLowEnable();
        MSSP_SPI_LowPrioritySet();
    }
    else{ /* Nothing */ }
#else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif      
#endif  
}

static void MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations(const SPI_Config *_Config)
{
    std_ReturnType ret_val = E_NOT_OK;
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_OUTPUT};
    
    ret_val = gpio_pin_direction_initialize(&SPI_SDO); 
    ret_val = gpio_pin_direction_initialize(&SPI_SDI);
    ret_val = gpio_pin_direction_initialize(&SPI_CLK);
}

static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *_Config)
{
    std_ReturnType ret_val = E_NOT_OK;
    pin_config_t SPI_SDO = {.port = PORTC_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_OUTPUT};
    pin_config_t SPI_SDI = {.port = PORTC_INDEX, .pin = GPIO_PIN4, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_CLK = {.port = PORTC_INDEX, .pin = GPIO_PIN3, .direction = GPIO_DIRECTION_INPUT};
    pin_config_t SPI_SS = {.port = PORTA_INDEX, .pin = GPIO_PIN5, .direction = GPIO_DIRECTION_INPUT};
    
    ret_val = gpio_pin_direction_initialize(&SPI_SDO); 
    ret_val = gpio_pin_direction_initialize(&SPI_SDI);
    ret_val = gpio_pin_direction_initialize(&SPI_CLK); 
    
    if(SPI_SLAVE_SS_ENABLE_CFG == _Config->spi_mode){
        ret_val = gpio_pin_direction_initialize(&SPI_SS);
    }
}


void MSSP_SPI_ISR(void)
{
#ifdef MSSP_SPI_INTERRUPT_FEATURE_ENABLE
    MSSP_SPI_InterruptFlagClear();
    if(SPI_InterruptHandler)
    {
        SPI_InterruptHandler();
    }
#endif
}