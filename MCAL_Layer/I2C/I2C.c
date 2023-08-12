/* 
 * File:   I2C.c
 * Author: Adnan
 *
 * Created on August 10, 2023, 10:57 PM
 */
#include "I2C.h"

/* ----------------------- Helper Functions Declaration ----------------------- */
static inline void I2C_GPIO_Configure(void);
static inline void I2C_Master_Mode_Clock_Configurations(const i2c_t* _i2c_obj);
static inline void I2C_Slave_Mode_Configurations(const i2c_t* _i2c_obj);
static inline void MSSP_I2C_Interrupt_Configurations(const i2c_t* _i2c_obj);

#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
    static void (*I2C_Report_Write_Collision_InterruptHandler)(void) = NULL;
    static void (*I2C_DefaultInterruptHandle)(void) = NULL;
    static void (*I2C_Report_Receive_Overflow_InterruptHandle)(void) = NULL;
#endif

/* ------------------------------------ Software Interface Definitions ------------------------------------ */
std_ReturnType I2C_Init(const i2c_t* _i2c_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _i2c_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable I2C */
        MSSP_MODULE_I2C_DISABLE();
        
        /* Select I2C Mode */
        if(I2C_MASTER_MODE == _i2c_obj->i2c_cfg.i2c_mode)       /* I2C Master Clock Configuration */
        {
            I2C_Master_Mode_Clock_Configurations(_i2c_obj);
        }
        else if(I2C_SLAVE_MODE == _i2c_obj->i2c_cfg.i2c_mode)   /* I2C Slave Mode General Call Configuration */
        {
            /* I2C Slave Mode General Call Configurations */
            if(I2C_GENERAL_CALL_ENABLE_CFG == _i2c_obj->i2c_cfg.i2c_general_call){
                I2C_GENERAL_CALL_ENABLE();
            }
            else if(I2C_GENERAL_CALL_DISABLE_CFG == _i2c_obj->i2c_cfg.i2c_general_call){
                I2C_GENERAL_CALL_DISABLE();
            }
            /* Clear the Write Collision Detect */
            SSPCON1bits.WCOL = 0; /* No Collision */
            /* Clear the Receive Overflow Indicator */
            SSPCON1bits.SSPOV = 0; /* No Overflow */
            /* Release the clock */
            SSPCON1bits.CKP = 1;
            /* Assign the I2C Slave Address */
            SSPADD = _i2c_obj->i2c_cfg.i2c_slave_address;
            /* I2C Slave Mode Configurations */
            I2C_Slave_Mode_Configurations(_i2c_obj);
        }
        
        /* GPIO Configuration */
        I2C_GPIO_Configure();
        
        /* Slew Rate Configuration */
        if(I2C_SLEW_RATE_ENABLE_CFG == _i2c_obj->i2c_cfg.i2c_slew_rate)
        {
            I2C_SLEW_RATE_ENABLE();
        }
        else if(I2C_SLEW_RATE_DISABLE_CFG == _i2c_obj->i2c_cfg.i2c_slew_rate)
        {
            I2C_SLEW_RATE_DISABLE();
        }
        
        /* SMBUS Configuration */
        if(I2C_SMBUS_ENABLE_CFG == _i2c_obj->i2c_cfg.i2c_SMBUS_control)
        {
            I2C_SMBUS_ENABLE();
        }
        else if(I2C_SMBUS_DISABLE_CFG == _i2c_obj->i2c_cfg.i2c_SMBUS_control)
        {
            I2C_SMBUS_DISABLE();
        }
        
        /* Interrupt Configuration */
        #ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
            MSSP_I2C_Interrupt_Configurations(_i2c_obj);
        #endif
        
        /* Enable I2C */
        MSSP_MODULE_I2C_ENABLE();
    }
    return ret_val;
}

std_ReturnType I2C_DeInit(const i2c_t* _i2c_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _i2c_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable I2C */
        MSSP_MODULE_I2C_DISABLE();
        
        /* Interrupt Configurations */
        #ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
            MSSP_I2C_InterruptDisable();
            MSSP_I2C_BUS_COL_InterruptDisable();
        #endif
    }
    return ret_val;
}

std_ReturnType I2C_Master_Send_Start(const i2c_t* _i2c_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _i2c_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Initiates Start condition on SDA and SCL pins */
        SSPCON2bits.SEN = 1; /* Initiates Start condition on SDA and SCL pins. Automatically cleared by hardware. */
        /* Wait for the completion of the Start condition */
        while(SSPCON2bits.SEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Start Condition Detection */
        if(START_BIT_DETECTED == SSPSTATbits.S)
        {
            ret_val = E_OK; /* Indicates that a Start bit has been detected last */
        }
        else{
            ret_val = E_NOT_OK; /* Start bit was not detected last */
        }
    }
    return ret_val;
}

std_ReturnType I2C_Master_Send_Repeated_Start(const i2c_t* _i2c_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _i2c_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Initiates Repeated Start condition on SDA and SCL pins */
        SSPCON2bits.RSEN = 1; /* Initiates Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware */
        /* Wait for the completion of the Repeated Start condition */
        while(SSPCON2bits.RSEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
    }
    return ret_val;
}

