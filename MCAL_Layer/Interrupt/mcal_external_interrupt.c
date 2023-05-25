/* 
 * File:   mcal_external_interrupt.c
 * Author: Adnan
 *
 * Created on May 7, 2023, 5:19 PM
 */
#include "mcal_external_interrupt.h"

/* Pointer to Function to hold the callbacks-----> INTx */
static void(* INT0_InterruptHandler)(void)= NULL;
static void(* INT1_InterruptHandler)(void)= NULL;
static void(* INT2_InterruptHandler)(void)= NULL;

/* Pointer to Function to hold the callbacks -----> RBx */
static void(* RB4_InterruptHandler_HIGH)(void)= NULL;
static void(* RB4_InterruptHandler_LOW)(void)= NULL;
static void(* RB5_InterruptHandler_HIGH)(void)= NULL;
static void(* RB5_InterruptHandler_LOW)(void)= NULL;
static void(* RB6_InterruptHandler_HIGH)(void)= NULL;
static void(* RB6_InterruptHandler_LOW)(void)= NULL;
static void(* RB7_InterruptHandler_HIGH)(void)= NULL;
static void(* RB7_InterruptHandler_LOW)(void)= NULL;


static std_ReturnType Interrupt_INTx_Enable(const Interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Disable(const Interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Priority_Init(const Interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Edge_Init(const Interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Pin_Init(const Interrupt_INTx_t *int_obj);
static std_ReturnType Interrupt_INTx_Clear_Flag(const Interrupt_INTx_t *int_obj);

static std_ReturnType INT0_SetInterruptHandler(void(* InterruptHandler)(void));
static std_ReturnType INT1_SetInterruptHandler(void(* InterruptHandler)(void));
static std_ReturnType INT2_SetInterruptHandler(void(* InterruptHandler)(void));
static std_ReturnType Interrupt_INTx_SetInterruptHandler(const Interrupt_INTx_t *int_obj);
 
static std_ReturnType Interrupt_RBx_Enable(const Interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Disable(const Interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Priority_Init(const Interrupt_RBx_t *int_obj);
static std_ReturnType Interrupt_RBx_Pin_Init(const Interrupt_RBx_t *int_obj);

std_ReturnType Interrupt_INTx_Init(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the External Interrupt */
        ret_val= Interrupt_INTx_Disable(int_obj);
        
        /* Clear Interrupt Flag(External Interrupt didn't Occur) */
        ret_val= Interrupt_INTx_Clear_Flag(int_obj);
        
        /* Configure External Interrupt Edge */
        ret_val= Interrupt_INTx_Edge_Init(int_obj);
        
        #ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        /* Configure External Interrupt Priority */
        ret_val= Interrupt_INTx_Priority_Init(int_obj);
        #endif
        /* Configure External Interrupt I/O pi */
        ret_val= Interrupt_INTx_Pin_Init(int_obj);
        
        /* Configure Default Interrupt CallBack */
        ret_val= Interrupt_INTx_SetInterruptHandler(int_obj);
        /* Enable External Interrupt */
        ret_val= Interrupt_INTx_Enable(int_obj);
    }
    
    return ret_val;
}

void INT0_ISR(void)
{
    /* The INT0 External Interrupt Occured (must be cleared in software) */
    EXT_INT0_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(INT0_InterruptHandler != NULL)
    {
        INT0_InterruptHandler();
    }
}

void INT1_ISR(void)
{
    /* The INT1 External Interrupt Occured (must be cleared in software) */
    EXT_INT1_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(INT1_InterruptHandler != NULL)
    {
        INT1_InterruptHandler();
    }
}

void INT2_ISR(void)
{
    /* The INT2 External Interrupt Occured (must be cleared in software) */
    EXT_INT2_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(INT2_InterruptHandler != NULL)
    {
        INT2_InterruptHandler();
    }
}

std_ReturnType Interrupt_INTx_DeInit(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= Interrupt_INTx_Disable(int_obj);
    }
    
    return ret_val;
}

std_ReturnType Interrupt_RBx_Init(const Interrupt_RBx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        /* Disable the External Interrupt */
        EXT_RBx_InterruptDisable();
        
        /* Clear Interrupt Flag(External Interrupt didn't Occur) */
        EXT_RBx_InterruptFlagClear();
        
        
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
        {
            INTERRUPT_GlobalInterruptLowEnable();
            
            EXT_RBx_Priority_Low();
        }
        else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
        {
            INTERRUPT_GlobalInterruptHighEnable();
            
            EXT_RBx_Priority_Low();
        }
#else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
#endif
        
        /* Configure the RBx pin to be input */
        Interrupt_RBx_Pin_Init(int_obj);
        
        /* Initialize call back functions */
        switch(int_obj->mcu_pin.pin)
        {
            case GPIO_PIN4:
                RB4_InterruptHandler_HIGH= int_obj->EXT_InterruptHandler_HIGH;
                RB4_InterruptHandler_LOW= int_obj->EXT_InterruptHandler_LOW;
                break;
            case GPIO_PIN5:
                RB5_InterruptHandler_HIGH= int_obj->EXT_InterruptHandler_HIGH;
                RB5_InterruptHandler_LOW= int_obj->EXT_InterruptHandler_LOW;
                break;
            case GPIO_PIN6:
                RB6_InterruptHandler_HIGH= int_obj->EXT_InterruptHandler_HIGH;
                RB6_InterruptHandler_LOW= int_obj->EXT_InterruptHandler_LOW;
                break;
            case GPIO_PIN7:
                RB7_InterruptHandler_HIGH= int_obj->EXT_InterruptHandler_HIGH;
                RB7_InterruptHandler_LOW= int_obj->EXT_InterruptHandler_LOW;
                break;
            default:
                ret_val= E_NOT_OK;
        }
        /* Enable the External Interrupt */
        EXT_RBx_InterruptEnable();
    }
    
    return ret_val;
}

std_ReturnType Interrupt_RBx_DeInit(const Interrupt_RBx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        
    }
    
    return ret_val;
}

void RB4_ISR(uint8_t RB4_src)
{
    /* The RB4 External Interrupt Occured (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(0 == RB4_src)
    {
        if(RB4_InterruptHandler_HIGH != NULL)
        {
            RB4_InterruptHandler_HIGH();
        }
        
    }
    else if(1 == RB4_src)
    {
        if(RB4_InterruptHandler_LOW != NULL)
        {
            RB4_InterruptHandler_LOW();
        }
    }
    else {/* Nothing */}
    
}

void RB5_ISR(uint8_t RB5_src)
{
    /* The RB5 External Interrupt Occured (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(0 == RB5_src)
    {
        if(RB5_InterruptHandler_HIGH != NULL)
        {
            RB5_InterruptHandler_HIGH();
        }
    }
    else if(1 == RB5_src)
    {
        if(RB5_InterruptHandler_LOW != NULL)
        {
            RB5_InterruptHandler_LOW();
        }
    }
    else {/* Nothing */}
    
}

