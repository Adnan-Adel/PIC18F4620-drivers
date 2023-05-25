/* 
 * File:   hal_gpio.h
 * Author: LAP TECH
 *
 * Created on February 4, 2023, 2:07 PM
 */
#include "hal_gpio.h"

/* Reference to the Data Direction Control Register */
volatile uint8_t *tris_registers[]={&TRISA, &TRISB, &TRISC, &TRISD, &TRISE};

/* Reference to the Data Latch Register (OUTPUT REGISTER) */
volatile uint8_t *lat_registers[]={&LATA, &LATB, &LATC, &LATD, &LATE};

/* Reference to the Port Status Register */
volatile uint8_t *port_registers[]={&PORTA, &PORTB, &PORTC, &PORTD, &PORTE};

#if GPIO_PIN_CONFIGURATIONS == CONFIG_ENABLE
/**
 * @brief Initialize direction of a specific pin
 * @param pin_config: Pointer to configurations (@ref pin_config_t)
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_pin_direction_initialize(const pin_config_t *pin_config)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == pin_config || pin_config->pin > PINS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(pin_config->direction)
        {
            case GPIO_DIRECTION_OUTPUT:
                CLEAR_BIT(*tris_registers[pin_config->port], pin_config->pin);
            break;
            case GPIO_DIRECTION_INPUT:
                SET_BIT(*tris_registers[pin_config->port], pin_config->pin);
            break;
            default:
                ret_val= E_NOT_OK;
        }
    }
    return ret_val;
}

/**
 * 
 * @param pin_config: Pointer to configurations (@ref pin_config_t)
 * @param direction_status
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_pin_get_direction_status(const pin_config_t *pin_config, direction_t *direction_status)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == pin_config) || (NULL == direction_status) || (pin_config->pin > PINS_MAX_NUMBER-1))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *direction_status= GET_BIT(*tris_registers[pin_config->port], pin_config->pin);
    }
    return ret_val;
}

/**
 * 
 * @param pin_config: Pointer to configurations (@ref pin_config_t)
 * @param logic
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_pin_write_logic(const pin_config_t *pin_config, logic_t logic)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == pin_config || pin_config->pin > PINS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        switch(logic)
        {
            case GPIO_LOW:
                CLEAR_BIT(*lat_registers[pin_config->port], pin_config->pin);
            break;
            case GPIO_HIGH:
                SET_BIT(*lat_registers[pin_config->port], pin_config->pin);
            break;
            default:
                ret_val= E_NOT_OK;
        }
    }
    return ret_val;
}

/**
 * 
 * @param pin_config: Pointer to configurations (@ref pin_config_t)
 * @param logic_value
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_pin_read_logic(const pin_config_t *pin_config, logic_t *logic_value)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == pin_config) || (NULL == logic_value) || (pin_config->pin > PINS_MAX_NUMBER-1))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *logic_value= GET_BIT(*port_registers[pin_config->port], pin_config->pin);
    }
    return ret_val;
}

/**
 * 
 * @param pin_config: Pointer to configurations (@ref pin_config_t)
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_pin_toggle_logic(const pin_config_t *pin_config)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == pin_config || pin_config->pin > PINS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        TOGGLE_BIT(*lat_registers[pin_config->port], pin_config->pin);
    }
    return ret_val;
}

/**
 * 
 * @param pin_config: Pointer to configurations (@ref pin_config_t)
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_pin_initialize(const pin_config_t *pin_config)
{
    std_ReturnType ret_val= E_OK;
    if(NULL == pin_config || pin_config->pin > PINS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_direction_initialize(pin_config);
        ret_val= gpio_pin_write_logic(pin_config, pin_config->logic);
    }
    return ret_val;
}

#endif


#if GPIO_PORT_CONFIGURATIONS == CONFIG_ENABLE

/**
 * 
 * @param port
 * @param direction
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_port_direction_initialize(port_index_t port, uint8_t direction)
{
    std_ReturnType ret_val= E_OK;
    if(port > PORTS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *tris_registers[port]= direction;
    }
    
    return ret_val;
}

/**
 * 
 * @param port
 * @param direction_status
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_port_get_direction_status(port_index_t port, uint8_t *direction_status)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == direction_status) || (port > PORTS_MAX_NUMBER-1))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *direction_status= *tris_registers[port];
    }
    return ret_val;
}

/**
 * 
 * @param port
 * @param logic
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_port_write_logic(port_index_t port, uint8_t logic)
{
    std_ReturnType ret_val= E_OK;
    if(port > PORTS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *lat_registers[port]= logic;
    }
    return ret_val;
}

/**
 * 
 * @param port
 * @param logic_value
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_port_read_logic(port_index_t port, uint8_t *logic_value)
{
    std_ReturnType ret_val= E_OK;
    if((NULL == logic_value) || (port > PORTS_MAX_NUMBER-1))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *logic_value= *lat_registers[port];
    }
    return ret_val;
}

/**
 * 
 * @param port
 * @return status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : There is an issue in performing function
 */
std_ReturnType gpio_port_toggle_logic(port_index_t port)
{
    std_ReturnType ret_val= E_OK;
    if(port > PORTS_MAX_NUMBER-1)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        *lat_registers[port] ^= 0xFF;
    }
    return ret_val;
}

#endif