std_ReturnType I2C_Master_Send_Stop(const i2c_t* _i2c_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _i2c_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Initiates Stop condition on SDA and SCL pins */
        SSPCON2bits.PEN = 1; /* Initiates Stop condition on SDA and SCL pins. Automatically cleared by hardware */
        /* Wait for the completion of the Stop condition */
        while(SSPCON2bits.PEN);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Stop Condition Detection */
        if(STOP_BIT_DETECTED == SSPSTATbits.P)
        {
            ret_val = E_OK; /* Indicates that a Stop bit has been detected last */
        }
        else{
            ret_val = E_NOT_OK; /* Stop bit was not detected last */
        }
    }
    return ret_val;
}

std_ReturnType I2C_Write(const i2c_t* _i2c_obj, uint8_t _data, uint8_t* _ack)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == _i2c_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Write Data to the Data register */
        SSPBUF = _data;
        /* Wait The transmission to be completed */
        while(SSPSTATbits.BF);
        /* Clear The MSSP Interrupt Flag bit -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report the acknowledge received from the slave */
        if(I2C_ACK_RECEIVED_FROM_SLAVE == SSPCON2bits.ACKSTAT)
        {
            *_ack = I2C_ACK_RECEIVED_FROM_SLAVE; /* Acknowledge was received from slave */
        }
        else
        {
            *_ack = I2C_ACK_NOT_RECEIVED_FROM_SLAVE; /* Acknowledge was not received from slave */
        }
    }
    return ret_val;
}

std_ReturnType I2C_Read(const i2c_t* _i2c_obj, uint8_t* _data, uint8_t _ack)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == _i2c_obj) || (NULL == _data))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Master Mode Receive Enable */
        I2C_MASTER_RECEIVE_ENABLE();
        
        /* Wait for buffer full flag : A complete byte received */
        while(!SSPSTATbits.BF);
        
        /* Copy The data registers to buffer variable */
        *_data = SSPBUF;
        
        /* Send ACK or NACK after read */
        if(I2C_MASTER_SEND_ACK == _ack)
        {
            SSPCON2bits.ACKDT = 0; /* Acknowledge */
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            I2C_INITIATE_ACK_SEQUENCE(); 
            while(SSPCON2bits.ACKEN); /* Automatically cleared by hardware */
        }
        else if(I2C_MASTER_SEND_NACK == _ack)
        {
            SSPCON2bits.ACKDT = 1; /* Not Acknowledge */
            /* Initiates Acknowledge sequence on SDA and SCL pins and transmit ACKDT data bit. */
            I2C_INITIATE_ACK_SEQUENCE();
            while(SSPCON2bits.ACKEN); /* Automatically cleared by hardware */
        }
    }
    return ret_val;
}


/* ----------------------- Helper Functions Definitions ----------------------- */
static inline void I2C_GPIO_Configure(void)
{
    TRISCbits.TRISC3= 1;    /* Configure Serial Clock (SCL) to be input */
    TRISCbits.TRISC4= 1;    /* Configure Serial Data (SDA) to be input */
}

static inline void I2C_Master_Mode_Clock_Configurations(const i2c_t* _i2c_obj)
{
    I2C_MODE_SELECT(_i2c_obj->i2c_cfg.i2c_mode_cfg);
    SSPADD= (uint8_t)(((_XTAL_FREQ/4) / _i2c_obj->i2c_clock) - 1);     /* clock = FOSC/(4 * (SSPADD + 1)) */
}

static inline void I2C_Slave_Mode_Configurations(const i2c_t* _i2c_obj)
{
    I2C_MODE_SELECT(_i2c_obj->i2c_cfg.i2c_mode_cfg);
}

static inline void MSSP_I2C_Interrupt_Configurations(const i2c_t* _i2c_obj)
{
    #ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
        MSSP_I2C_InterruptEnable();
        MSSP_I2C_BUS_COL_InterruptEnable();
        MSSP_I2C_InterruptFlagClear();
        MSSP_I2C_BUS_COL_InterruptFlagClear();
        I2C_Report_Write_Collision_InterruptHandler = _i2c_obj->I2C_Report_Write_Collision;
        I2C_DefaultInterruptHandle = _i2c_obj->I2C_InterruptHandler;
        I2C_Report_Receive_Overflow_InterruptHandle = _i2c_obj->I2C_Report_Receive_Overflow;
    /* Interrupt Priority Configurations */
    #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE 
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            MSSP_I2C_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            MSSP_I2C_LowPrioritySet();
        }
        else{ /* Nothing */ }

        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_bc_priority){
            /* Enables all high-priority interrupts */
            INTERRUPT_GlobalInterruptHighEnable();
            MSSP_I2C_BUS_COL_HighPrioritySet();
        }
        else if(INTERRUPT_LOW_PRIORITY == i2c_obj->i2c_cfg.mssp_i2c_bc_priority){
            /* Enables all unmasked peripheral interrupts */
            INTERRUPT_GlobalInterruptLowEnable();
            MSSP_I2C_BUS_COL_LowPrioritySet();
        }
        else{ /* Nothing */ }
    #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
    #endif

    #endif 
}


/* ------------------------------ ISR ------------------------------ */
void MSSP_I2C_ISR(void)
{
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE    
    MSSP_I2C_InterruptFlagClear();
    if(I2C_DefaultInterruptHandle)
    {
        I2C_DefaultInterruptHandle();
    }
#endif
}

void MSSP_I2C_BC_ISR(void)
{
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    if(I2C_Report_Write_Collision_InterruptHandler)
    {
        I2C_Report_Write_Collision_InterruptHandler();
    }
#endif
}