void RB6_ISR(uint8_t RB6_src)
{
    /* The RB6 External Interrupt Occured (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(0 == RB6_src)
    {
        if(RB6_InterruptHandler_HIGH != NULL)
        {
            RB6_InterruptHandler_HIGH();
        }
    }
    else if(1 == RB6_src)
    {
        if(RB6_InterruptHandler_LOW != NULL)
        {
            RB6_InterruptHandler_LOW();
        }
    }
    else {/* Nothing */}
}

void RB7_ISR(uint8_t RB7_src)
{
    /* The RB7 External Interrupt Occured (must be cleared in software) */
    EXT_RBx_InterruptFlagClear();
    
    /* Code to be executed from MCAL Interrupt Context */
    
    /* Callback Function gets called every time this ISR executes */
    if(0 == RB7_src)
    {
        if(RB7_InterruptHandler_HIGH != NULL)
        {
            RB7_InterruptHandler_HIGH();
        }
    }
    else if(1 == RB7_src)
    {
        if(RB7_InterruptHandler_LOW != NULL)
        {
            RB7_InterruptHandler_LOW();
        }
    }
    else {/* Nothing */}
}

/* ----------------------------------- INTx Helper Functions ----------------------------------- */
static std_ReturnType Interrupt_INTx_Enable(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(int_obj->source)
        {
            case INTERRUPT_EXTERNAL_INT0:
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
                INTERRUPT_GlobalInterruptHighEnable();
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT0_InterruptEnable();
                break;
            case INTERRUPT_EXTERNAL_INT1:
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                {
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                {
                    INTERRUPT_GlobalInterruptHighEnable();
                }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT1_InterruptEnable();
                break;
            case INTERRUPT_EXTERNAL_INT2:
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                {
                    INTERRUPT_GlobalInterruptLowEnable();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                {
                    INTERRUPT_GlobalInterruptHighEnable();
                }
#else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
#endif
                EXT_INT2_InterruptEnable();
                break;
            default:
                ret_val= E_NOT_OK;
                break;
        }
    }
    
    return ret_val;
}

static std_ReturnType Interrupt_INTx_Disable(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(int_obj->source)
        {
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptDisable();
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptDisable();
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptDisable();
                break;
            default:
                ret_val= E_NOT_OK;
                break;
        }
    }
    
    return ret_val;
}

