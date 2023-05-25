/* 
 * File:   ecu_motor.c
 * Author: LAP TECH
 *
 * Created on February 13, 2023, 1:33 PM
 */
#include "ecu_motor.h"

/**
 * @brief Initialize the assigned 
 * @param dc_motor
 * @return 
 */
std_ReturnType dc_motor_initialize(const dc_motor_t *dc_motor)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == dc_motor)
    {
        ret_val= E_NOT_OK;
    }
    else
    {    
        gpio_pin_initialize(&(dc_motor->dc_motor_pins[0]));
        gpio_pin_initialize(&(dc_motor->dc_motor_pins[1]));
    }
    return ret_val;
}

/**
 * 
 * @param dc_motor
 * @return 
 */
std_ReturnType dc_motor_move_right(const dc_motor_t *dc_motor)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == dc_motor)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[0]), GPIO_HIGH);
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[1]), GPIO_LOW);
    }
    return ret_val;
}

/**
 * 
 * @param dc_motor
 * @return 
 */
std_ReturnType dc_motor_move_left(const dc_motor_t *dc_motor)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == dc_motor)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[0]), GPIO_LOW);
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[1]), GPIO_HIGH);
    }
    return ret_val;
}

/**
 * 
 * @param dc_motor
 * @return 
 */
std_ReturnType dc_motor_stop(const dc_motor_t *dc_motor)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == dc_motor)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[0]), GPIO_LOW);
        gpio_pin_write_logic(&(dc_motor->dc_motor_pins[1]), GPIO_LOW);
    }
    return ret_val;
}