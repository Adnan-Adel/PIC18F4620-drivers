/* 
 * File:   application.c
 * Author: LAP TECH
 *
 * Created on February 4, 2023, 1:59 PM
 */

#include "application.h"

std_ReturnType ret= E_NOT_OK;

led_t led1= {.port= PORTC_INDEX, .pin= GPIO_PIN0, .led_status= GPIO_LOW};
led_t led2= {.port= PORTC_INDEX, .pin= GPIO_PIN1, .led_status= GPIO_LOW};
led_t led3= {.port= PORTC_INDEX, .pin= GPIO_PIN2, .led_status= GPIO_LOW};

void Int0_APP_ISR(void)
{
    led_toggle(&led1);
}

void Int1_APP_ISR(void)
{
    led_toggle(&led2);
}

void Int2_APP_ISR(void)
{
    led_toggle(&led3);
}

Interrupt_INTx_t int0_obj=
{
  .EXT_InterruptHandler= Int0_APP_ISR,
  .edge= INTERRUPT_RISING_EDGE,
  .priority= INTERRUPT_HIGH_PRIORITY,
  .source= INTERRUPT_EXTERNAL_INT0,
  .mcu_pin.port= PORTB_INDEX,
  .mcu_pin.pin= GPIO_PIN0,
  .mcu_pin.direction= GPIO_DIRECTION_INPUT
};
Interrupt_INTx_t int1_obj=
{
  .EXT_InterruptHandler= Int1_APP_ISR,
  .edge= INTERRUPT_RISING_EDGE,
  .priority= INTERRUPT_LOW_PRIORITY,
  .source= INTERRUPT_EXTERNAL_INT1,
  .mcu_pin.port= PORTB_INDEX,
  .mcu_pin.pin= GPIO_PIN1,
  .mcu_pin.direction= GPIO_DIRECTION_INPUT
};
Interrupt_INTx_t int2_obj=
{
  .EXT_InterruptHandler= Int2_APP_ISR,
  .edge= INTERRUPT_RISING_EDGE,
  .priority= INTERRUPT_HIGH_PRIORITY,
  .source= INTERRUPT_EXTERNAL_INT2,
  .mcu_pin.port= PORTB_INDEX,
  .mcu_pin.pin= GPIO_PIN2,
  .mcu_pin.direction= GPIO_DIRECTION_INPUT
};

int main() {
    application_initialize();
    ret= Interrupt_INTx_Init(&int0_obj);
    ret= Interrupt_INTx_Init(&int1_obj);
    ret= Interrupt_INTx_Init(&int2_obj);
    ret= led_initialize(&led1);
    ret= led_initialize(&led2);
    ret= led_initialize(&led3);
    while(1)
    {
        
    }
    return (EXIT_SUCCESS);
}

void application_initialize(void)
{
    ecu_layer_initialize();
}   