#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
static std_ReturnType Interrupt_INTx_Priority_Init(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(int_obj->source)
        {
            case INTERRUPT_EXTERNAL_INT1:
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                {
                    EXT_INT1_LowPrioritySet();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                {
                    EXT_INT1_HighPrioritySet();
                }
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if(INTERRUPT_LOW_PRIORITY == int_obj->priority)
                {
                    EXT_INT2_LowPrioritySet();
                }
                else if(INTERRUPT_HIGH_PRIORITY == int_obj->priority)
                {
                    EXT_INT2_HighPrioritySet();
                }
                break;
        }
    }
    
    return ret_val;
}
#endif

static std_ReturnType Interrupt_INTx_Edge_Init(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(int_obj->source)
        {
             case INTERRUPT_EXTERNAL_INT0:
                if(INTERRUPT_FALLING_EDGE == int_obj->edge)
                {
                    EXTERNAL_IN0_FallingEdgeSet();
                }
                else if(INTERRUPT_RISING_EDGE == int_obj->edge)
                {
                    EXTERNAL_IN0_RisingEdgeSet();
                }
                break;
            case INTERRUPT_EXTERNAL_INT1:
                if(INTERRUPT_FALLING_EDGE == int_obj->edge)
                {
                    EXTERNAL_IN1_FallingEdgeSet();
                }
                else if(INTERRUPT_RISING_EDGE == int_obj->edge)
                {
                    EXTERNAL_IN1_RisingEdgeSet();
                }
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if(INTERRUPT_FALLING_EDGE == int_obj->edge)
                {
                    EXTERNAL_IN2_FallingEdgeSet();
                }
                else if(INTERRUPT_RISING_EDGE == int_obj->edge)
                {
                    EXTERNAL_IN2_RisingEdgeSet();
                }
                break;
        }
    }
    
    return ret_val;
}

static std_ReturnType Interrupt_INTx_Pin_Init(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_direction_initialize(&(int_obj->mcu_pin));
    }
    
    return ret_val;
}

static std_ReturnType Interrupt_INTx_Clear_Flag(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(int_obj->source)
        {
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptFlagClear();
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptFlagClear();
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptFlagClear();
                break;
            default:
                ret_val= E_NOT_OK;
                break;
        }
    }
    return ret_val;
}

/* ------------------------------------------------------------------------------------------- */
static std_ReturnType INT0_SetInterruptHandler(void(* InterruptHandler)(void))
{
    std_ReturnType ret_val= E_OK;
    if(NULL == InterruptHandler)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        INT0_InterruptHandler= InterruptHandler;
    }
    return ret_val;
}

static std_ReturnType INT1_SetInterruptHandler(void(* InterruptHandler)(void))
{
    std_ReturnType ret_val= E_OK;
    if(NULL == InterruptHandler)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        INT1_InterruptHandler= InterruptHandler;
    }
    return ret_val;
}

static std_ReturnType INT2_SetInterruptHandler(void(* InterruptHandler)(void))
{
    std_ReturnType ret_val= E_OK;
    if(NULL == InterruptHandler)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        INT2_InterruptHandler= InterruptHandler;
    }
    return ret_val;
}

static std_ReturnType Interrupt_INTx_SetInterruptHandler(const Interrupt_INTx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(int_obj->source)
        {
            case INTERRUPT_EXTERNAL_INT0:
                ret_val= INT0_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT1:
                ret_val= INT1_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT2:
                ret_val= INT2_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            default:
                ret_val= E_NOT_OK;
                break;
        }
    }
    return ret_val;
}

/* ----------------------------------- RBx Helper Functions ----------------------------------- */
static std_ReturnType Interrupt_RBx_Enable(const Interrupt_RBx_t *int_obj)
{
    
}

static std_ReturnType Interrupt_RBx_Disable(const Interrupt_RBx_t *int_obj)
{
    
}

#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
static std_ReturnType Interrupt_RBx_Priority_Init(const Interrupt_RBx_t *int_obj)
{
    
}

#endif
static std_ReturnType Interrupt_RBx_Pin_Init(const Interrupt_RBx_t *int_obj)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == int_obj)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_direction_initialize(&(int_obj->mcu_pin));
    }
    
    return ret_val;
}
