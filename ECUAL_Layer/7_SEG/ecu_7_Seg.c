/* 
 * File:   ecu_7_Seg.c
 * Author: Adnan
 *
 * Created on February 16, 2023, 10:45 PM
 */
#include "ecu_7_Seg.h"

std_ReturnType seven_segment_initialize(const segment_t *seg)
{
    std_ReturnType ret_val= E_NOT_OK;
    if(NULL == seg)
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_initialize(&(seg->segment_pins[SEGMENT_PIN0]));
        ret_val= gpio_pin_initialize(&(seg->segment_pins[SEGMENT_PIN1]));
        ret_val= gpio_pin_initialize(&(seg->segment_pins[SEGMENT_PIN2]));
        ret_val= gpio_pin_initialize(&(seg->segment_pins[SEGMENT_PIN3]));
  
    }
    return ret_val;
}
std_ReturnType seven_segment_write_number(const segment_t *seg, uint8_t number)
{
    std_ReturnType ret_val= E_NOT_OK;
    if((NULL == seg) || (number > 9))
    {
        ret_val= E_NOT_OK;
    }
    else
    {
        ret_val= gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN0]), GET_BIT(number,0));
        ret_val= gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN1]), GET_BIT(number,1));
        ret_val= gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN2]), GET_BIT(number,2));
        ret_val= gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN3]), GET_BIT(number,3));
    }
    return ret_val;
}